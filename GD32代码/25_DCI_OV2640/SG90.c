#include "SG90.h"

void SG90_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_TIMER0);

    /* 2. 配置PA11为复用功能 */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_11); // TIMER0_CH3对应AF1

    /* 3. 配置TIMER0 */
    timer_parameter_struct timer_initpara;

    // 反初始化TIMER0
    timer_deinit(TIMER0);

    // 配置TIMER0基本参数
    timer_initpara.prescaler = 100 - 1; // 240分频，假设系统时钟240MHz，得到1MHz计数时钟
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = 20000 - 1; // 20ms周期 (1MHz / 20000 = 50Hz)
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0, &timer_initpara);

    /* 4. 配置TIMER0 CH3输出比较模式 */
    timer_oc_parameter_struct timer_ocintpara;

    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER0, TIMER_CH_3, &timer_ocintpara);

    /* 5. 设置默认占空比(1.5ms对应90度) */
    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_3, 1500); // 1.5ms高电平

    /* 6. 配置CH3模式为PWM模式0 */
    timer_channel_output_mode_config(TIMER0, TIMER_CH_3, TIMER_OC_MODE_PWM0);

    /* 7. 使能TIMER0 CH3 */
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);

    /* 8. 使能TIMER0 */
    timer_enable(TIMER0);
}

void SG90_SetAngle(uint16_t angle)
{
    uint32_t pulse_width;

    // 限制角度范围在0-180度
    if (angle > 180)
    {
        angle = 180;
    }

    // 计算脉冲宽度 (单位: 微秒)
    // 脉冲宽度 = 500 + (angle * 2000 / 180) = 500 + (angle * 100 / 9)
    pulse_width = 500 + (angle * 2000) / 180;

    // 设置TIMER0 CH3比较值
    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_3, pulse_width);
}