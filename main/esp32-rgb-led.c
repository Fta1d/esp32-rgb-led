#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_intr_alloc.h"

// Нові GPIO
#define RED_PIN   18
#define GREEN_PIN 19
#define BLUE_PIN  23

#define LEDC_TIMER          LEDC_TIMER_0
#define LEDC_MODE           LEDC_HIGH_SPEED_MODE
#define LEDC_CHANNEL_RED    LEDC_CHANNEL_0
#define LEDC_CHANNEL_GREEN  LEDC_CHANNEL_1
#define LEDC_CHANNEL_BLUE   LEDC_CHANNEL_2
#define LEDC_TIMER_RES      LEDC_TIMER_8_BIT
#define LEDC_FREQUENCY      1000 // Частота 1 кГц

void configure_ledc() {
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_TIMER_RES,
        .freq_hz          = LEDC_FREQUENCY,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel[] = {
        {
            .channel    = LEDC_CHANNEL_RED,
            .duty       = 0,
            .gpio_num   = RED_PIN,
            .speed_mode = LEDC_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_TIMER
        },
        {
            .channel    = LEDC_CHANNEL_GREEN,
            .duty       = 0,
            .gpio_num   = GREEN_PIN,
            .speed_mode = LEDC_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_TIMER
        },
        {
            .channel    = LEDC_CHANNEL_BLUE,
            .duty       = 0,
            .gpio_num   = BLUE_PIN,
            .speed_mode = LEDC_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_TIMER
        }
    };

    for (int ch = 0; ch < 3; ch++) {
        ledc_channel_config(&ledc_channel[ch]);
    }
}

void set_color(uint8_t red, uint8_t green, uint8_t blue) {
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_RED, red);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_RED);

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_GREEN, green);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_GREEN);

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_BLUE, blue);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_BLUE);
}

void app_main(void) {
    // gpio_reset_pin(RED_PIN);
    configure_ledc();

    while (1) {
        ledc_fade_func_install(0);
        ledc_set_fade_with_step(LEDC_MODE, LEDC_CHANNEL_RED, 100, 5, 50);
        ledc_fade_start(LEDC_MODE, LEDC_CHANNEL_RED, LEDC_FADE_WAIT_DONE);


        ledc_fade_func_install(0);
        ledc_set_fade_with_step(LEDC_MODE, LEDC_CHANNEL_RED, 0, 5, 50);
        ledc_fade_start(LEDC_MODE, LEDC_CHANNEL_RED, LEDC_FADE_NO_WAIT);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

}

