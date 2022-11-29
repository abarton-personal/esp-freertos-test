

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
#define SECRET_BLINK_PERIOD 250

static const int led_pin = 17;
static uint8_t s_led_state = 0;

static TaskHandle_t task1;
static TaskHandle_t task2;


/* 
Introduction to RTOS part 3: Task Scheduling
https://www.youtube.com/watch?v=95yUbClyf3E&list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz&index=4

In this example we create 2 tasks that print stuff. A third task will periodically suspend
and resume one of them. Finally it deletes said task. You can see all of that happen via 
its output on the console.

*/



static void configure_led(void)
{
    gpio_reset_pin(led_pin);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(led_pin, GPIO_MODE_OUTPUT);
}


void setUp(void)
{
    /* Configure the peripheral according to the LED type */
    configure_led();

}



void starttask1(void *parameter)
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


void starttask2(void *parameter)
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



void app_main(void)
{

    setUp();

    xTaskCreatePinnedToCore(
        starttask1,
        "blink led",
        1024,
        NULL,
        1,
        &task1,
        app_cpu
    );

    xTaskCreatePinnedToCore(
        starttask2,
        "secret blink led",
        1024,
        NULL,
        1,
        &task2,
        app_cpu
    );


    for(int i=0; i<3; i++){
        vTaskSuspend(task2);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        vTaskResume(task2);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

    if(task2 != NULL){
        vTaskDelete(task2);
        task2 = NULL;
    }
    
    while(1){
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
