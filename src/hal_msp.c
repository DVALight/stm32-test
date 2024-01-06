// HAL MSP - hardware initialization callbacks

#include "stm32f4xx_hal.h"

void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_RCC_GPIOA_CLK_ENABLE(); // used by USART1 and SPI1
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  if (huart->Instance == USART1)
  {
    __HAL_RCC_USART1_CLK_ENABLE();

    // PA9 - TX, PA10 - RX
    HAL_GPIO_Init(GPIOA, &(GPIO_InitTypeDef) {
      .Pin = GPIO_PIN_9 | GPIO_PIN_10,
      .Mode = GPIO_MODE_AF_PP,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
      .Alternate = GPIO_AF7_USART1
    });
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if (huart->Instance == USART1)
  {
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
  }
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  if (hspi->Instance == SPI1)
  {
    __HAL_RCC_SPI1_CLK_ENABLE();
    
    HAL_GPIO_Init(GPIOA, &(GPIO_InitTypeDef) {
      .Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
      .Mode = GPIO_MODE_AF_PP,
      .Pull = GPIO_NOPULL,
      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
      .Alternate = GPIO_AF5_SPI1
    });
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
  if (hspi->Instance == SPI1)
  {
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
  }
}
