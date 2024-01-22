#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "i2c-lcd.h"
#include "unistd.h"
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#define LED 2
#define BOTAO 12

QueueHandle_t xFila;
TaskHandle_t xTask1;

static const char *TAG = "LCD_botao_led";

char buffer[50];
static uint8_t state_led = 0;
static uint8_t state_button = 0;
static int contador = 0;


static void IRAM_ATTR int_bt(void *args);

void configure_led();
void configure_button();
void interrupt_button();

static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_NUM_0;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 1000000,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, I2C_MODE_MASTER, 0, 0, 0);
}


void app_main(void)
{
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C inicializado com sucesso");

    xFila = xQueueCreate(50, sizeof(int));
    xTaskCreate(interrupt_button, "interrupt_button", configMINIMAL_STACK_SIZE + 1024, NULL,1, &xTask1);

    configure_button();
    configure_led();

    lcd_init();
    lcd_clear();
    lcd_put_cur(0, 0);
    lcd_send_string("LED: ");
    lcd_put_cur(0, 5);
    lcd_send_string("Desligado");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void configure_led(){
    gpio_reset_pin(LED);
    gpio_set_direction(LED,GPIO_MODE_OUTPUT);
}

void configure_button(){
    gpio_reset_pin(BOTAO);
    gpio_set_direction(BOTAO, GPIO_MODE_INPUT);
    gpio_pulldown_en(BOTAO);
    gpio_pullup_dis(BOTAO);

    // Configurando o botao para interrupção
    gpio_set_intr_type(BOTAO, GPIO_INTR_POSEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BOTAO, int_bt, (void *) BOTAO);
}

static void IRAM_ATTR int_bt(void *args){
    int pino = (int) args;
    xQueueSendFromISR(xFila, &pino, NULL);
     
}

void interrupt_button(void *params){
    int pino;

    while (1)
    {
        if (xQueueReceive(xFila, &pino, portMAX_DELAY))
        {
            // De-bouncing
            int estado = gpio_get_level(pino);

            if (estado == 1)
            {
                
                gpio_isr_handler_remove(BOTAO);
                while (gpio_get_level(pino) == estado)
                {
                    vTaskDelay(pdMS_TO_TICKS(50));
                }

                state_led = !state_led;

                gpio_set_level(LED, state_led);
                lcd_put_cur(0, 5);
                if (state_led == 0)
                {
                    lcd_send_string("Desligado");
                }else
                {
                    lcd_send_string("Ligado");
                    lcd_put_cur(0, 11);
                    lcd_send_data(0x20);
                    lcd_put_cur(0, 12);
                    lcd_send_data(0x20);
                    lcd_put_cur(0, 13);
                    lcd_send_data(0x20);
                }

                vTaskDelay(pdMS_TO_TICKS(50));
                gpio_isr_handler_add(BOTAO, int_bt, (void *) BOTAO);
            }        
        }
    }
}
