cmake_minimum_required(VERSION 3.16)

# STM32_TOOLCHAIN_PATH, STM32_TARGET_TRIPLET, STM32_CUBE_F4_PATH must be present in environment variables
# check https://github.com/DVALight/stm32-cmake for more info

project(stm32-test C ASM)
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

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

list(APPEND HAL_COMP_LIST STM32F4 CORTEX RCC GPIO UART SPI)
list(APPEND CMSIS_COMP_LIST "STM32F4")

if(NOT STM32_CMAKE_PACKAGES)
    find_package(CMSIS COMPONENTS "${CMSIS_COMP_LIST}" REQUIRED)
    find_package(HAL COMPONENTS "${HAL_COMP_LIST}" REQUIRED)
    set(STM32_CMAKE_PACKAGES 1)
endif()

add_subdirectory(../stm32-enc28j60 stm32-enc28j60)

add_executable(stm32-test ${SOURCES} ${HEADERS})
target_compile_definitions(stm32-test PRIVATE
    REMOTE_MAC_STR="$ENV{LOCAL_MAC}" # we do this to hide sensitive information like MAC address 
)

target_include_directories(stm32-test PRIVATE
    src
)

target_link_libraries(stm32-test
    stm32-enc28j60
    HAL::STM32::F4::CORTEX
    HAL::STM32::F4::RCC
    HAL::STM32::F4::GPIO
    HAL::STM32::F4::UART
    HAL::STM32::F4::SPI
    CMSIS::STM32::F401VE
    STM32::NoSys
)

add_custom_command(TARGET stm32-test POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex stm32-test.elf stm32-test.hex
)

stm32_print_size_of_target(stm32-test)