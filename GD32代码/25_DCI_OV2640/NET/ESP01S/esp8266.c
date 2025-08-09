// 单片机头文件
#include "main.h"

// 网络设备驱动
#include "esp8266.h"

// 硬件驱动
#include "systick.h"
#include "gd32f5xx.h"

// C库
#include <string.h>
#include <stdio.h>

#define ESP8266_WIFI_INFO "AT+CWJAP=\"K80\",\"dm030206\"\r\n"

uint8_t huart3_rev_byte;

unsigned char esp8266_buf[512];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;


void Usart2_SendString(unsigned char *str, unsigned short len)
{
   unsigned short count = 0;
//	 printf("uart3 send %s\r\n",str);
	 for(; count < len; count++)
	{
		usart_data_transmit(USART2, *str++); // 发送一个字节数据
		while (RESET == usart_flag_get(USART2, USART_FLAG_TC)); // 发送完成判断
	}



}
//==========================================================
//	函数名称：	ESP8266_Clear
//
//	函数功能：	清空缓存
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：
//==========================================================
void ESP8266_Clear(void)
{

	memset(esp8266_buf, 0, sizeof(esp8266_buf));
	esp8266_cnt = 0;
}

//==========================================================
//	函数名称：	ESP8266_WaitRecive
//
//	函数功能：	等待接收完成
//
//	入口参数：	无
//
//	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
//
//	说明：		循环调用检测是否接收完成
//==========================================================
_Bool ESP8266_WaitRecive(void)
{

	if (esp8266_cnt == 0) // 如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return REV_WAIT;

	if (esp8266_cnt == esp8266_cntPre) // 如果上一次的值和这次相同，则说明接收完毕
	{
		esp8266_cnt = 0; // 清0接收计数

		return REV_OK; // 返回接收完成标志
	}

	esp8266_cntPre = esp8266_cnt; // 置为相同

	return REV_WAIT; // 返回接收未完成标志
}

//==========================================================
//	函数名称：	ESP8266_SendCmd
//
//	函数功能：	发送命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//
//	返回参数：	0-成功	1-失败
//
//	说明：
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res)
{

	unsigned char timeOut = 200;

	// Usart_SendString(USART2, (unsigned char *)cmd, strlen((const char *)cmd));
	// HAL_UART_Transmit(&huart2, (uint8_t *)cmd, strlen((const char *)cmd), 500);
	Usart2_SendString((unsigned char *)cmd, strlen((const char *)cmd));
	while (timeOut--)
	{
		if (ESP8266_WaitRecive() == REV_OK) // 如果收到数据
		{
			if (strstr((const char *)esp8266_buf, res) != NULL) // 如果检索到关键词
			{
				ESP8266_Clear(); // 清空缓存

				return 0;
				printf("receive ok\r\n");
			}
		}

		delay_1ms(10);
	}

	return 1;
}

//==========================================================
//	函数名称：	ESP8266_SendData
//
//	函数功能：	发送数据
//
//	入口参数：	data：数据
//				len：长度
//
//	返回参数：	无
//
//	说明：
//==========================================================
void ESP8266_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];

	ESP8266_Clear();						   // 清空接收缓存
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len); // 发送命令
	if (!ESP8266_SendCmd(cmdBuf, ">"))		   // 收到‘>’时可以发送数据
	{
		// Usart_SendString(USART2, data, len);		//发送设备连接请求数据
		// HAL_UART_Transmit(&huart2, (uint8_t *)data, len, 500);
		Usart2_SendString(data, len);
	}
}

//==========================================================
//	函数名称：	ESP8266_GetIPD
//
//	函数功能：	获取平台返回的数据
//
//	入口参数：	等待的时间(乘以10ms)
//
//	返回参数：	平台返回的原始数据
//
//	说明：		不同网络设备返回的格式不同，需要去调试
//				如ESP8266的返回格式为	"+IPD,x:yyy"	x代表数据长度，yyy是数据内容
//==========================================================
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{

	char *ptrIPD = NULL;

	do
	{
		if (ESP8266_WaitRecive() == REV_OK) // 如果接收完成
		{
			ptrIPD = strstr((char *)esp8266_buf, "IPD,"); // 搜索“IPD”头
			if (ptrIPD == NULL)							  // 如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
			{
				// printf("\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':'); // 找到':'
				if (ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
			}
		}

		delay_1ms(5); // 延时等待
	} while (timeOut--);

	return NULL; // 超时还未找到，返回空指针
}

//==========================================================
//	函数名称：	ESP8266_Init
//
//	函数功能：	初始化ESP8266
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：
//==========================================================
void ESP8266_Init(void)
{

	ESP8266_Clear();

	printf("1. AT\r\n");
	// OLED_Clear(); OLED_ShowString(0,0,"1.AT...",8);
	while (ESP8266_SendCmd("AT\r\n", "OK"))
		delay_1ms(500);

	printf("2. CWMODE\r\n");
	// OLED_ShowString(0,2,"2.CWMODE...",8);
	while (ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK"))
		delay_1ms(500);

	printf("3. AT+CWDHCP\r\n");
	// OLED_ShowString(0,4,"3.AT+CWDHCP...",8);
	while (ESP8266_SendCmd("AT+CWDHCP=1,1\r\n", "OK"))
		delay_1ms(500);

	printf("4. CWJAP\r\n");
	// OLED_ShowString(0,6,"4.CWJAP...",8);
	while (ESP8266_SendCmd(ESP8266_WIFI_INFO, "GOT IP"))
		delay_1ms(500);

	printf("5. ESP8266 Init OK\r\n");
	// OLED_Clear(); OLED_ShowString(0,0,"ESP8266 Init OK",16); delay_ms(500);
}

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//     if (huart == &huart2)
//     {
//         if(esp8266_cnt >= sizeof(esp8266_buf))	esp8266_cnt = 0; //防止串口被刷爆
//		esp8266_buf[esp8266_cnt++] = huart2_rev_byte;

//        HAL_UART_Receive_IT(&huart2, &huart2_rev_byte, 1);
//    }
//}
//void UART3_IRQHandler(void)
//{
//    if (usart_interrupt_flag_get(UART3, USART_INT_FLAG_RBNE) != RESET)
//    {
//			 // 清除中断标志
//				
//        if (esp8266_cnt >= sizeof(esp8266_buf))
//            esp8266_cnt = 0; // 防止串口被刷爆
//				     huart3_rev_byte = usart_data_receive(UART3);
//            esp8266_buf[esp8266_cnt++] = huart3_rev_byte;
//				usart_interrupt_flag_clear(UART3, USART_INT_FLAG_RBNE);
//				if (usart_interrupt_flag_get(UART3, USART_INT_FLAG_RBNE_ORERR) != RESET) 
//    {
//        // 清除溢出错误标志
//        usart_interrupt_flag_clear(UART3, USART_INT_FLAG_RBNE_ORERR);
//    }
//    }
//		
//}

void USART2_IRQHandler(void)
{
    if (usart_interrupt_flag_get(USART2, USART_INT_FLAG_RBNE) != RESET)
    {
			 // 清除中断标志
				
        if (esp8266_cnt >= sizeof(esp8266_buf))
            esp8266_cnt = 0; // 防止串口被刷爆
				     huart3_rev_byte = usart_data_receive(USART2);
            esp8266_buf[esp8266_cnt++] = huart3_rev_byte;
				usart_interrupt_flag_clear(USART2, USART_INT_FLAG_RBNE);
				if (usart_interrupt_flag_get(USART2, USART_INT_FLAG_RBNE_ORERR) != RESET) 
    {
        // 清除溢出错误标志
        usart_interrupt_flag_clear(USART2, USART_INT_FLAG_RBNE_ORERR);
    }
    }
		
}

