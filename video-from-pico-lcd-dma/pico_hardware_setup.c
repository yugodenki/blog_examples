#include "pico_hardware_setup.h"


void lcd_set_dc_cs(bool dc, bool cs) {
    sleep_us(1);
    gpio_put_masked((1u << PIN_DC) | (1u << PIN_CS), !!dc << PIN_DC | !!cs << PIN_CS);
    sleep_us(1);
}

void lcd_send_command(uint8_t command, size_t data_len, const char *data, uint32_t delay_ms) {
    lcd_set_dc_cs(false, false);
    spi_write_blocking(spi0, &command, 1);
    if (data_len) {
        lcd_set_dc_cs(true, false);
        spi_write_blocking(spi0, data, data_len);
    }
    lcd_set_dc_cs(true, true);
    if (delay_ms) sleep_ms(delay_ms);
}

// LCD setup does not use PIO but only uses SPI
void lcd_setup(void) {
    spi_init(spi0, 8000000);

    // Reset cycle the screen
    gpio_set_function(PIN_RESET, GPIO_FUNC_SIO);
    gpio_set_dir(PIN_RESET, GPIO_OUT);
    gpio_put(PIN_RESET, 0);
    sleep_ms(100);
    gpio_put(PIN_RESET, 1);

    gpio_set_function(PIN_DC, GPIO_FUNC_SIO);
    gpio_set_dir(PIN_DC, GPIO_OUT);
    gpio_set_function(PIN_CS, GPIO_FUNC_SIO);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_set_function(PIN_CLK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_DIN, GPIO_FUNC_SPI);

    // Set CS pin to command mode to setup st7789
    lcd_set_dc_cs(0, 1);

    lcd_send_command(0x01, 0, NULL, 100);  // Software reset
    lcd_send_command(0x11, 0, NULL, 50);  // Exit sleep mode
    lcd_send_command(0x3A, 1, "\x53", 10);  // Set color mode to 12 bits
    lcd_send_command(0x36, 1, "\x00", 0);  // Set MADCTL
    lcd_send_command(0x2A, 4, "\x00\x00\x00\xF0", 0);  // CASET: column addresses, set 0 to 240
    lcd_send_command(0x2B, 4, "\x00\x14\x01\x2B", 0);  // RASET: row addresses, set 20 to 299 (280 pixels)
    lcd_send_command(0x21, 0, NULL, 10);  // Inversion on, then 10ms delay
    lcd_send_command(0x13, 0, NULL, 10);  // Normal display on, then 10ms delay
    lcd_send_command(0x29, 0, NULL, 10);  // Main screen on, then wait 500ms

    // Put LCD into data mode once, and keep updating LCD with data
    // LCD will be in sync as long as we feed it the whole screen worth
    // of pixel data every time
    lcd_send_command(0x2C, 0, NULL, 0);  // Turn Data mode on and feed data forever
}

void setup_pio_for_lcd(PIO pio, uint sm) {
  uint offset = pio_add_program(pio, &st7789_lcd_program);
  st7789_lcd_program_init(pio, sm, offset, PIN_DIN, PIN_CLK, SERIAL_CLK_DIV);
}

// Setup DMA for transferring frame data to LCD
int setup_dma_lcd(PIO pio, uint sm) {
  // Using 16 bit DMA size for simplicity
  int dma_lcd_ch = dma_claim_unused_channel(true);
  dma_channel_config dma_lcd_ch_config = dma_channel_get_default_config(dma_lcd_ch);
  channel_config_set_transfer_data_size(&dma_lcd_ch_config, DMA_SIZE_16);
  channel_config_set_read_increment(&dma_lcd_ch_config, true);
  channel_config_set_write_increment(&dma_lcd_ch_config, false);
  channel_config_set_dreq(&dma_lcd_ch_config, pio_get_dreq(pio, sm, true));
  dma_channel_configure(
      dma_lcd_ch,
      &dma_lcd_ch_config,
      &pio->txf[sm],
      NULL,  // To be set later
      SCREEN_HEIGHT * SCREEN_WIDTH,
      false
  );
  return dma_lcd_ch;
}

void setup_hardware(int *dma_lcd_ch) {
  lcd_setup();

  PIO pio = pio0;
  uint sm = 0;
  setup_pio_for_lcd(pio, sm);

  *dma_lcd_ch = setup_dma_lcd(pio, sm);

  // Turn on data mode for the screen here
  lcd_set_dc_cs(true, false);
}