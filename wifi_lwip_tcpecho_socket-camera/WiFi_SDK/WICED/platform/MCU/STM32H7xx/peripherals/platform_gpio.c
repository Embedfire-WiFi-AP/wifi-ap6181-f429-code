/*
 * Copyright 2018, Cypress Semiconductor Corporation or a subsidiary of 
 * Cypress Semiconductor Corporation. All Rights Reserved.
 * 
 * This software, associated documentation and materials ("Software"),
 * is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

/** @file
 * STM32H7xx GPIO implementation
 */
#include "platform_peripheral.h"

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Static Function Declarations
 ******************************************************/

/******************************************************
 *               Variable Definitions
 ******************************************************/

void hal_rcc_gpioa_clock_enable( void )
{
    __HAL_RCC_GPIOA_CLK_ENABLE( );
}

void hal_rcc_gpiob_clock_enable( void )
{
    __HAL_RCC_GPIOB_CLK_ENABLE( );
}

void hal_rcc_gpioc_clock_enable( void )
{
    __HAL_RCC_GPIOC_CLK_ENABLE( );
}

void hal_rcc_gpiod_clock_enable( void )
{
    __HAL_RCC_GPIOD_CLK_ENABLE( );
}

void hal_rcc_gpioe_clock_enable( void )
{
    __HAL_RCC_GPIOE_CLK_ENABLE( );
}

void hal_rcc_gpiof_clock_enable( void )
{
    __HAL_RCC_GPIOF_CLK_ENABLE( );
}

void hal_rcc_gpiog_clock_enable( void )
{
    __HAL_RCC_GPIOG_CLK_ENABLE( );
}

void hal_rcc_gpioh_clock_enable( void )
{
    __HAL_RCC_GPIOH_CLK_ENABLE( );
}

void hal_rcc_gpioi_clock_enable( void )
{
    __HAL_RCC_GPIOI_CLK_ENABLE( );
}

void hal_rcc_gpioj_clock_enable( void )
{
    __HAL_RCC_GPIOJ_CLK_ENABLE( );
}

void hal_rcc_gpiok_clock_enable( void )
{
    __HAL_RCC_GPIOK_CLK_ENABLE( );
}

void hal_rcc_gpioa_clock_disable( void )
{
    __HAL_RCC_GPIOA_CLK_DISABLE( );
}

void hal_rcc_gpiob_clock_disable( void )
{
    __HAL_RCC_GPIOB_CLK_DISABLE( );
}

void hal_rcc_gpioc_clock_disable( void )
{
    __HAL_RCC_GPIOC_CLK_DISABLE( );
}

void hal_rcc_gpiod_clock_disable( void )
{
    __HAL_RCC_GPIOD_CLK_DISABLE( );
}

void hal_rcc_gpioe_clock_disable( void )
{
    __HAL_RCC_GPIOE_CLK_DISABLE( );
}

void hal_rcc_gpiof_clock_disable( void )
{
    __HAL_RCC_GPIOF_CLK_DISABLE( );
}

void hal_rcc_gpiog_clock_disable( void )
{
    __HAL_RCC_GPIOG_CLK_DISABLE( );
}

void hal_rcc_gpioh_clock_disable( void )
{
    __HAL_RCC_GPIOH_CLK_DISABLE( );
}

void hal_rcc_gpioi_clock_disable( void )
{
    __HAL_RCC_GPIOI_CLK_DISABLE( );
}

void hal_rcc_gpioj_clock_disable( void )
{
    __HAL_RCC_GPIOJ_CLK_DISABLE( );
}

void hal_rcc_gpiok_clock_disable( void )
{
    __HAL_RCC_GPIOK_CLK_DISABLE( );
}

/* GPIO peripheral clock enable functions */
const platform_gpio_clock_enable_function_t platform_gpio_clock_enable_function[ NUMBER_OF_GPIO_PORTS ] =
{
    [0]  = hal_rcc_gpioa_clock_enable,
    [1]  = hal_rcc_gpiob_clock_enable,
    [2]  = hal_rcc_gpioc_clock_enable,
    [3]  = hal_rcc_gpiod_clock_enable,
    [4]  = hal_rcc_gpioe_clock_enable,
    [5]  = hal_rcc_gpiof_clock_enable,
    [6]  = hal_rcc_gpiog_clock_enable,
    [7]  = hal_rcc_gpioh_clock_enable,
    [8]  = hal_rcc_gpioi_clock_enable,
    [9]  = hal_rcc_gpioj_clock_enable,
    [10] = hal_rcc_gpiok_clock_enable,
};

/* GPIO peripheral clock disable functions */
const platform_gpio_clock_disable_function_t platform_gpio_clock_disable_function[ NUMBER_OF_GPIO_PORTS ] =
{
    [0]  = hal_rcc_gpioa_clock_disable,
    [1]  = hal_rcc_gpiob_clock_disable,
    [2]  = hal_rcc_gpioc_clock_disable,
    [3]  = hal_rcc_gpiod_clock_disable,
    [4]  = hal_rcc_gpioe_clock_disable,
    [5]  = hal_rcc_gpiof_clock_disable,
    [6]  = hal_rcc_gpiog_clock_disable,
    [7]  = hal_rcc_gpioh_clock_disable,
    [8]  = hal_rcc_gpioi_clock_disable,
    [9]  = hal_rcc_gpioj_clock_disable,
    [10] = hal_rcc_gpiok_clock_disable,
};

/******************************************************
 *               Function Definitions
 ******************************************************/

uint8_t platform_gpio_get_port_number( platform_gpio_port_t * gpio_port )
{
    uint8_t port_index = ( (uint32_t) gpio_port - GPIOA_BASE ) / 0x400;

    if ( ( (uint32_t) gpio_port < GPIOA_BASE ) || ( (uint32_t) gpio_port > GPIOK_BASE ) )
    {
        return INVALID_GPIO_PORT_NUMBER;
    }

    return ( port_index );
}
