# 嵌入式项目-基于GD32F527I-EVAL的多媒体门禁系统

#### 介绍
基于GD32F527I-EVAL的多媒体门禁系统，主控GD32F527I-EVAL，具备摄像头采集图像(DMA直接到SDRAM)，保存照片，查看照片。DCI数字图像接口、TLI接口驱动TFT-LCD屏幕，ESP01S(MQTT协议)联网上传数据，语音识别控制开门、开灯等，RFID门禁卡识别，手机APP远程监控与控制(HTTP协议)。
视频展示：
【基于 GD32F527I-EVAL 的多媒体智能门禁系统-哔哩哔哩】 https://b23.tv/hOGykGD
GD32多媒体门禁系统硬件连接关系表
语音识别模块、MFRC522、SG90舵机、TFTLCD、OV2640摄像头、ESP01S、LED灯、调试串口、按键输入。详细硬件连接关系表如下：

2.1 OV2640摄像头模块
通过DCI（数字相机接口）连接到GD32：

DCI_PIXCLK (PA6)
DCI_VSYNC (PG9)
DCI_HSYNC (PA4)
DCI_D0 (PC6)
DCI_D1 (PC7)
DCI_D2 (PC8)
DCI_D3 (PC9)
DCI_D4 (PC11)
DCI_D5 (PD3)
DCI_D6 (PB8)
DCI_D7 (PB9)
SCCB_SCL (PA4) - 用于配置摄像头
SCCB_SDA (PC6) - 用于配置摄像头
2.2 TFTLCD显示屏
通过TLI（LCD-TFT控制器）连接：

LCD_R7 (PG6)
LCD_R6 (PH12)
LCD_R5 (PH11)
LCD_R4 (PH10)
LCD_R3 (PH9)
LCD_R2 (PH8)
LCD_R1 (PH3)
LCD_R0 (PH2)
LCD_G7 (PI2)
LCD_G6 (PI1)
LCD_G5 (PI0)
LCD_G4 (PH15)
LCD_G3 (PH14)
LCD_G2 (PH13)
LCD_G1 (PE6)
LCD_G0 (PE5)
LCD_B7 (PI7)
LCD_B6 (PI6)
LCD_B5 (PI5)
LCD_B4 (PI4)
LCD_B3 (PG11)
LCD_B2 (PG10)
LCD_B1 (PG12)
LCD_B0 (PE4)
LCD_HSYNC (PI10)
LCD_VSYNC (PI9)
LCD_PCLK (PG7)
LCD_PWM_BackLight (PB15)
2.3 MFRC522 RFID读卡器模块
通过软件模拟SPI接口连接：

SDA (PA15) - 片选信号
SCK (PB3) - 时钟信号
MOSI (PB5) - 主机输出从机输入
MISO (PB4) - 主机输入从机输出
RST (PA8) - 复位信号
2.4 ESP01S WiFi模块
通过UART2串口连接：

TX (PB10) - 发送数据到ESP01S
RX (PB11) - 从ESP01S接收数据
2.5 语音识别模块
通过UART1串口连接：

TX (PA2) - 发送数据到语音识别模块
RX (PA3) - 从语音识别模块接收数据
2.6 SG90舵机
通过TIMER0_CH3 PWM信号控制：

PWM信号 (PA11) - 控制舵机角度
2.7 指示LED灯
通过GPIO控制：

LED1 (PF8) - 一个指示灯
LED2 (PE3) - 另一个指示灯
2.8 调试串口
通过UART3连接（用于printf调试输出）：

TX (PC10) - 调试信息输出
RX (PC11) - 接收调试命令
2.9 按键输入
通过外部中断连接：

WAKEUP按键 (PA0) - 外部中断0
TAMPER按键 (PC13) - 外部中断13
USER按键 (PB14) - 外部中断14