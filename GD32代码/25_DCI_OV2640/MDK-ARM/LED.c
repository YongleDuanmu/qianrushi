#include "LED.h"
void LED_Init()
{
	//1.开启LED所对应的时钟
	rcu_periph_clock_enable(RCU_GPIOF);
	//2.设置GPIO模式
	gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO_PIN_7);
	//3.设置GPIO模式的详细参数 输出速度 推挽还是开漏
	gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_7);
	
	//1.开启LED所对应的时钟
	rcu_periph_clock_enable(RCU_GPIOF);
	//2.设置GPIO模式
	gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO_PIN_8);
	//3.设置GPIO模式的详细参数 输出速度 推挽还是开漏
	gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_8);
	
	//1.开启LED所对应的时钟
	rcu_periph_clock_enable(RCU_GPIOE);
	//2.设置GPIO模式
	gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO_PIN_3);
	//3.设置GPIO模式的详细参数 输出速度 推挽还是开漏
	gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_3);
	
	//1.开启LED所对应的时钟
	rcu_periph_clock_enable(RCU_GPIOE);
	//2.设置GPIO模式
	gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO_PIN_2);
	//3.设置GPIO模式的详细参数 输出速度 推挽还是开漏
	gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_2);
	
}
void LED_On()
{
  gpio_bit_set(GPIOF, GPIO_PIN_7);
}
void LED_off()
{
gpio_bit_reset(GPIOF, GPIO_PIN_7);
}
void LED_tog()
{
  gpio_bit_toggle(GPIOF, GPIO_PIN_7);
}
void LED4_ON()
{
	gpio_bit_set(GPIOE, GPIO_PIN_2);
}
void LED4_OFF()
{
  gpio_bit_reset(GPIOE, GPIO_PIN_2);
}


