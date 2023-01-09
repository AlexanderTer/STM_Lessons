
#include "stm32f7xx.h"
#include "crc.h"

void init_CRC(void)
{
    // Вкл. тактирование модуля CRC.
    RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;

    // Битность алгоритма (полинома) = 16 бит.
    CRC->CR |= CRC_CR_POLYSIZE_0;

    // Инверсия выходных данных.
    CRC->CR |= CRC_CR_REV_OUT;

    // Инверсия входных данных по байтам.
    CRC->CR |= CRC_CR_REV_IN_0;

    // Начальное значение CRC.
    CRC->INIT = 0xFFFF;

    // Коэффициенты полинома.
    CRC->POL = 0x8005;
}

/**
 * \brief   Расчёт CRC-16.
 *
 * \param   data: указатель на данные.
 * \param   size: кол-во данных.
 *
 * \return  crc: значение CRC.
 *
 */

uint16_t calc_CRC16(uint8_t *data, uint32_t size)
{
    // Сброс CRC.
    CRC->CR |= CRC_CR_RESET;

    // Расчёт CRC.
    for (int i = 0; i < size; i++)
        *(__IO uint8_t *)&CRC->DR = data[i];

    return CRC->DR;
}
