#include "gd32f5xx.h"
#include "systick.h"
#include <stdio.h>
#include "dci_ov2640.h"
#include "picture.h"
#include "exmc_sdram.h"
#include "main.h"
#include "RC522.h"
#include "TFTLCD.h"
#include "UART.h"
#include "SG90.h"
#include "esp8266.h"
#include "onenet.h"
#include "LED.h"
// 在全局变量区域添加

static void lcd_gpio_config(void);
static void nvic_configuration(void);
extern int64_t tick_count;
unsigned char buf[20]; // 存放卡序号

#define ESP8266_ONENET_INFO "AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"

tli_parameter_struct tli_initstruct;
tli_layer_parameter_struct tli_layer0_initstruct;
tli_layer_parameter_struct tli_layer1_initstruct;

// 添加全局变量用于记录拍照时间
int64_t capture_time = 0;
int32_t led_status = 0; // 0表示关闭，1表示打开
int32_t door = 0;
int64_t doortime = 0;
char cardid[9];
int main(void)
{
    ov2640_id_struct ov2640id;
    systick_config();
    nvic_configuration();
    UART_Init();
    UART3_Init(); // 初始化UART3
    SG90_Init();  // 初始化舵机
    LED_Init();
    /* initialize SDRAM */
    exmc_synchronous_dynamic_ram_init(EXMC_SDRAM_DEVICE0);
    // 初始化外部存储器控制器(EXMC)连接的同步动态随机存储器(SDRAM)
    // 这个函数会配置SDRAM的各种参数，如时序、容量、数据宽度等
    delay_1ms(1000);
    /* configure key */
    key_config();

    /* initialize camera */
    dci_ov2640_init();             // 图像数据通过DCI传输给CPU，SCCB来配置和控制摄像头芯片本身的工作状态。
    dci_ov2640_id_read(&ov2640id); // 验证摄像头是否正确连接和识别

    /* enable DMA interrupt and channel */
    dma_interrupt_enable(DMA1, DMA_CH7, DMA_CHXCTL_FTFIE);
    dma_channel_enable(DMA1, DMA_CH7); // DCI接口到SDRAM 的图像数据传输

    /* enable DCI */
    dci_enable();
    dci_capture_enable();
    delay_1ms(100);

    /* configure LCD and enable TLI */
    // TLI是GD32的LCD-TFT显示控制器，支持多层显示合成。
    lcd_config();
    tli_layer_enable(LAYER0);
    tli_layer_enable(LAYER1);

    tli_reload_config(TLI_REQUEST_RELOAD_EN);
    tli_enable();
    uint32_t counter = 0;

    /* initialize MFRC522 */
    MFRC522_Init();

    ESP8266_Init();
    while (ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
        delay_1ms(500);
    while (OneNet_DevLink()) // 接入OneNET
    {
        ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT");
        delay_1ms(500);
    }

    OneNET_Subscribe();   // 订阅设备
    unsigned char status; // RC522返回符
    unsigned int temp;

    delay_1ms(1000);
    temp = 0;
    unsigned char *dataPtr = NULL;
    while (1)
    {
        // 检查是否有RFID卡
        status = MFRC522_Request(PICC_REQALL, buf); // 寻卡
        if (status != MI_OK)
        {
            MFRC522_Reset();
            MFRC522_AntennaOff();
            MFRC522_AntennaOn();
        }
        else
        {
            // 防冲撞，获取卡的序列号
            status = MFRC522_Anticoll(buf);
            if (status == MI_OK)
            {
                char card_id_str[9]; // 4字节卡号转换为8个字符 + 1结束符
                uint8_t i;
                byte_array_to_hex_string(buf, 4, card_id_str);
                temp = 1;
                // 显示卡信息
                clear_text_area();
                draw_string_on_image(10, 32, card_id_str, 0xFFFF); // 白色
                for (i = 0; i < 9; i++)
                    cardid[i] = card_id_str[i];
                if (card_id_str[0] == '4')
                {
                    gpio_bit_set(GPIOF, GPIO_PIN_8);
                    gpio_bit_reset(GPIOE, GPIO_PIN_3);
                    usart_data_transmit(USART1, 'A');
                    delay_1ms(500);

                } // 圆卡
                if (card_id_str[0] == '5')
                {
                    gpio_bit_set(GPIOE, GPIO_PIN_3);
                    gpio_bit_reset(GPIOF, GPIO_PIN_8);
                    usart_data_transmit(USART1, 'B');
                    delay_1ms(500);
                } // 方卡
            }
        }
        if (temp == 0)
        {
            clear_text_area();
            draw_string_on_image(10, 20, "PLEASE VERIFY", 0xF800); // 红色"PLEASE VERIFY"}
        }
        if (temp == 1)
        {
            draw_string_on_image(10, 20, "VERIFY SUCCESS", 0x07FF); // 蓝色"WAITING CARD"
        }
        // 发送数据到OneNET
          OneNet_SendData();
					delay_1ms(500);
				// 检查是否有来自OneNET的命令
        dataPtr = ESP8266_GetIPD(10);
        if (dataPtr != NULL)
        {
            OneNet_RevPro(dataPtr);
        }
        delay_1ms(10);
    }
}

/*!
    \brief      key configuration
    \param[in]  none
    \param[out] none
    \retval     none
*/
void key_config(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_SYSCFG);

    /* configure wakeup key interrupt */
    {
        /* configure button pin as input */
        gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_0);

        /* connect key EXTI line to key GPIO pin */
        syscfg_exti_line_config(EXTI_SOURCE_GPIOA, EXTI_SOURCE_PIN0);

        /* configure key EXTI line0 */
        exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(EXTI_0);

        /* enable and set key EXTI interrupt priority */
        nvic_irq_enable(EXTI0_IRQn, 1U, 1U);
    }

    /* configure tamper key interrupt */
    {
        /* configure PC13 pin */
        gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_13);

        /* connect EXTI line13 to PC13 pin */
        syscfg_exti_line_config(EXTI_SOURCE_GPIOC, EXTI_SOURCE_PIN13);

        /* configure key EXTI line13 */
        exti_init(EXTI_13, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(EXTI_13);
    }
    /* configure user key interrupt */
    {
        /* configure PB14 pin */
        gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_14);

        /* connect EXTI line14 to PB14 pin */
        syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN14);

        /* configure EXTI line14 */
        exti_init(EXTI_14, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(EXTI_14);

        nvic_irq_enable(EXTI10_15_IRQn, 1U, 0U);
    }
}

/*!
    \brief      save image to sdram
    \param[in]  none
    \param[out] none
    \retval     none
*/
void image_save(void)
{
    uint32_t i = 0;

    dma_interrupt_disable(DMA1, DMA_CH7, DMA_CHXCTL_FTFIE);
    dma_channel_disable(DMA1, DMA_CH7);
    dci_capture_disable();

    /* save image to sdram */
    for (i = 0; i < 32640; i++)
    {
        *(uint32_t *)(0XC0800000 + 4 * i) = *(uint32_t *)(0XC1000000 + 4 * i);
    }
}

/*!
    \brief      display image to lcd
    \param[in]  diapaly_image_addr: address of display image
    \param[out] none
    \retval     none
*/
void image_display(uint32_t display_image_addr)
{
    /* input address configuration */
    tli_layer1_initstruct.layer_frame_bufaddr = (uint32_t)display_image_addr;

    tli_layer_init(LAYER1, &tli_layer1_initstruct);
    /* enable layer0 */
    tli_layer_enable(LAYER0);
    /* enable layer1 */
    tli_layer_enable(LAYER1);
    /* reload configuration */
    tli_reload_config(TLI_REQUEST_RELOAD_EN);

    /* enable TLI */
    tli_enable();
}
/*!
    \brief      LCD configure
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lcd_config(void)
{
    rcu_periph_clock_enable(RCU_TLI);

    lcd_gpio_config();

    /* enable pixel clock */
    /* configure PLLSAI prescalers for LCD */
    if (ERROR == rcu_pllsai_r_config(240, 2))
    {
        while (1)
        {
        }
    }
    rcu_tli_clock_div_config(RCU_PLLSAIR_DIV8);
    rcu_osci_on(RCU_PLLSAI_CK);
    if (ERROR == rcu_osci_stab_wait(RCU_PLLSAI_CK))
    {
        while (1)
        {
        }
    }

    /* TLI initialization */
    /* initialize the horizontal synchronization polarity as active low */
    tli_initstruct.signalpolarity_hs = TLI_HSYN_ACTLIVE_LOW;
    /* initialize the vertical synchronization polarity as active low */
    tli_initstruct.signalpolarity_vs = TLI_VSYN_ACTLIVE_LOW;
    /* initialize the data enable polarity as active low */
    tli_initstruct.signalpolarity_de = TLI_DE_ACTLIVE_LOW;
    /* initialize the pixel clock polarity as input pixel clock */
    tli_initstruct.signalpolarity_pixelck = TLI_PIXEL_CLOCK_TLI;

    /* timing configuration */
    /* configure horizontal synchronization width */
    tli_initstruct.synpsz_hpsz = 40;
    /* configure vertical synchronization height */
    tli_initstruct.synpsz_vpsz = 9;
    /* configure accumulated horizontal back porch */
    tli_initstruct.backpsz_hbpsz = 42;
    /* configure accumulated vertical back porch */
    tli_initstruct.backpsz_vbpsz = 11;
    /* configure accumulated active width */
    tli_initstruct.activesz_hasz = 522;
    /* configure accumulated active height */
    tli_initstruct.activesz_vasz = 283;
    /* configure total width */
    tli_initstruct.totalsz_htsz = 524;
    /* configure total height */
    tli_initstruct.totalsz_vtsz = 285;

    /* configure R,G,B component values for LCD background color */
    tli_initstruct.backcolor_red = 0xFF;
    tli_initstruct.backcolor_green = 0xFF;
    tli_initstruct.backcolor_blue = 0xFF;

    tli_init(&tli_initstruct);

    /* layer0 windowing configuration */
    tli_layer0_initstruct.layer_window_leftpos = 43;
    tli_layer0_initstruct.layer_window_rightpos = (43 + 480 - 1);
    tli_layer0_initstruct.layer_window_toppos = 12;
    tli_layer0_initstruct.layer_window_bottompos = (12 + 272 - 1);

    /* pixel format configuration */
    tli_layer0_initstruct.layer_ppf = LAYER_PPF_RGB565;

    /* alpha constant configuration : the constant alpha for layer 0 is decreased
    to see the layer 0 in the intersection zone */
    tli_layer0_initstruct.layer_sa = 255;

    /* default color configuration (configure A,R,G,B component values) */
    tli_layer0_initstruct.layer_default_blue = 0xFF;
    tli_layer0_initstruct.layer_default_green = 0xFF;
    tli_layer0_initstruct.layer_default_red = 0xFF;
    tli_layer0_initstruct.layer_default_alpha = 0xFF;

    /* blending factors */
    tli_layer0_initstruct.layer_acf1 = LAYER_ACF1_SA;
    tli_layer0_initstruct.layer_acf2 = LAYER_ACF1_SA;

    /* configure input address : frame buffer is located at FLASH memory */
    tli_layer0_initstruct.layer_frame_bufaddr = (uint32_t)image_background0;

    tli_layer0_initstruct.layer_frame_line_length = ((480 * 2) + 3);
    tli_layer0_initstruct.layer_frame_buf_stride_offset = (480 * 2);

    tli_layer0_initstruct.layer_frame_total_line_number = 272;

    tli_layer_init(LAYER0, &tli_layer0_initstruct);

    /* layer1 windowing configuration */
    tli_layer1_initstruct.layer_window_leftpos = 160;
    tli_layer1_initstruct.layer_window_rightpos = (160 + 240 - 1);
    tli_layer1_initstruct.layer_window_toppos = 12;
    tli_layer1_initstruct.layer_window_bottompos = (12 + 272 - 1);

    /* pixel format configuration */
    tli_layer1_initstruct.layer_ppf = LAYER_PPF_RGB565;

    /* alpha constant configuration : the constant alpha for layer 1 is decreased
    to see the layer 0 in the intersection zone */
    tli_layer1_initstruct.layer_sa = 255;

    /* default color configuration (configure A,R,G,B component values) */
    tli_layer1_initstruct.layer_default_blue = 0xFF;
    tli_layer1_initstruct.layer_default_green = 0xFF;
    tli_layer1_initstruct.layer_default_red = 0xFF;
    tli_layer1_initstruct.layer_default_alpha = 0;

    /* blending factors */
    tli_layer1_initstruct.layer_acf1 = LAYER_ACF1_PASA;
    tli_layer1_initstruct.layer_acf2 = LAYER_ACF1_PASA;

    /* configure input address : frame buffer is located at memory */
    tli_layer1_initstruct.layer_frame_bufaddr = (uint32_t)0xC1000000;
    // 直接把从DCI接收到的图像数据存储在SDRAM中的地址显示在前景层中

    tli_layer1_initstruct.layer_frame_line_length = ((240 * 2) + 3);
    tli_layer1_initstruct.layer_frame_buf_stride_offset = (240 * 2);

    tli_layer1_initstruct.layer_frame_total_line_number = 272;

    tli_layer_init(LAYER1, &tli_layer1_initstruct);

    tli_dither_config(TLI_DITHER_ENABLE);
}

/*!
    \brief      configure the nested vectored interrupt controller
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void nvic_configuration(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    /* enable DMA1 channel 7 */
    nvic_irq_enable(DMA1_Channel7_IRQn, 0U, 0U);
}

/*!
    \brief      LCD GPIO configure
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void lcd_gpio_config(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOH);
    rcu_periph_clock_enable(RCU_GPIOI);
    rcu_periph_clock_enable(RCU_GPIOG);
    rcu_periph_clock_enable(RCU_GPIOF);

    /* configure TLI pins AF */
    gpio_af_set(GPIOE, GPIO_AF_14, GPIO_PIN_5);
    gpio_af_set(GPIOE, GPIO_AF_14, GPIO_PIN_6);
    gpio_af_set(GPIOE, GPIO_AF_14, GPIO_PIN_4);

    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_2);
    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_3);
    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_8);
    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_9);
    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_10);
    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_11);
    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_12);
    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_13);
    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_14);
    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_15);

    gpio_af_set(GPIOI, GPIO_AF_14, GPIO_PIN_0);
    gpio_af_set(GPIOI, GPIO_AF_14, GPIO_PIN_1);
    gpio_af_set(GPIOI, GPIO_AF_14, GPIO_PIN_2);
    gpio_af_set(GPIOI, GPIO_AF_14, GPIO_PIN_4);
    gpio_af_set(GPIOI, GPIO_AF_14, GPIO_PIN_5);
    gpio_af_set(GPIOI, GPIO_AF_14, GPIO_PIN_6);
    gpio_af_set(GPIOI, GPIO_AF_14, GPIO_PIN_7);
    gpio_af_set(GPIOI, GPIO_AF_14, GPIO_PIN_9);
    gpio_af_set(GPIOI, GPIO_AF_14, GPIO_PIN_10);

    gpio_af_set(GPIOG, GPIO_AF_14, GPIO_PIN_6);
    gpio_af_set(GPIOG, GPIO_AF_14, GPIO_PIN_7);
    gpio_af_set(GPIOG, GPIO_AF_14, GPIO_PIN_10);
    gpio_af_set(GPIOG, GPIO_AF_14, GPIO_PIN_11);
    gpio_af_set(GPIOG, GPIO_AF_14, GPIO_PIN_12);

    gpio_af_set(GPIOF, GPIO_AF_14, GPIO_PIN_10);

    /* configure HSYNC(PI10), VSYNC(PI9), PCLK(PG7) */
    /* configure LCD_R7(PG6), LCD_R6(PH12), LCD_R5(PH11), LCD_R4(PH10), LCD_R3(PH9),LCD_R2(PH8),
                 LCD_R1(PH3), LCD_R0(PH2), LCD_G7(PI2), LCD_G6(PI1), LCD_G5(PI0), LCD_G4(PH15),
                 LCD_G3(PH14), LCD_G2(PH13),LCD_G1(PE6), LCD_G0(PE5),LCD_B7(PI7), LCD_B6(PI6),
                 LCD_B5(PI5), LCD_B4(PI4), LCD_B3(PG11),LCD_B2(PG10), LCD_B1(PG12), LCD_B0(PE4) */
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);

    gpio_mode_set(GPIOH, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_output_options_set(GPIOH, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

    gpio_mode_set(GPIOI, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10);
    gpio_output_options_set(GPIOI, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10);

    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12);

    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* configure LCD_PWM_BackLight(PB15) */
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_15);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);

    gpio_bit_set(GPIOB, GPIO_PIN_15);
}

int fputc(int ch, FILE *f)
{
    // 等待发送缓冲区为空
    while (RESET == usart_flag_get(UART3, USART_FLAG_TBE))
        ;
    // 发送数据
    usart_data_transmit(UART3, (uint8_t)ch);
    return ch;
}
void USART1_IRQHandler(void)
{
    if (usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE) != RESET)
    {
        // 读取接收到的数据，防止中断一直触发
        uint8_t data = usart_data_receive(USART1);

        if (data == 0x01)
        {
            LED_On();
            printf("led on\r\n");
            led_status = 1;
        }
        if (data == 0x02)
        {
            LED_off();
            printf("led off\r\n");
            led_status = 0;
        }
        if (data == 0x03)
        {
            image_save();
            image_display((uint32_t)image_background1);
            // 拍照并记录时间
            capture_time = get_tick_count(); // 假设有获取系统时间的函数
            printf("pai zhao\r\n");
        }
        if (data == 0x04)
            image_display((uint32_t)0XC0800000);
        if (data == 0x05)
        {
            //            SG90_SetAngle(0);
            door = 1;
            doortime = tick_count;
            printf("door open\r\n");
        }
        if (data == 0x06)
        {
            //            SG90_SetAngle(90);
            door = 0;
            printf("door off\r\n");
        }
        if (data == 0x07)
        {
            printf("carm open\r\n");
            tli_layer_disable(LAYER0);
            tli_layer_disable(LAYER1);
            /* reload configuration */
            tli_reload_config(TLI_REQUEST_RELOAD_EN);
            delay_1ms(100);
            dci_ov2640_init();
            dma_interrupt_enable(DMA1, DMA_CH7, DMA_CHXCTL_FTFIE);
            dma_channel_enable(DMA1, DMA_CH7);
            delay_1ms(10);
            /* enable DCI interface */
            dci_enable();
            /* start image capture */
            dci_capture_enable();
            delay_1ms(100);
            lcd_config();
            /* enable layer0 */
            tli_layer_enable(LAYER0);
            /* enable layer1 */
            tli_layer_enable(LAYER1);
            /* reload configuration */
            tli_reload_config(TLI_REQUEST_RELOAD_EN);
            /* enable tli */
            tli_enable();
        }
        // 清除中断标志（通常读取数据后会自动清除）
        usart_interrupt_flag_clear(USART1, USART_INT_FLAG_RBNE);
    }
}
