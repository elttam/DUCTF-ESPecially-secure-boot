#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#define READ_OFFSET 0x133370
#define BUFFER_SIZE 64  // Adjust size as needed

void app_main(void)
{
    uint8_t buffer[BUFFER_SIZE];
    esp_err_t ret;

    // Initialize buffer to zero
    memset(buffer, 0, BUFFER_SIZE);

    // Read data from SPI flash
    ret = spi_flash_read(READ_OFFSET, buffer, BUFFER_SIZE);
    if (ret != ESP_OK) {
        printf("Error reading SPI flash: %s\n", esp_err_to_name(ret));
    } else {
        printf("Data read from SPI flash: %s\n", buffer);
    }

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
