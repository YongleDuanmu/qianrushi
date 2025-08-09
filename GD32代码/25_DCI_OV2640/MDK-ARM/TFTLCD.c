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
    hex_string[len * 2] = '\0'; // ����ַ���������
}

// �����ַ����� - ֧��Ӣ�Ĵ�д��ĸ�Ϳո�
void draw_char_on_image(uint16_t x, uint16_t y, char ch, uint16_t color)
{
    uint16_t *frame_buffer = (uint16_t *)0xC1000000;

    // ȷ����������Ч��Χ��
    if (x >= 240 || y >= 272)
        return;

    // �����ַ� - 10x14����
    switch (ch)
    {
    case '0':
        // ����'0'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2 && i > 1 && i < 12) || // �������
                    (j > 7 && i > 1 && i < 12) || // �Ҳ�����
                    (i < 2 && j > 1 && j < 8) ||  // ��������
                    (i > 11 && j > 1 && j < 8))   // �ײ�����
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
        // ����'1'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if (j > 3 && j < 6) // �м�����
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
        // ����'2'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((i < 2) ||          // ��������
                    (i > 5 && i < 8) || // �м����
                    (i > 11) ||         // �ײ�����
                    (j > 7 && i < 7) || // �Ҳ��ϰ벿��
                    (j < 2 && i > 7))   // ����°벿��
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
        // ����'3'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((i < 2) ||          // ��������
                    (i > 5 && i < 8) || // �м����
                    (i > 11) ||         // �ײ�����
                    (j > 7 && i < 12))  // �Ҳ�����
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
        // ����'4'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j > 7) ||          // �Ҳ�����
                    (i > 5 && i < 8) || // �м����
                    (j < 2 && i < 6))   // ����ϰ벿��
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
        // ����'5'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((i < 2) ||          // ��������
                    (i > 5 && i < 8) || // �м����
                    (i > 11) ||         // �ײ�����
                    (j < 2 && i < 7) || // ����ϰ벿��
                    (j > 7 && i > 7))   // �Ҳ��°벿��
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
        // ����'6'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2) ||                  // �������
                    (i < 2) ||                  // ��������
                    (i > 5 && i < 8) ||         // �м����
                    (i > 11) ||                 // �ײ�����
                    (j > 7 && i > 7 && i < 12)) // �Ҳ��°벿��
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
        // ����'7'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((i < 2) ||         // ��������
                    (j > 7 && i < 12)) // �Ҳ�б��
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
        // ����'8'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2 && i > 1 && i < 12) || // �������
                    (j > 7 && i > 1 && i < 12) || // �Ҳ�����
                    (i < 2) ||                    // ��������
                    (i > 5 && i < 8) ||           // �м����
                    (i > 11))                     // �ײ�����
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
        // ����'9'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2 && i < 12) || // �������
                    (j > 7) ||           // �Ҳ�����
                    (i < 2) ||           // ��������
                    (i > 5 && i < 8) ||  // �м����
                    (i > 11))            // �ײ�����
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
        // ����'B'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2) ||                                 // �������
                    (i < 2) || (i > 5 && i < 8) || (i > 11) || // ��������
                    (j > 7 && i > 1 && i < 7) ||               // �Ҳ��ϰ벿��
                    (j > 7 && i > 7 && i < 12))                // �Ҳ��°벿��
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
        // ����'H'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2) || (j > 7) || // ������������
                    (i > 5 && i < 8))     // �м����
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
        // ����'J'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if (j > 7 ||                      // �Ҳ�����
                    (i > 11 && j > 1 && j < 8) || // �ײ�����
                    (j < 2 && i > 9 && i < 12))   // ���½Ƕ�����
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
        // ����'K'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if (j < 2 ||                              // �������
                    (j > 2 && j < 5 && i > 4 && i < 9) || // �м���б��
                    (j > 5 && j < 8 && i > 0 && i < 5) || // ����б��
                    (j > 5 && j < 8 && i > 9 && i < 14))  // ����б��
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
        // ����'M'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if (j < 2 || j > 7 ||                     // ������������
                    (j > 1 && j < 4 && i > 0 && i < 5) || // ����б��
                    (j > 5 && j < 8 && i > 0 && i < 5))   // ����б��
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
        // ����'O'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2 && i > 1 && i < 12) || // �������
                    (j > 7 && i > 1 && i < 12) || // �Ҳ�����
                    (i < 2 && j > 1 && j < 8) ||  // ��������
                    (i > 11 && j > 1 && j < 8))   // �ײ�����
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
        // ����'Q'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2 && i > 1 && i < 12) || // �������
                    (j > 7 && i > 1 && i < 12) || // �Ҳ�����
                    (i < 2 && j > 1 && j < 8) ||  // ��������
                    (i > 11 && j > 1 && j < 8) || // �ײ�����
                    (j > 5 && i > 9 && i < 12))   // ���½�б��
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
        // ����'X'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2 && i < 5) ||                   // ����б��
                    (j > 7 && i < 5) ||                   // ����б��
                    (j > 2 && j < 5 && i > 4 && i < 9) || // �м䲿��
                    (j < 2 && i > 9) ||                   // ����б��
                    (j > 7 && i > 9))                     // ����б��
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
        // ����'Z'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if (i < 2 || i > 11 ||               // �����͵ײ�����
                    (j > 2 && j < 8 && i + j == 12)) // �м�б��
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
        // ����'P'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2) ||          // �������
                    (i < 2) ||          // ��������
                    (i > 5 && i < 8) || // �м����
                    (j > 7 && i < 8 && i > 1))
                { // �Ҳ��ϰ벿��
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'L':
        // ����'L'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2) || // �������
                    (i > 11 && j < 8))
                { // �ײ�����
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'E':
        // ����'E'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2) ||           // �������
                    (i < 2 || i > 11) || // �����͵ײ�����
                    (i > 5 && i < 8))
                { // �м����
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'A':
        // ����'A'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                // ����A����״
                if ((i + j / 2 < 4 && j < 6 && j > 1) || // ������� (���)
                    (i - j / 2 < 0 && j > 5 && j < 9) || // ������� (�Ҳ�)
                    (j < 2 && i > 2 && i < 12) ||        // �������
                    (j > 7 && i > 2 && i < 12) ||        // �Ҳ�����
                    (i > 6 && i < 9))
                { // �м����
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'S':
        // ����'S'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((i < 2) ||          // ��������
                    (i > 5 && i < 8) || // �м����
                    (i > 11) ||         // �ײ�����
                    (j < 2 && i < 7) || // ����ϰ벿��
                    (j > 7 && i > 7))
                { // �Ҳ��°벿��
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;
    case 'W':
        // ����'W'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                // ����W����״
                if ((j < 2 && i < 12) ||                   // �������
                    (j > 7 && i < 12) ||                   // �Ҳ�����
                    (j > 1 && j < 4 && i > 7 && i < 12) || // ����б��
                    (j > 5 && j < 8 && i > 7 && i < 12) || // ����б��
                    (j > 3 && j < 6 && i > 9 && i < 13))
                { // �м����Ӳ���
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;
    case 'T':
        // ����'T'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                // ����T����״
                if ((i < 2) || // ��������
                    (j > 3 && j < 6 && i < 12))
                { // �м�����
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'N':
        // ����'N'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                // ����N����״
                if ((j < 2 && i < 12) || // �������
                    (j > 7 && i < 12) || // �Ҳ�����
                    (j > 1 && j < 8 && i == j - 1))
                { // �Խ�������
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'G':
        // ����'G'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                // ����G����״
                if ((j < 2 && i > 2 && i < 11) || // �������
                    (i < 2 && j > 1 && j < 8) ||  // ��������
                    (i > 11 && j > 1 && j < 8) || // �ײ�����
                    (j > 7 && i > 5 && i < 11) || // �Ҳ�����(����)
                    (i > 7 && i < 10 && j > 4 && j < 8))
                { // �м����
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'D':
        // ����'D'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                // ����D����״
                if ((j < 2 && i < 12) || // �������
                    (i < 2 && j < 8) ||  // ��������
                    (i > 11 && j < 8) || // �ײ�����
                    (j > 7 && i > 1 && i < 11))
                { // �Ҳ�����
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;
    case 'V':
        // ����'V'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2 && i < 7) ||          // ����ϰ벿��
                    (j > 7 && i < 7) ||          // �Ҳ��ϰ벿��
                    (j > 2 && j < 5 && i > 6) || // ����°벿��
                    (j > 5 && j < 8 && i > 6))
                { // �Ҳ��°벿��
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'R':
        // ����'R'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2) ||                   // �������
                    (i < 2) ||                   // ��������
                    (i > 5 && i < 8) ||          // �м����
                    (j > 7 && i < 8 && i > 1) || // �Ҳ��ϰ벿��
                    (j > 5 && j < 8 && i > 8))
                { // �Ҳ��°벿��б��
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'I':
        // ����'I'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j > 3 && j < 6) || // �м�����
                    (i < 2 || i > 11))
                { // �����͵ײ�����
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'F':
        // ����'F'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                if ((j < 2) || // �������
                    (i < 2) || // ��������
                    (i > 5 && i < 8))
                { // �м����
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;

    case 'Y':
        // ����'Y'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                // ���ж���Y����״
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
                    if (i >= 5 && j >= 4 && j <= 5) // �°벿������
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
        // ����'C'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                // ����C����״
                if ((j < 2 && i > 2 && i < 11) || // �������
                    (i < 2 && j > 1 && j < 8) ||  // ��������
                    (i > 11 && j > 1 && j < 8))
                { // �ײ�����
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;
    case 'U':
        // ����'U'�ַ�
        for (int i = 0; i < 14 && (y + i) < 272; i++)
        {
            for (int j = 0; j < 10 && (x + j) < 240; j++)
            {
                // ����U����״
                if ((j < 2 && i < 12) || // �������
                    (j > 7 && i < 12) || // �Ҳ�����
                    (i > 11 && j > 1 && j < 8))
                { // �ײ�����
                    if ((y + i) < 272 && (x + j) < 240)
                    {
                        frame_buffer[(y + i) * 240 + (x + j)] = color;
                    }
                }
            }
        }
        break;
    case ' ':
        // �ո��ַ����������κ�����
        break;

    default:
        // ����δ������ַ�������һ���򵥵ķ���
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

// �����ַ�������
void draw_string_on_image(uint16_t x, uint16_t y, char *str, uint16_t color)
{
    uint16_t pos_x = x;
    while (*str && pos_x < 240)
    {
        draw_char_on_image(pos_x, y, *str, color);
        pos_x += 12; // �ַ����
        str++;
    }
}

// �������ص㺯��
void draw_pixel_on_image(uint16_t x, uint16_t y, uint16_t color)
{
    if (x < 240 && y < 272)
    {
        uint16_t *frame_buffer = (uint16_t *)0xC1000000;
        frame_buffer[y * 240 + x] = color;
    }
}

// ���Ƽ��ı���ʾ����
void draw_simple_text(void)
{
    // ���ƺ�ɫ�߿�
    for (int i = 0; i < 240; i++)
    {
        draw_pixel_on_image(i, 0, 0xF800);   // ��ɫ�����߿�
        draw_pixel_on_image(i, 271, 0xF800); // ��ɫ�ײ��߿�
    }
    for (int i = 0; i < 272; i++)
    {
        draw_pixel_on_image(0, i, 0xF800);   // ��ɫ��߿�
        draw_pixel_on_image(239, i, 0xF800); // ��ɫ�ұ߿�
    }

    // ���Ƽ��ı�
    draw_string_on_image(10, 20, "PLEASE VERIFY", 0xF800); // ��ɫ'DM'
}
// ���ָ������ĺ���
void clear_area_on_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    uint16_t *frame_buffer = (uint16_t *)0xC1000000;

    // ȷ����������Ч��Χ��
    if (x >= 240 || y >= 272)
        return;
    if (x + width > 240)
        width = 240 - x;
    if (y + height > 272)
        height = 272 - y;

    // ���ָ����������Ϊ��ɫ��
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            frame_buffer[(y + i) * 240 + (x + j)] = 0x0000; // ��ɫ
        }
    }
}

// ����ʹ�ñ���ɫ�������
void clear_text_area(void)
{
    // ����ı���ʾ���򣨱��綥��50�У�
    clear_area_on_image(0, 0, 240, 50);

    // ���»��Ʊ߿�
    for (int i = 0; i < 240; i++)
    {
        draw_pixel_on_image(i, 0, 0xF800);   // ��ɫ�����߿�
        draw_pixel_on_image(i, 271, 0xF800); // ��ɫ�ײ��߿�
    }
    for (int i = 0; i < 272; i++)
    {
        draw_pixel_on_image(0, i, 0xF800);   // ��ɫ��߿�
        draw_pixel_on_image(239, i, 0xF800); // ��ɫ�ұ߿�
    }
}