#ifndef  __CONTROL_H__
#define __CONTROL_H__

#include "dsp.h"
#include "kiss_fftr.h"
#define SET_SHIFTS_MAX_COUNT ((unsigned int)(0.5 * 100.e3))

#define IL_REF1 (2.f)
#define IL_REF2 (3.f)

#define FFT_SIZE (1000) // Количество точек на периоде расчёта FFT

typedef struct {

	float duty; // Коэффициент заполнения, [0..1]
	PID_Controller_Struct pid_current;// Структура регулятора тока реактора
	PID_Controller_Struct pid_voltage;// Структура регулятора напряжения

} Control_Struct; // Структура с параметрами системы кправления

typedef struct {

	unsigned int count; // 0 - режим измерения(не происходит автоопределения смещения)
						// >0 - режим автоопределения смещения

	struct {
		float inj; 			// Сигнал инжекции(частотные характеристики)
		float u2; 			// Выходное напряжение, В
		float iL; 			// Ток реактора
		float temperature;  // Температура
		float u1; 			// Входное напряжение
		float in; 			// Выходной ток
	} data, shift, scale, sum; 	// data - рассчитанное значение
	// shift - смещение значения
	// scale - коэффициент масштабироания
	// sum - переменная для накопления при автоопределении смещения

	struct {
		float data;			// Значение переменной для вывода на ЦАП
		float shift;		// Смещение значения переменной
		float scale;		// Коэффициент масштабирования переменной

	} dac[2]; // Структура с параметрами ЦАП1 и ЦАП2

} Measure_Struct; // Структура с параметрами измерений

typedef struct {

	float iL_max; // Условие срабатывания защиты по току реактора
	float in_max; // Условие сабатывания защиты по максимальному выходному току
	float u1_max; // условие срабатывания защиты по максимальному входному напряжению
	float u2_max; // условие срабатывания защиты по максимальному выходному напряжению

	float iL_int_sum; // Сумма (выход интегратора) интегрально-токовой защиты (A*c)
	float iL_int_max; // Уставка условия срабатывания интегрально-токовой защиты (A*c)
	float iL_n;       // Номинальный ток реатора

	struct{
		float duty_min;		// Минимальный коэффициент заполнения
		float duty_max;		// Максимальный коэффициент заполнения
	}sat; // Параметры ограничителей

} Protect_Struct; // Структура с параметрами защит

extern Control_Struct Boost_Control;
extern Measure_Struct Boost_Measure;
extern Protect_Struct Boost_Protect;
extern kiss_fftr_cfg FFT_CFG;

#endif
