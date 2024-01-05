#include "stm32f4xx_hal.h"
#include <stdio.h>

UART_HandleTypeDef huart1 = {
  .Instance = USART1,
  .Init.BaudRate = 115200,
  .Init.WordLength = UART_WORDLENGTH_8B,
  .Init.StopBits = UART_STOPBITS_1,
  .Init.Parity = UART_PARITY_NONE,
  .Init.Mode = UART_MODE_TX_RX,
  .Init.HwFlowCtl = UART_HWCONTROL_NONE,
  .Init.OverSampling = UART_OVERSAMPLING_16
};

// stdout/stdin syscalls for UART
int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart1, (const uint8_t*)&ch, 1, -1);
  return ch;
}

int __io_getchar(void)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&huart1, &ch, 1, -1);
  return ch;
}

int main(void)
{
  HAL_Init();
  HAL_UART_Init(&huart1);

  printf("Hello World! %d\r\n", 69);

  while (1) {}
}
