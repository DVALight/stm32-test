#include "stm32f4xx_hal.h"

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

int main(void)
{
  HAL_Init();
  
  HAL_UART_Init(&huart1);

  HAL_UART_Transmit(&huart1, (const uint8_t*)"Hello World\r\n", 13, -1);

  while (1) {}
}
