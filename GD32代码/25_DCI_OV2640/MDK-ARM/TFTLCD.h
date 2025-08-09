#ifndef _TFTLCD_H_
#define _TFTLCD_H_
#include "gd32f5xx.h"

void draw_char_on_image(uint16_t x, uint16_t y, char ch, uint16_t color);
void draw_string_on_image(uint16_t x, uint16_t y, char *str, uint16_t color);
void draw_pixel_on_image(uint16_t x, uint16_t y, uint16_t color);
void draw_simple_text(void);
void clear_area_on_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void clear_text_area(void);
void byte_array_to_hex_string(unsigned char* byte_array, int len, char* hex_string);
#endif
