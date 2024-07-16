//Emma Krompascikova, xkromp00, IMS projekt 2021/2022
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"

//uart
#include "driver/uart.h"
#include "sdkconfig.h"
#include "esp_log.h"

#define ECHO_TEST_TXD 1
#define ECHO_TEST_RXD 3
#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

#define ECHO_UART_PORT_NUM      2
#define ECHO_UART_BAUD_RATE     115200
#define ECHO_TASK_STACK_SIZE    2048

#define BUF_SIZE (1024)

// pripoji reproduktor na GND a GPIO14
#define SPK_GPIO_NUM 14

#define LEDC_TIMER_NUM  LEDC_TIMER_0
#define LEDC_TIMER_MODE LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL    LEDC_CHANNEL_0
#define LEDC_DUTY_RES   LEDC_TIMER_4_BIT // duty resolution 4 bits
#define LEDC_DUTY       15 // Set duty to 50%. ((2 ** 4) - 1) * 50% = 15

//frekvencie not
//stvrta oktava
const float c4 = 261.63;
const float d4 = 293.66;
const float e4 = 329.63;
const float f4 = 349.23;
const float g4 = 392.00;
const float gs4 = 415.30; // g# pre fur elise
const float a4 = 440.00;
const float h4 = 493.88;
//piata oktava
const float c5 = 523.25;
const float d5 = 587.33;
const float ds5 = 622.25; //d5# pre fur elise
const float e5 = 659.25;
const float f5 = 698.46;
const float g5 = 783.99;
const float a5 = 880.00;
const float h5 = 987.77;
//siesta oktava
const float c6 = 1046.50;
const float d6 = 1174.66;
const float e6 = 1318.51;
const float f6 = 1396.91;
const float g6 = 1567.98;
const float a6 = 1760.00;
const float h6 = 1975.53;
//dlzky not v milisekundach
int cela = 4000;
int polova = 2000;
int stvrtova = 1000;
int osminova_bodka = 750; //osminová nota + sesnastinova nota
int osminova = 500;
int sesnastinova = 250;

static void zahrat_notu(float nota)
{  // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_TIMER_MODE,
        .timer_num        = LEDC_TIMER_NUM,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = (int)nota,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_TIMER_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER_NUM,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = SPK_GPIO_NUM,
        .duty           = 0, //strida nastavena na nulu
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    // Set the PWM duty to 50% according to the parameters defined above
    // (here we want to generate ordinary square wave for driving the speaker)
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_TIMER_MODE, LEDC_CHANNEL, LEDC_DUTY));
}

static void dlzka(int dlzka){ //dlzka v akej na byt odohrana nota v ms
    vTaskDelay(dlzka / portTICK_RATE_MS);  
    //vypnutie repraku na kratky cas aby nedoslo k prelinaniu
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_TIMER_MODE, LEDC_CHANNEL, 0));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_TIMER_MODE, LEDC_CHANNEL));
    vTaskDelay(25 / portTICK_RATE_MS);
}

static void fur_elise(){
    zahrat_notu(e5);
    dlzka(sesnastinova);
    zahrat_notu(ds5);
    dlzka(sesnastinova);
    zahrat_notu(e5);
    dlzka(sesnastinova);
    zahrat_notu(ds5);
    dlzka(sesnastinova);
    zahrat_notu(e5);
    dlzka(sesnastinova);
    zahrat_notu(h4);
    dlzka(sesnastinova);
    zahrat_notu(d5);
    dlzka(sesnastinova);
    zahrat_notu(c5);
    dlzka(sesnastinova);
    zahrat_notu(a4);
    dlzka(osminova_bodka);
    zahrat_notu(c4);
    dlzka(sesnastinova);
    zahrat_notu(e4);
    dlzka(sesnastinova);
    zahrat_notu(a4);
    dlzka(sesnastinova);
    zahrat_notu(h4);
    dlzka(osminova_bodka);
    zahrat_notu(e4);
    dlzka(sesnastinova);
    zahrat_notu(gs4);
    dlzka(sesnastinova);
    zahrat_notu(h4);
    dlzka(sesnastinova);
    zahrat_notu(c5);
    dlzka(osminova_bodka);
    zahrat_notu(e4);
    dlzka(osminova);

    zahrat_notu(e5);
    dlzka(sesnastinova);
    zahrat_notu(ds5);
    dlzka(sesnastinova);
    zahrat_notu(e5);
    dlzka(sesnastinova);
    zahrat_notu(ds5);
    dlzka(sesnastinova);
    zahrat_notu(e5);
    dlzka(sesnastinova);
    zahrat_notu(h4);
    dlzka(sesnastinova);
    zahrat_notu(d5);
    dlzka(sesnastinova);
    zahrat_notu(c5);
    dlzka(sesnastinova);
    zahrat_notu(a4);
    dlzka(osminova_bodka);
    zahrat_notu(c4);
    dlzka(sesnastinova);
    zahrat_notu(e4);
    dlzka(sesnastinova);
    zahrat_notu(a4);
    dlzka(sesnastinova);
    zahrat_notu(h4);
    dlzka(osminova_bodka);
    zahrat_notu(e4);
    dlzka(sesnastinova);
    zahrat_notu(c5);
    dlzka(sesnastinova);
    zahrat_notu(h4);
    dlzka(sesnastinova);
    zahrat_notu(a4);
    dlzka(osminova_bodka);

    zahrat_notu(h4);
    dlzka(sesnastinova);
    zahrat_notu(c5);
    dlzka(sesnastinova);
    zahrat_notu(d5);
    dlzka(sesnastinova);
    zahrat_notu(e5);
    dlzka(osminova_bodka);
    zahrat_notu(g4);
    dlzka(sesnastinova);
    zahrat_notu(f5);
    dlzka(sesnastinova);
    zahrat_notu(e5);
    dlzka(sesnastinova);
    zahrat_notu(d5);
    dlzka(osminova_bodka);
    zahrat_notu(f4);
    dlzka(sesnastinova);
    zahrat_notu(e5);
    dlzka(sesnastinova);
    zahrat_notu(d5);
    dlzka(sesnastinova);
    zahrat_notu(c5);
    dlzka(osminova_bodka);
    zahrat_notu(e4);
    dlzka(sesnastinova);
    zahrat_notu(d5);
    dlzka(sesnastinova);
    zahrat_notu(c5);
    dlzka(sesnastinova);
    zahrat_notu(h4);
    dlzka(osminova_bodka);
    zahrat_notu(e4);
    dlzka(osminova_bodka);
}

static void echo_task(void *arg)
{
    //skonfigurovanie parametrov pre UART driver a instalacia driveru
    uart_config_t uart_config = {
        .baud_rate = ECHO_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    int intr_alloc_flags = 0;
    
    ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));

    // konfiguacia doasneho bufferu pre prichadzajuce data
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);

    while (1) {
        // citanie dat z UART
        int len = uart_read_bytes(ECHO_UART_PORT_NUM, data, (BUF_SIZE - 1), 20 / portTICK_RATE_MS);
        
        if (len) {    
            if(strcmp((char*)data, "c") == 0){
                zahrat_notu(c4);
                dlzka(osminova);
            }
            else if (strcmp((char*)data, "C") == 0){
                zahrat_notu(c5);
                dlzka(osminova);
            }
            else if (strcmp((char*)data, "d") == 0){
                zahrat_notu(d4);
                dlzka(osminova);
            }
            else if (strcmp((char*)data, "D") == 0){
                zahrat_notu(d5);
                dlzka(osminova);
            }
            else if (strcmp((char*)data, "e") == 0){
                zahrat_notu(e4);
                dlzka(osminova);
            }
            else if (strcmp((char*)data, "E") == 0){
                zahrat_notu(e5);
                dlzka(osminova);
            }
            else if (strcmp((char*)data, "f") == 0){
                zahrat_notu(f4);
                dlzka(osminova);
            }
            else if (strcmp((char*)data, "F") == 0){
                zahrat_notu(f5);
                dlzka(osminova);
            }
            else if (strcmp((char*)data, "g") == 0){
                zahrat_notu(g4);
                dlzka(osminova);
            }
            else if (strcmp((char*)data, "G") == 0){
                zahrat_notu(g5);
                dlzka(osminova);
            }
            else if (strcmp((char*)data, "a") == 0){
                zahrat_notu(a4);
                dlzka(osminova);
            }
            else if (strcmp((char*)data, "A") == 0){
                zahrat_notu(a5);
                dlzka(osminova);
            }
            else if (strcmp((char*)data, "h") == 0){
                zahrat_notu(h4);
                dlzka(osminova);
            }
            else if (strcmp((char*)data, "H") == 0){
                zahrat_notu(h5);
                dlzka(osminova);
            }
            else if (strcmp((char*)data, "p") == 0){
                fur_elise();
            }
        data[len] = '\0';
        }
    }
}

void app_main(void)
{   
    xTaskCreate(echo_task, "uart_echo_task", ECHO_TASK_STACK_SIZE, NULL, 10, NULL);
    
}