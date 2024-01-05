cmake_minimum_required(VERSION 3.16)

# STM32_TOOLCHAIN_PATH, STM32_TARGET_TRIPLET, STM32_CUBE_F4_PATH must be present in environment variables
# check https://github.com/DVALight/stm32-cmake for more info

project(stm32-base C ASM)
set(SOURCES
    src/core/syscalls.c
    src/core/sysmem.c
    src/hal_isr.c
    src/hal_msp.c
    src/main.c
)

set(HEADERS
    src/stm32f4xx_hal_conf.h
    src/hal_isr.h
)

include_directories(src src/core)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

#set(HAL_COMP_LIST RCC GPIO CORTEX)
set(HAL_COMP_LIST )
set(CMSIS_COMP_LIST "")

list(APPEND CMSIS_COMP_LIST STM32F4)
list(APPEND HAL_COMP_LIST STM32F4)

find_package(CMSIS COMPONENTS "${CMSIS_COMP_LIST}" REQUIRED)
find_package(HAL COMPONENTS "${HAL_COMP_LIST}" REQUIRED)

add_executable(stm32-base ${SOURCES} ${HEADERS})
target_link_libraries(stm32-base
#    HAL::STM32::F4::RCC
#    HAL::STM32::F4::GPIO
    HAL::STM32::F4::CORTEX
    CMSIS::STM32::F401VE
    STM32::NoSys
)

add_custom_command(TARGET stm32-base POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex stm32-base.elf stm32-base.hex
)

stm32_print_size_of_target(stm32-base)