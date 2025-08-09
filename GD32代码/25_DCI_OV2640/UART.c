#include "UART.h"
#include "LED.h"
#include "dci_ov2640.h"
#include "picture.h"
#include "systick.h"

//串口1连接语音识别
//串口2 esp01s
//串口3 printf调试
void UART_Init()
{
	/* 1. 使能UART1 */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART1);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2 | GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2 | GPIO_PIN_3);
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_2 | GPIO_PIN_3);
	
    usart_deinit(USART1);
    usart_baudrate_set(USART1, 115200U);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART1, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART1, USART_CTS_DISABLE);
	
    nvic_irq_enable(USART1_IRQn, 1U, 1U);           // 配置USART1中断优先级
    usart_interrupt_enable(USART1, USART_INT_RBNE); // 使能接收缓冲区非空中断
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_enable(USART1);
	
		/* 2. 使能UART2 */
		rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_USART2);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10 | GPIO_PIN_11);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10 | GPIO_PIN_11);
    gpio_af_set(GPIOB, GPIO_AF_7, GPIO_PIN_10 | GPIO_PIN_11);

    usart_deinit(USART2);
    usart_baudrate_set(USART2, 115200U);
    usart_word_length_set(USART2, USART_WL_8BIT);
    usart_stop_bit_set(USART2, USART_STB_1BIT);
    usart_parity_config(USART2, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART2, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART2, USART_CTS_DISABLE);
		
    nvic_irq_enable(USART2_IRQn, 1U, 1U);           // 配置USART1中断优先级
    usart_interrupt_enable(USART2, USART_INT_RBNE); // 使能接收缓冲区非空中断
		
    /* 5. 使能UART2 */
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);
    usart_enable(USART2);
}


void UART3_Init()
{

    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_UART3);

   

    /* configure UART3 Tx (PC10) as alternate function push-pull */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* configure UART3 Rx (PC11) as alternate function pull-up input */
    gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_11);
//    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
	
		/* connect port to UART3 */
    gpio_af_set(GPIOC, GPIO_AF_8, GPIO_PIN_10 | GPIO_PIN_11);
	
    /* UART3 configuration */
    usart_deinit(UART3);
    usart_baudrate_set(UART3, 115200U); // ESP01S 默认波特率通常是 115200
    usart_word_length_set(UART3, USART_WL_8BIT);
    usart_stop_bit_set(UART3, USART_STB_1BIT);
    usart_parity_config(UART3, USART_PM_NONE);
    usart_hardware_flow_rts_config(UART3, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(UART3, USART_CTS_DISABLE);
    usart_receive_config(UART3, USART_RECEIVE_ENABLE);
    usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);
//		
//		// 1. 使能UART接收中断
//		// 2. 配置NVIC中断优先级并使能中断
//		nvic_irq_enable(UART3_IRQn, 0U, 0U); 
//		usart_interrupt_enable(UART3, USART_INT_RBNE);  // 使能接收缓冲区非空中断
	
		usart_receive_config(UART3, USART_RECEIVE_ENABLE);
    usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);
    /* enable UART3 */
    usart_enable(UART3);
		
	
}




