#include "RC522.h"

#define MAXRLEN 18
#define RC522_DELAY() delay_1ms(1)

void MFRC522_Init(void)

{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);

    /* 配置 SPI_RC522_SPI 引脚：SDA */
    gpio_mode_set(MFRC522_GPIO_SDA_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, MFRC522_GPIO_SDA_PIN);
    gpio_output_options_set(MFRC522_GPIO_SDA_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, MFRC522_GPIO_SDA_PIN);

    /* 配置 SPI_RC522_SPI 引脚：SCK */
    gpio_mode_set(MFRC522_GPIO_SCK_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, MFRC522_GPIO_SCK_PIN);
    gpio_output_options_set(MFRC522_GPIO_SCK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, MFRC522_GPIO_SCK_PIN);

    /* 配置 SPI_RC522_SPI 引脚：MOSI */
    gpio_mode_set(MFRC522_GPIO_MOSI_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, MFRC522_GPIO_MOSI_PIN);
    gpio_output_options_set(MFRC522_GPIO_MOSI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, MFRC522_GPIO_MOSI_PIN);

    /* 配置 SPI_RC522_SPI 引脚：MISO */
    gpio_mode_set(MFRC522_GPIO_MISO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, MFRC522_GPIO_MISO_PIN);

    /* 配置 SPI_RC522_SPI 引脚：RST */
    gpio_mode_set(MFRC522_GPIO_RST_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, MFRC522_GPIO_RST_PIN);
    gpio_output_options_set(MFRC522_GPIO_RST_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, MFRC522_GPIO_RST_PIN);
}

/////////////////////////////////////////////////////////////////////
// 功能描述从MFRC522的某一寄存器读一个字节数据
// 参数说明：Address[IN]:寄存器地址
// 返    回：读出的值
/////////////////////////////////////////////////////////////////////
unsigned char Read_MFRC522(unsigned char Address)
{
    unsigned char i, ucAddr;
    unsigned char ucResult = 0;

    MFRC522_SCK_L;
    RC522_DELAY();
    MFRC522_SDA_L;
    RC522_DELAY();
    ucAddr = ((Address << 1) & 0x7E) | 0x80;

    for (i = 8; i > 0; i--)
    {

        if (ucAddr & 0x80)
        {
            MFRC522_MOSI_H;
        }
        else
        {
            MFRC522_MOSI_L;
        }
        RC522_DELAY();
        MFRC522_SCK_H;
        RC522_DELAY();
        ucAddr <<= 1;
        MFRC522_SCK_L;
        RC522_DELAY();
    }

    for (i = 8; i > 0; i--)
    {
        MFRC522_SCK_H;
        RC522_DELAY();
        ucResult <<= 1;
        ucResult |= MFRC522_MISO_READ;
        MFRC522_SCK_L;
        RC522_DELAY();
    }

    MFRC522_SCK_H;
    RC522_DELAY();
    MFRC522_SDA_H;
    RC522_DELAY();
    return ucResult;
}

/////////////////////////////////////////////////////////////////////
// 功能描述向MFRC522的某一寄存器写一个字节数据
// 参数说明：Address[IN]:寄存器地址
//           value[IN]:写入的值
/////////////////////////////////////////////////////////////////////
void Write_MFRC522(unsigned char Address, unsigned char value)
{
    unsigned char i, ucAddr;

    MFRC522_SCK_L;
    MFRC522_SDA_L;
    ucAddr = ((Address << 1) & 0x7E);

    for (i = 8; i > 0; i--)
    {
        if (ucAddr & 0x80)
        {
            MFRC522_MOSI_H;
        }
        else
        {
            MFRC522_MOSI_L;
        }
        RC522_DELAY();
        MFRC522_SCK_H;
        RC522_DELAY();
        ucAddr <<= 1;
        MFRC522_SCK_L;
        RC522_DELAY();
    }

    for (i = 8; i > 0; i--)
    {
        if (value & 0x80)
        {
            MFRC522_MOSI_H;
        }
        else
        {
            MFRC522_MOSI_L;
        }
        RC522_DELAY();
        MFRC522_SCK_H;
        RC522_DELAY();
        value <<= 1;
        MFRC522_SCK_L;
        RC522_DELAY();
    }

    MFRC522_SCK_H;
    RC522_DELAY();
    MFRC522_SDA_H;
    RC522_DELAY();
}

/////////////////////////////////////////////////////////////////////
// 功    能：复位RC522
// 返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char MFRC522_Reset(void)
{
    // unsigned char i;
    MFRC522_RST_H;
    delay_1ms(1);
    MFRC522_RST_L;
    delay_1ms(1);
    MFRC522_RST_H;
    delay_1ms(1);

    // MFRC522_RST_H;
    Write_MFRC522(CommandReg, 0x0F); // soft reset
    while (Read_MFRC522(CommandReg) & 0x10)
        ; // wait chip start ok

    delay_1ms(1);

    Write_MFRC522(ModeReg, 0x3D); // 和Mifare卡通讯，CRC初始值0x6363
    Write_MFRC522(TReloadRegL, 30);
    Write_MFRC522(TReloadRegH, 0);
    Write_MFRC522(TModeReg, 0x8D);
    Write_MFRC522(TPrescalerReg, 0x3E);
    Write_MFRC522(TxAutoReg, 0x40);
    return MI_OK;
}

/////////////////////////////////////////////////////////////////////
// 功    能：置RC522寄存器位
// 参数说明：reg[IN]:寄存器地址
//           mask[IN]:置位值
/////////////////////////////////////////////////////////////////////
void SetBitMask(unsigned char reg, unsigned char mask)
{
    char tmp = 0x0;
    tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp | mask); // set bit mask
}

/////////////////////////////////////////////////////////////////////
// 功    能：清RC522寄存器位
// 参数说明：reg[IN]:寄存器地址
//           mask[IN]:清位值
/////////////////////////////////////////////////////////////////////
void ClearBitMask(unsigned char reg, unsigned char mask)
{
    char tmp = 0x0;
    tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp & ~mask); // clear bit mask
}

/////////////////////////////////////////////////////////////////////
// 功    能：通过RC522和ISO14443卡通讯
// 参数说明：Command[IN]:RC522命令字
//           pInData[IN]:通过RC522发送到卡片的数据
//           InLenByte[IN]:发送数据的字节长度
//           pOutData[OUT]:接收到的卡片返回数据
//           *pOutLenBit[OUT]:返回数据的位长度
/////////////////////////////////////////////////////////////////////
char MFRC522_ToCard(unsigned char Command,
                    unsigned char *pInData,
                    unsigned char InLenByte,
                    unsigned char *pOutData,
                    unsigned int *pOutLenBit)
{
    char status = MI_ERR;
    unsigned char irqEn = 0x00;
    unsigned char waitFor = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;
    switch (Command)
    {
    case PCD_AUTHENT:
        irqEn = 0x12;
        waitFor = 0x10;
        break;
    case PCD_TRANSCEIVE:
        irqEn = 0x77;
        waitFor = 0x30;
        break;
    default:
        break;
    }

    Write_MFRC522(ComIEnReg, irqEn | 0x80); // PCD_TRANSCEIVE模式 无命令改变
    ClearBitMask(ComIrqReg, 0x80);          // IRQ开漏输出
    Write_MFRC522(CommandReg, PCD_IDLE);    // 取消当前命令
    SetBitMask(FIFOLevelReg, 0x80);         // 清除FIFO Flash 内ErrReg  BufferOvfl标志

    for (i = 0; i < InLenByte; i++)
    {
        Write_MFRC522(FIFODataReg, pInData[i]); // 把数据存到FIFO
    }
    Write_MFRC522(CommandReg, Command); // 发送FIFO内容

    if (Command == PCD_TRANSCEIVE)
    {
        SetBitMask(BitFramingReg, 0x80); // 立即停止定时器
    }

    n = Read_MFRC522(ComIrqReg);
    i = 1500; // 根据时钟频率调整，操作M1卡最大等待时间25ms
    do
    {
        n = Read_MFRC522(ComIrqReg);

        i--;

    } while ((i != 0) && !(n & 0x01) && !(n & waitFor));
    ClearBitMask(BitFramingReg, 0x80);

    if (i != 0)
    {
        if (!(Read_MFRC522(ErrorReg) & 0x1B))
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {
                status = MI_NOTAGERR;
            }
            if (Command == PCD_TRANSCEIVE)
            {
                n = Read_MFRC522(FIFOLevelReg);
                lastBits = Read_MFRC522(ControlReg) & 0x07;
                if (lastBits)
                {
                    *pOutLenBit = (n - 1) * 8 + lastBits;
                }
                else
                {
                    *pOutLenBit = n * 8;
                }
                if (n == 0)
                {
                    n = 1;
                }
                if (n > MAXRLEN)
                {
                    n = MAXRLEN;
                }
                for (i = 0; i < n; i++)
                {
                    pOutData[i] = Read_MFRC522(FIFODataReg);
                }
            }
        }
        else
        {
            status = MI_ERR;
        }
    }

    SetBitMask(ControlReg, 0x80); // stop timer now
    Write_MFRC522(CommandReg, PCD_IDLE);
    return status;
}

// 开启天线
// 每次启动或关闭天线发射之间应至少有1ms的间隔
void MFRC522_AntennaOn(void)
{
    unsigned char i;
    i = Read_MFRC522(TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}

// 关闭天线
void MFRC522_AntennaOff(void)
{
    ClearBitMask(TxControlReg, 0x03);
}

// 功能描述用MF522计算CRC
// 输入参数pIndata--要读数CRC的数据len--数据长度pOutData--计算的CRC结果
void CalulateCRC(unsigned char *pIndata, unsigned char len, unsigned char *pOutData)
{
    unsigned char i, n;
    ClearBitMask(DivIrqReg, 0x04);
    Write_MFRC522(CommandReg, PCD_IDLE);
    SetBitMask(FIFOLevelReg, 0x80);
    for (i = 0; i < len; i++)
    {
        Write_MFRC522(FIFODataReg, *(pIndata + i));
    }
    Write_MFRC522(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do
    {
        n = Read_MFRC522(DivIrqReg);
        i--;
    } while ((i != 0) && !(n & 0x04));
    pOutData[0] = Read_MFRC522(CRCResultRegL);
    pOutData[1] = Read_MFRC522(CRCResultRegM);
}

/////////////////////////////////////////////////////////////////////
// 功    能：命令卡片进入休眠状态
// 返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char MFRC522_Halt(void)
{

    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN];
    char status;
    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

    status = MFRC522_ToCard(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);

    return MI_OK;
}

/////////////////////////////////////////////////////////////////////
// 功    能：寻卡
// 参数说明: req_code[IN]:寻卡方式
//                 0x52 = 寻感应区内所有符合14443A标准的卡
//                 0x26 = 寻未进入休眠状态的卡
//           pTagType[OUT]：卡片类型代码
//                 0x4400 = Mifare_UltraLight
//                 0x0400 = Mifare_One(S50)
//                 0x0200 = Mifare_One(S70)
//                 0x0800 = Mifare_Pro(X)
//                 0x4403 = Mifare_DESFire
// 返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char MFRC522_Request(unsigned char req_code, unsigned char *pTagType)
{
    char status;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN];

    ClearBitMask(Status2Reg, 0x08);
    Write_MFRC522(BitFramingReg, 0x07);
    SetBitMask(TxControlReg, 0x03);

    ucComMF522Buf[0] = req_code;

    status = MFRC522_ToCard(PCD_TRANSCEIVE, ucComMF522Buf, 1, ucComMF522Buf, &unLen);

    if ((status == MI_OK) && (unLen == 0x10))
    {
        *pTagType = ucComMF522Buf[0];
        *(pTagType + 1) = ucComMF522Buf[1];
    }
    else
    {
        status = MI_ERR;
    }

    return status;
}

// 功    能：防冲突检测读取选中卡片的卡序列号
// 参数说明: pSnr[OUT]:卡片序列号，4字节
// 返    回: 成功返回MI_OK
char MFRC522_Anticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i, snr_check = 0;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN];

    ClearBitMask(Status2Reg, 0x08);
    Write_MFRC522(BitFramingReg, 0x00);
    ClearBitMask(CollReg, 0x80);

    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = MFRC522_ToCard(PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, &unLen);

    if (status == MI_OK)
    {
        for (i = 0; i < 4; i++)
        {
            *(pSnr + i) = ucComMF522Buf[i];
            snr_check ^= ucComMF522Buf[i];
        }
        if (snr_check != ucComMF522Buf[i])
        {
            status = MI_ERR;
        }
    }

    SetBitMask(CollReg, 0x80);
    return status;
}

/////////////////////////////////////////////////////////////////////
// 功    能：选定卡片
// 参数说明: pSnr[IN]:卡片序列号，4字节
// 返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char MFRC522_SelectTag(unsigned char *pSnr)
{
    char status;
    unsigned char i;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i = 0; i < 4; i++)
    {
        ucComMF522Buf[i + 2] = *(pSnr + i);
        ucComMF522Buf[6] ^= *(pSnr + i);
    }
    CalulateCRC(ucComMF522Buf, 7, &ucComMF522Buf[7]);

    ClearBitMask(Status2Reg, 0x08);

    status = MFRC522_ToCard(PCD_TRANSCEIVE, ucComMF522Buf, 9, ucComMF522Buf, &unLen);

    if ((status == MI_OK) && (unLen == 0x18))
    {
        status = MI_OK;
    }
    else
    {
        status = MI_ERR;
    }

    return status;
}

/////////////////////////////////////////////////////////////////////
// 功    能：验证卡片密码
// 参数说明: auth_mode[IN]: 密码验证模式
//                  0x60 = 验证A密钥
//                  0x61 = 验证B密钥
//           addr[IN]：块地址
//           pKey[IN]：密码
//           pSnr[IN]：卡片序列号，4字节
// 返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char MFRC522_AuthState(unsigned char auth_mode, unsigned char addr, unsigned char *pKey, unsigned char *pSnr)
{
    char status;
    unsigned int unLen;
    unsigned char i, ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i = 0; i < 6; i++)
    {
        ucComMF522Buf[i + 2] = *(pKey + i);
    }
    for (i = 0; i < 6; i++)
    {
        ucComMF522Buf[i + 8] = *(pSnr + i);
    }
    //   memcpy(&ucComMF522Buf[2], pKey, 6);
    //   memcpy(&ucComMF522Buf[8], pSnr, 4);

    status = MFRC522_ToCard(PCD_AUTHENT, ucComMF522Buf, 12, ucComMF522Buf, &unLen);
    if ((status != MI_OK) || (!(Read_MFRC522(Status2Reg) & 0x08)))
    {
        status = MI_ERR;
    }

    return status;
}

/////////////////////////////////////////////////////////////////////
// 功    能：读取M1卡一块数据
// 参数说明: addr[IN]：块地址
//           pData[OUT]：读出的数据，16字节
// 返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char MFRC522_Read(unsigned char addr, unsigned char *pData)
{
    char status;
    unsigned int unLen;
    unsigned char i, ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

    status = MFRC522_ToCard(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);
    if ((status == MI_OK) && (unLen == 0x90))
    //   {   memcpy(pData, ucComMF522Buf, 16);   }
    {
        for (i = 0; i < 16; i++)
        {
            *(pData + i) = ucComMF522Buf[i];
        }
    }
    else
    {
        status = MI_ERR;
    }

    return status;
}

/////////////////////////////////////////////////////////////////////
// 功    能：写数据到M1卡一块
// 参数说明: addr[IN]：块地址
//           pData[IN]：写入的数据，16字节
// 返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char MFRC522_Write(unsigned char addr, unsigned char *pData)
{
    char status;
    unsigned int unLen;
    unsigned char i, ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

    status = MFRC522_ToCard(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {
        status = MI_ERR;
    }

    if (status == MI_OK)
    {
        // memcpy(ucComMF522Buf, pData, 16);
        for (i = 0; i < 16; i++)
        {
            ucComMF522Buf[i] = *(pData + i);
        }
        CalulateCRC(ucComMF522Buf, 16, &ucComMF522Buf[16]);

        status = MFRC522_ToCard(PCD_TRANSCEIVE, ucComMF522Buf, 18, ucComMF522Buf, &unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {
            status = MI_ERR;
        }
    }

    return status;
}
