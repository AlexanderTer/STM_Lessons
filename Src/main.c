/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "rcc.h"
#include "gpio.h"
#include "timer.h"
#include <stdint.h>
#include "stm32f7xx.h"


int main(void)
{
	init_RCC();
    init_GPIO();
    init_timer8();
    /* Loop forever */
    for(;;)
    {
        for(int i = 0; i < 10000000; i++);
        GPIOD->ODR ^= 1 << 1;
    }
}
