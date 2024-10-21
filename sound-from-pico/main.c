#include <stdio.h>
#include "pico/stdlib.h"   // stdlib 
#include "hardware/irq.h"  // interrupts
#include "hardware/pwm.h"  // pwm 
#include "hardware/sync.h" // wait for interrupt 
#include "hardware/vreg.h"

// Set the audio output GPIO pin
#define AUDIO_PIN 7

// Import the sound file
#include "../data/iwashiro_carpaccio.h"
int wav_position = 0;

#define SAMPLE_RATE 24000
#define PWM_WRAP_COUNT 5208  //  Pico clock 125MHz / sample rate 24000Hz ~= 5208

void pwm_interrupt_handler() {
    pwm_clear_irq(pwm_gpio_to_slice_num(AUDIO_PIN));    

    if (wav_position < (WAV_DATA_LENGTH - 1)) { 
        // set pwm level     
        pwm_set_gpio_level(AUDIO_PIN, (WAV_DATA[wav_position] * PWM_WRAP_COUNT / 255));
        wav_position++;
    } else {
        // Loop music, reset position to start
        wav_position = 0;
    }
}


int main(void) {
    stdio_init_all();

    gpio_set_function(AUDIO_PIN, GPIO_FUNC_PWM);

    int audio_pin_slice = pwm_gpio_to_slice_num(AUDIO_PIN);

    // Setup PWM interrupt to fire when PWM cycle is complete
    pwm_clear_irq(audio_pin_slice);
    pwm_set_irq_enabled(audio_pin_slice, true);
    // set the handle function above
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_interrupt_handler); 
    irq_set_enabled(PWM_IRQ_WRAP, true);

    // Setup PWM for audio output
    pwm_config config = pwm_get_default_config();

    pwm_config_set_clkdiv(&config, 1.0f);
    pwm_config_set_wrap(&config, PWM_WRAP_COUNT); // 24000Hz Sample rate

    pwm_init(audio_pin_slice, &config, true);

    pwm_set_gpio_level(AUDIO_PIN, 0);

    while(1) {
        __wfi(); // Wait for Interrupt
    }
}
