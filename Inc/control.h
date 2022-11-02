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
		float il; 			// Ток реактора
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

} Protect_Struct; // Структура с параметрами защит

extern Control_Struct Boost_Control;
extern Measure_Struct Boost_Measure;
extern Protect_Struct Boost_Protect;

#endif
