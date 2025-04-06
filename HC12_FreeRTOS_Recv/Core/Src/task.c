#include <stdio.h>
#include <string.h>
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "main.h"

QueueHandle_t uartQueueHandle; // FreeRTOS용 큐 핸들

extern UART_HandleTypeDef husart3;

void UARTTask(void *argument);
void HC12ComSERIALTask(void *argument);

void systemTaskInit(void)
{
  uartQueueHandle = xQueueCreate(10, sizeof(uint32_t));
  if (uartQueueHandle == NULL)
  {
    Error_Handler();
  }

  // UARTTask 생성
  xTaskCreate(UARTTask,
              "UARTTask",
              128,
              NULL,
              tskIDLE_PRIORITY + 1,
              NULL);

  // HC12ComSERIALTask 생성
  xTaskCreate(HC12ComSERIALTask,
              "HC12ComSERIALTask",
              128,
              NULL,
              tskIDLE_PRIORITY + 1,
              NULL);
}

void UARTTask(void *argument)
{
    uint32_t msg;
    char txBuffer[32];

    for (;;)
    {
        if (xQueueReceive(uartQueueHandle, &msg, portMAX_DELAY) == pdTRUE)
        {
            sprintf(txBuffer, "Received: %lu\r\n", msg);
            HAL_UART_Transmit(&husart3, (uint8_t *)txBuffer, strlen(txBuffer), HAL_MAX_DELAY);
        }
    }
}

void HC12ComSERIALTask(void *argument)
{
    for (;;)
    {
        printf("HC-12 TASK\r\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
