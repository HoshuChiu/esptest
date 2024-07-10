#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "i80_drive.h"
#include "lcd_panel.h"

static const char *TAG = "ChiuPad_V1";

void lcd_backlight_init(){
    //初始化背光灯
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << LCD_PANEL_PIN_NUM_BK_LIGHT
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
    lcd_backlight_off();

    //初始化IM1 IM2 LEDK~4
    gpio_config_t im1_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << LCD_PANEL_PIN_IM1
    };
    gpio_config_t im2_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << LCD_PANEL_PIN_IM2
    };
    gpio_config_t l1_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << LCD_PANEL_PIN_LEDK1
    };
    gpio_config_t l2_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << LCD_PANEL_PIN_LEDK2
    };
    gpio_config_t l3_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << LCD_PANEL_PIN_LEDK3
    };
    gpio_config_t l4_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << LCD_PANEL_PIN_LEDK4
    };
    ESP_ERROR_CHECK(gpio_config(&im1_gpio_config));
    ESP_ERROR_CHECK(gpio_config(&im2_gpio_config));
    ESP_ERROR_CHECK(gpio_config(&l1_gpio_config));
    ESP_ERROR_CHECK(gpio_config(&l2_gpio_config));
    ESP_ERROR_CHECK(gpio_config(&l3_gpio_config));
    ESP_ERROR_CHECK(gpio_config(&l4_gpio_config));
    gpio_set_level(LCD_PANEL_PIN_LEDK1, 0);
    gpio_set_level(LCD_PANEL_PIN_LEDK2, 0);
    gpio_set_level(LCD_PANEL_PIN_LEDK3, 0);
    gpio_set_level(LCD_PANEL_PIN_LEDK4, 0);
    gpio_set_level(LCD_PANEL_PIN_IM1, 0);
    gpio_set_level(LCD_PANEL_PIN_IM2, 0);
}

void lcd_backlight_on(){
    ESP_LOGI(TAG, "Turn on LCD backlight");
    gpio_set_level(LCD_PANEL_PIN_NUM_BK_LIGHT, LCD_BK_LIGHT_ON_LEVEL);
}

void lcd_backlight_off(){
    ESP_LOGI(TAG, "Turn off LCD backlight");
    gpio_set_level(LCD_PANEL_PIN_NUM_BK_LIGHT, LCD_BK_LIGHT_OFF_LEVEL);
}



void lcd_panel_init(esp_lcd_panel_io_handle_t io_handle, esp_lcd_panel_handle_t *panel){
    esp_lcd_panel_handle_t panel_handle = NULL;
    ESP_LOGI(TAG, "Install LCD driver of st7789");
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = BUS_I80_PIN_NUM_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = 16,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));
    esp_lcd_panel_reset(panel_handle);
    esp_lcd_panel_init(panel_handle);
    // Set inversion, x/y coordinate order, x/y mirror according to your LCD module spec
    // the gap is LCD panel specific, even panels with the same driver IC, can have different gap value
    esp_lcd_panel_invert_color(panel_handle, true);
    esp_lcd_panel_set_gap(panel_handle, LCD_X_OFFSET, LCD_Y_OFFSET);
    *panel = panel_handle;
}