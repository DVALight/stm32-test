#include "stm32f4xx_hal.h"
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

// ENC chip select
#define ENC_CS_GPIO GPIOA
#define ENC_CS_PIN GPIO_PIN_4

void ENC_Select(uint8_t select)
{
  HAL_GPIO_WritePin(ENC_CS_GPIO, ENC_CS_PIN,
    select ? GPIO_PIN_RESET : GPIO_PIN_SET);
  HAL_Delay(2);
}

#define ENC_Enable() ENC_Select(1)
#define ENC_Disable() ENC_Select(0)

typedef enum {
  ENC_RCR,  // Read Control Register
  ENC_RBM,  // Read Buffer Memory
  ENC_WCR,  // Write Control Register
  ENC_WBM,  // Write Buffer Memory
  ENC_BFS,  // Bit Field Set
  ENC_BFC,  // Bit Field Clear
  ENC_SRC   // System Reset Command
} ENC_Op_t;

#define ENC_ADDR_MASK 0x1F

uint8_t ENC_ReadOp(ENC_Op_t op, uint8_t addr)
{
  uint8_t rx, tx = ((uint8_t)op << 5) | (addr & ENC_ADDR_MASK);
  HAL_SPI_TransmitReceive(&hspi1, &tx, &rx, 1, -1);
  return rx;
}

void ENC_WriteOp(ENC_Op_t op, uint8_t addr, uint8_t data)
{
  uint8_t tx = ((uint8_t)op << 5) | (addr & ENC_ADDR_MASK);
  HAL_SPI_Transmit(&hspi1, &tx, 1, -1);
  HAL_SPI_Transmit(&hspi1, &data, 1, -1);
}

void ENC_ResetCommand()
{
  ENC_Enable();
  ENC_WriteOp(ENC_SRC, 0x1F, 0);
  ENC_Disable();
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  HAL_UART_Init(&huart1);
  printf("HAL_GetTickFreq() -> %d\r\n", HAL_GetTickFreq());
  printf("Hello World! %lu\r\n", HAL_GetTick());

  HAL_SPI_Init(&hspi1);

  HAL_GPIO_WritePin(ENC_CS_GPIO, ENC_CS_PIN, GPIO_PIN_SET);
  //ENC_ResetCommand();

  // ENC_Enable();
  // printf("ERDPTL -> %d\r\n", ENC_ReadOp(ENC_RCR, 0x00));
  // ENC_Disable();

  // printf("ENC done\r\n");
  while (1) {}
}
