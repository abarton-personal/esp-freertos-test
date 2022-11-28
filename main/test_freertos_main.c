

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "stdio.h"
#include "sdkconfig.h"
#include "driver/gpio.h"

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#define BLINK_PERIOD 1000
#define SECRET_BLINK_PERIOD 300

static const int led_pin = 17;
static uint8_t s_led_state = 0;


static void configure_led(void)
{
    gpio_reset_pin(led_pin);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(led_pin, GPIO_MODE_OUTPUT);
}


void blink_led(void *parameter)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/    
    while (1) {
        printf("balls\n");
        gpio_set_level(led_pin, s_led_state);
        /* Toggle the LED state */
        s_led_state = !s_led_state;
        vTaskDelay(BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}


void secret_blink_led(void *parameter)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/    
    while (1) {
        printf("nuts\n");
        gpio_set_level(led_pin, s_led_state);
        /* Toggle the LED state */
        s_led_state = !s_led_state;
        vTaskDelay(SECRET_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}


void setUp(void)
{
    /* Configure the peripheral according to the LED type */
    configure_led();


}


void app_main(void)
{

    setUp();

    xTaskCreatePinnedToCore(
        blink_led,
        "blink led",
        1024,
        NULL,
        1,
        NULL,
        app_cpu
    );

    xTaskCreatePinnedToCore(
        secret_blink_led,
        "secret blink led",
        1024,
        NULL,
        1,
        NULL,
        app_cpu
    );

}
