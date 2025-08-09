#include "LED.h"
void LED_Init()
{
	//1.����LED����Ӧ��ʱ��
	rcu_periph_clock_enable(RCU_GPIOF);
	//2.����GPIOģʽ
	gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO_PIN_7);
	//3.����GPIOģʽ����ϸ���� ����ٶ� ���컹�ǿ�©
	gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_7);
	
	//1.����LED����Ӧ��ʱ��
	rcu_periph_clock_enable(RCU_GPIOF);
	//2.����GPIOģʽ
	gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO_PIN_8);
	//3.����GPIOģʽ����ϸ���� ����ٶ� ���컹�ǿ�©
	gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_8);
	
	//1.����LED����Ӧ��ʱ��
	rcu_periph_clock_enable(RCU_GPIOE);
	//2.����GPIOģʽ
	gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO_PIN_3);
	//3.����GPIOģʽ����ϸ���� ����ٶ� ���컹�ǿ�©
	gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_3);
	
	//1.����LED����Ӧ��ʱ��
	rcu_periph_clock_enable(RCU_GPIOE);
	//2.����GPIOģʽ
	gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO_PIN_2);
	//3.����GPIOģʽ����ϸ���� ����ٶ� ���컹�ǿ�©
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


