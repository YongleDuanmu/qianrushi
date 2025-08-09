#include "TFTLCD.h"

void byte_array_to_hex_string(unsigned char *byte_array, int len, char *hex_string)
{
    const char hex_chars[] = "0123456789ABCDEF";
    int i;
    for (i = 0; i < len; i++)
    {
        hex_string[i * 2] = hex_chars[(byte_array[i] >> 4) & 0x0F];
        hex_string[i * 2 + 1] = hex_chars[byte_array[i] & 0x0F];
    }
    hex_string[len * 2] = '\0'; // 添加字符串结束符
}

// 绘制字符函数 - 支持英文大写字母和空格
void draw_char_on_image(uint16_t x, uint16_t y, char ch, uint16_t color)
{
    uint16_t *frame_buffer = (uint16_t *)0xC1000000;

    // 确保坐标在有效范围内
    if (x >= 240 || y >= 272)
        return;

    // 绘制字符 - 10x14字体
    switch (ch)
    {
    case '0':
        // 绘制'0'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2 && i > 1 && i < 12) || // 左侧竖线
                    (j > 7 && i > 1 && i < 12) || // 右侧竖线
                    (i < 2 && j > 1 && j < 8) ||  // 顶部横线
                    (i > 11 && j > 1 && j < 8))   // 底部横线
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case '1':
        // 绘制'1'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if (j > 3 && j < 6) // 中间竖线
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case '2':
        // 绘制'2'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((i < 2) ||          // 顶部横线
                    (i > 5 && i < 8) || // 中间横线
                    (i > 11) ||         // 底部横线
                    (j > 7 && i < 7) || // 右侧上半部分
                    (j < 2 && i > 7))   // 左侧下半部分
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case '3':
        // 绘制'3'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((i < 2) ||          // 顶部横线
                    (i > 5 && i < 8) || // 中间横线
                    (i > 11) ||         // 底部横线
                    (j > 7 && i < 12))  // 右侧竖线
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case '4':
        // 绘制'4'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j > 7) ||          // 右侧竖线
                    (i > 5 && i < 8) || // 中间横线
                    (j < 2 && i < 6))   // 左侧上半部分
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case '5':
        // 绘制'5'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((i < 2) ||          // 顶部横线
                    (i > 5 && i < 8) || // 中间横线
                    (i > 11) ||         // 底部横线
                    (j < 2 && i < 7) || // 左侧上半部分
                    (j > 7 && i > 7))   // 右侧下半部分
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case '6':
        // 绘制'6'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2) ||                  // 左侧竖线
                    (i < 2) ||                  // 顶部横线
                    (i > 5 && i < 8) ||         // 中间横线
                    (i > 11) ||                 // 底部横线
                    (j > 7 && i > 7 && i < 12)) // 右侧下半部分
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case '7':
        // 绘制'7'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((i < 2) ||         // 顶部横线
                    (j > 7 && i < 12)) // 右侧斜线
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case '8':
        // 绘制'8'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2 && i > 1 && i < 12) || // 左侧竖线
                    (j > 7 && i > 1 && i < 12) || // 右侧竖线
                    (i < 2) ||                    // 顶部横线
                    (i > 5 && i < 8) ||           // 中间横线
                    (i > 11))                     // 底部横线
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case '9':
        // 绘制'9'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2 && i < 12) || // 左侧竖线
                    (j > 7) ||           // 右侧竖线
                    (i < 2) ||           // 顶部横线
                    (i > 5 && i < 8) ||  // 中间横线
                    (i > 11))            // 底部横线
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;
    case 'B':
        // 绘制'B'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2) ||                                 // 左侧竖线
                    (i < 2) || (i > 5 && i < 8) || (i > 11) || // 三条横线
                    (j > 7 && i > 1 && i < 7) ||               // 右侧上半部分
                    (j > 7 && i > 7 && i < 12))                // 右侧下半部分
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'H':
        // 绘制'H'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2) || (j > 7) || // 左右两侧竖线
                    (i > 5 && i < 8))     // 中间横线
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'J':
        // 绘制'J'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if (j > 7 ||                      // 右侧竖线
                    (i > 11 && j > 1 && j < 8) || // 底部横线
                    (j < 2 && i > 9 && i < 12))   // 左下角短竖线
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'K':
        // 绘制'K'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if (j < 2 ||                              // 左侧竖线
                    (j > 2 && j < 5 && i > 4 && i < 9) || // 中间左斜线
                    (j > 5 && j < 8 && i > 0 && i < 5) || // 右上斜线
                    (j > 5 && j < 8 && i > 9 && i < 14))  // 右下斜线
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'M':
        // 绘制'M'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if (j < 2 || j > 7 ||                     // 左右两侧竖线
                    (j > 1 && j < 4 && i > 0 && i < 5) || // 左上斜线
                    (j > 5 && j < 8 && i > 0 && i < 5))   // 右上斜线
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'O':
        // 绘制'O'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2 && i > 1 && i < 12) || // 左侧竖线
                    (j > 7 && i > 1 && i < 12) || // 右侧竖线
                    (i < 2 && j > 1 && j < 8) ||  // 顶部横线
                    (i > 11 && j > 1 && j < 8))   // 底部横线
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'Q':
        // 绘制'Q'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2 && i > 1 && i < 12) || // 左侧竖线
                    (j > 7 && i > 1 && i < 12) || // 右侧竖线
                    (i < 2 && j > 1 && j < 8) ||  // 顶部横线
                    (i > 11 && j > 1 && j < 8) || // 底部横线
                    (j > 5 && i > 9 && i < 12))   // 右下角斜线
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'X':
        // 绘制'X'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2 && i < 5) ||                   // 左上斜线
                    (j > 7 && i < 5) ||                   // 右上斜线
                    (j > 2 && j < 5 && i > 4 && i < 9) || // 中间部分
                    (j < 2 && i > 9) ||                   // 左下斜线
                    (j > 7 && i > 9))                     // 右下斜线
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'Z':
        // 绘制'Z'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if (i < 2 || i > 11 ||               // 顶部和底部横线
                    (j > 2 && j < 8 && i + j == 12)) // 中间斜线
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;
    case 'P':
        // 绘制'P'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2) ||          // 左侧竖线
                    (i < 2) ||          // 顶部横线
                    (i > 5 && i < 8) || // 中间横线
                    (j > 7 && i < 8 && i > 1))
                { // 右侧上半部分
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'L':
        // 绘制'L'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2) || // 左侧竖线
                    (i > 11 && j < 8))
                { // 底部横线
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'E':
        // 绘制'E'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2) ||           // 左侧竖线
                    (i < 2 || i > 11) || // 顶部和底部横线
                    (i > 5 && i < 8))
                { // 中间横线
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'A':
        // 绘制'A'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                // 绘制A的形状
                if ((i + j / 2 < 4 && j < 6 && j > 1) || // 顶部尖角 (左侧)
                    (i - j / 2 < 0 && j > 5 && j < 9) || // 顶部尖角 (右侧)
                    (j < 2 && i > 2 && i < 12) ||        // 左侧竖线
                    (j > 7 && i > 2 && i < 12) ||        // 右侧竖线
                    (i > 6 && i < 9))
                { // 中间横线
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'S':
        // 绘制'S'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((i < 2) ||          // 顶部横线
                    (i > 5 && i < 8) || // 中间横线
                    (i > 11) ||         // 底部横线
                    (j < 2 && i < 7) || // 左侧上半部分
                    (j > 7 && i > 7))
                { // 右侧下半部分
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;
    case 'W':
        // 绘制'W'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                // 绘制W的形状
                if ((j < 2 && i < 12) ||                   // 左侧竖线
                    (j > 7 && i < 12) ||                   // 右侧竖线
                    (j > 1 && j < 4 && i > 7 && i < 12) || // 左下斜线
                    (j > 5 && j < 8 && i > 7 && i < 12) || // 右下斜线
                    (j > 3 && j < 6 && i > 9 && i < 13))
                { // 中间连接部分
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;
    case 'T':
        // 绘制'T'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                // 绘制T的形状
                if ((i < 2) || // 顶部横线
                    (j > 3 && j < 6 && i < 12))
                { // 中间竖线
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'N':
        // 绘制'N'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                // 绘制N的形状
                if ((j < 2 && i < 12) || // 左侧竖线
                    (j > 7 && i < 12) || // 右侧竖线
                    (j > 1 && j < 8 && i == j - 1))
                { // 对角线连接
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'G':
        // 绘制'G'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                // 绘制G的形状
                if ((j < 2 && i > 2 && i < 11) || // 左侧竖线
                    (i < 2 && j > 1 && j < 8) ||  // 顶部横线
                    (i > 11 && j > 1 && j < 8) || // 底部横线
                    (j > 7 && i > 5 && i < 11) || // 右侧竖线(部分)
                    (i > 7 && i < 10 && j > 4 && j < 8))
                { // 中间横线
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'D':
        // 绘制'D'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                // 绘制D的形状
                if ((j < 2 && i < 12) || // 左侧竖线
                    (i < 2 && j < 8) ||  // 顶部横线
                    (i > 11 && j < 8) || // 底部横线
                    (j > 7 && i > 1 && i < 11))
                { // 右侧竖线
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;
    case 'V':
        // 绘制'V'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2 && i < 7) ||          // 左侧上半部分
                    (j > 7 && i < 7) ||          // 右侧上半部分
                    (j > 2 && j < 5 && i > 6) || // 左侧下半部分
                    (j > 5 && j < 8 && i > 6))
                { // 右侧下半部分
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'R':
        // 绘制'R'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2) ||                   // 左侧竖线
                    (i < 2) ||                   // 顶部横线
                    (i > 5 && i < 8) ||          // 中间横线
                    (j > 7 && i < 8 && i > 1) || // 右侧上半部分
                    (j > 5 && j < 8 && i > 8))
                { // 右侧下半部分斜线
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'I':
        // 绘制'I'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j > 3 && j < 6) || // 中间竖线
                    (i < 2 || i > 11))
                { // 顶部和底部横线
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'F':
        // 绘制'F'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2) || // 左侧竖线
                    (i < 2) || // 顶部横线
                    (i > 5 && i < 8))
                { // 中间横线
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'Y':
        // 绘制'Y'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                // 逐行定义Y的形状
                if (i <= 4)
                {
                    if ((i == 0 && (j == 2 || j == 7)) ||
                        (i == 1 && (j == 3 || j == 6)) ||
                        (i == 2 && (j == 4 || j == 5)) ||
                        (i == 3 && (j == 4 || j == 5)) ||
                        (i == 4 && (j == 4 || j == 5)))
                        goto draw_pixel;
                }
                else
                {
                    if (i >= 5 && j >= 4 && j <= 5) // 下半部分竖线
                        goto draw_pixel;
                }
                continue;
            draw_pixel:
                if ((y + i) < 272 && (x + j) < 240)
                {
                    frame_buffer[(y + i) * 240 + (x + j)] = color;
                }
            }
        }
        break;
    case 'C':
        // 绘制'C'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                // 绘制C的形状
                if ((j < 2 && i > 2 && i < 11) || // 左侧竖线
                    (i < 2 && j > 1 && j < 8) ||  // 顶部横线
                    (i > 11 && j > 1 && j < 8))
                { // 底部横线
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;
    case 'U':
        // 绘制'U'字符
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                // 绘制U的形状
                if ((j < 2 && i < 12) || // 左侧竖线
                    (j > 7 && i < 12) || // 右侧竖线
                    (i > 11 && j > 1 && j < 8))
                { // 底部横线
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;
    case ' ':
        // 空格字符，不绘制任何内容
        break;

    default:
        // 对于未定义的字符，绘制一个简单的方框
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if (i < 2 || i > 11 || j < 2 || j > 7)
                {
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;
    }
}

// 绘制字符串函数
void draw_string_on_image(uint16_t x, uint16_t y, char *str, uint16_t color)
{
    uint16_t pos_x = x;
    while (*str && pos_x < 240)
    {
        draw_char_on_image(pos_x, y, *str, color);
        pos_x += 12; // 字符间距
        str++;
    }
}

// 绘制像素点函数
void draw_pixel_on_image(uint16_t x, uint16_t y, uint16_t color)
{
    if (x < 240 && y < 272)
    {
        uint16_t *frame_buffer = (uint16_t *)0xC1000000;
        frame_buffer[y * 240 + x] = color;
    }
}

// 绘制简单文本（示例）
void draw_simple_text(void)
{
    // 绘制红色边框
    for (int i = 0; i < 240; i++)
    {
        draw_pixel_on_image(i, 0, 0xF800);   // 红色顶部边框
        draw_pixel_on_image(i, 271, 0xF800); // 红色底部边框
    }
    for (int i = 0; i < 272; i++)
    {
        draw_pixel_on_image(0, i, 0xF800);   // 红色左边框
        draw_pixel_on_image(239, i, 0xF800); // 红色右边框
    }

    // 绘制简单文本
    draw_string_on_image(10, 20, "PLEASE VERIFY", 0xF800); // 绿色'DM'
}
// 清除指定区域的函数
void clear_area_on_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    uint16_t *frame_buffer = (uint16_t *)0xC1000000;

    // 确保参数在有效范围内
    if (x >= 240 || y >= 272)
        return;
    if (x + width > 240)
        width = 240 - x;
    if (y + height > 272)
        height = 272 - y;

    // 清除指定区域（设置为黑色）
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            frame_buffer[(y + i) * 240 + (x + j)] = 0x0000; // 黑色
        }
    }
}

// 或者使用背景色清除函数
void clear_text_area(void)
{
    // 清除文本显示区域（比如顶部50行）
    clear_area_on_image(0, 0, 240, 50);

    // 重新绘制边框
    for (int i = 0; i < 240; i++)
    {
        draw_pixel_on_image(i, 0, 0xF800);   // 红色顶部边框
        draw_pixel_on_image(i, 271, 0xF800); // 红色底部边框
    }
    for (int i = 0; i < 272; i++)
    {
        draw_pixel_on_image(0, i, 0xF800);   // 红色左边框
        draw_pixel_on_image(239, i, 0xF800); // 红色右边框
    }
}