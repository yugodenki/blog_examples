#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/spi.h"
#include "st7789_lcd.pio.h"

#define SCREEN_HEIGHT 280
#define SCREEN_WIDTH 240

#define PIN_DIN 7
#define PIN_CLK 6
#define PIN_CS 5
#define PIN_DC 4
#define PIN_RESET 3
#define PIN_BL 2

#define SERIAL_CLK_DIV 1.f


void lcd_set_dc_cs(bool dc, bool cs);
void lcd_send_command(uint8_t command, size_t data_len, const char *data, uint32_t delay_ms);
void lcd_setup(void);
void setup_pio_for_lcd(PIO pio, uint sm);
int setup_dma_lcd(PIO pio, uint sm);
void setup_hardware(int *dma_lcd_ch);
