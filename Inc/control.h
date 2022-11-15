#ifndef  __CONTROL_H__
#define __CONTROL_H__

#define SET_SHIFTS_MAX_COUNT ((unsigned int)(0.5 * 100.e3))

typedef struct {

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

} Protect_Struct; // Структура с параметрами защит

extern Control_Struct Boost_Control;
extern Measure_Struct Boost_Measure;
extern Protect_Struct Boost_Protect;

#endif
