/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * Modifications made by yugodenki, 2024
 */
 

#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/gpio.h"

#include "pico_hardware_setup.h"
#include "st7789_lcd.pio.h"

#include "../data/280x240images/frame00_rgb444.h"
#include "../data/280x240images/frame01_rgb444.h"
#include "../data/280x240images/frame02_rgb444.h"
#include "../data/280x240images/frame03_rgb444.h"
#include "../data/280x240images/frame04_rgb444.h"

int dma_lcd_ch;

void send_frame_to_display(int dma_lcd_ch, const uint16_t *frame) {
    dma_channel_wait_for_finish_blocking(dma_lcd_ch);
    dma_channel_set_read_addr(dma_lcd_ch, frame, true);
}

int main() {
    stdio_init_all();

    setup_hardware(&dma_lcd_ch);

    const uint16_t *frame;
    int frame_selector = 0;

    while (1) {
        switch (frame_selector) {
            case 0:
            default: 
                frame = frame00;
                break;
            case 1:
            case 7:
                frame = frame01;
                break;
            case 2:
            case 6:
                frame = frame02;
                break;
            case 3:
            case 5:
                frame = frame03;
                break;
            case 4:
                frame = frame04;
                break;
        }
        if (frame_selector == 7) {
            frame_selector = 0;
        } else {
            frame_selector++;
        }

        absolute_time_t lcd_write_start = get_absolute_time();

        send_frame_to_display(dma_lcd_ch, frame);

        absolute_time_t lcd_write_end = get_absolute_time();
        int64_t lcd_write_time = absolute_time_diff_us(lcd_write_start, lcd_write_end);
        printf("%lld us\n", lcd_write_time);
    }
}
