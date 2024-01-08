#include "stm32f4xx_hal.h"
#include "enc28j60.h"
#include <stdio.h>

// RCC

void SystemClock_Config(void)
{
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  HAL_RCC_OscConfig(&(RCC_OscInitTypeDef) {
    .OscillatorType = RCC_OSCILLATORTYPE_HSI,
    .HSIState = RCC_HSI_ON,
    .HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT,
    .PLL.PLLState = RCC_PLL_NONE
  });

  HAL_RCC_ClockConfig(&(RCC_ClkInitTypeDef) {
    .ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
    | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2,
    .SYSCLKSource = RCC_SYSCLKSOURCE_HSI,
    .AHBCLKDivider = RCC_SYSCLK_DIV1,
    .APB1CLKDivider = RCC_HCLK_DIV1,
    .APB2CLKDivider = RCC_HCLK_DIV1
  }, FLASH_LATENCY_0);
}

// UART

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

// SPI

SPI_HandleTypeDef hspi1 = {
  .Instance = SPI1,
  .Init.Mode = SPI_MODE_MASTER,
  .Init.Direction = SPI_DIRECTION_2LINES,
  .Init.DataSize = SPI_DATASIZE_8BIT,
  .Init.CLKPolarity = SPI_POLARITY_LOW,
  .Init.CLKPhase = SPI_PHASE_1EDGE,
  .Init.NSS = SPI_NSS_SOFT,
  .Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64,
  .Init.FirstBit = SPI_FIRSTBIT_MSB,
  .Init.TIMode = SPI_TIMODE_DISABLE,
  // .Init.CRCCalculation = SPI_CRCCALCULATION_ENABLE,
  // .Init.CRCPolynomial = 7
  .Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE
};

static void parse_mac(uint8_t* mac, const char* macStr)
{
  sscanf(macStr, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
    &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
}

int main(void)
{
  HAL_Init();
  //SystemClock_Config();

  HAL_UART_Init(&huart1);
  printf("HAL_GetTickFreq() -> %d\r\n", HAL_GetTickFreq());
  printf("Hello World! %lu\r\n", HAL_GetTick());

  HAL_GPIO_Init(GPIOA, &(GPIO_InitTypeDef) {
    .Pin = ETHERNET_CS_PIN | ETHERNET_LED_PIN,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Speed = GPIO_SPEED_LOW
  });

  HAL_SPI_Init(&hspi1);

  printf("REMOTE_MAC_STR: %s\r\n", REMOTE_MAC_STR);

  uint8_t remote_mac[6] = {0};
  parse_mac(remote_mac, REMOTE_MAC_STR);
  printf("%02x:%02x:%02x:%02x:%02x:%02x\r\n",
    remote_mac[0], remote_mac[1], remote_mac[2], remote_mac[3], remote_mac[4], remote_mac[5]);

  while (1) {}
}
