/**
 *\brief Функция фильтра скользящего среднего
 *
 *\param filter: структура с параметрами фильтра
 *\param x: входная переменная
 *\return y: среднее значение
 *
 */

#include "stm32f7xx.h"
#include "dsp.h"

#include <stdlib.h>
#include <string.h>

MovingFloatFilter_Struct FILTER_MOV;
MedianFloatFilter_Struct FILTER_MED;
Low_Filter_1st_Order_Struct FILTER_1ORD = {
		.b0 = TS / (TAU_1ORD + TS),
		.a1 = -TAU_1ORD / (TAU_1ORD + TS)
};
DigitalFilter_Struct FILTER_DIG1 = {
		.b[0] = 0.013359200027857,
		.b[1] = 0.026718400055713,
		.b[2] = 0.013359200027857,
		.a[0] =1,
		.a[1] = -1.647459981076977,
		.a[2] = 0.700896781188403,
};

DigitalFilter_Struct FILTER_DIG2 = {
		.b[0] = 0.993755964953657,
		.b[1] = -1.925108587845861,
		.b[2] = 0.993755964953657,
		.a[0] = 1,
		.a[1] = -1.925108587845860,
		.a[2] = 0.987511929907314,
};

float MovingFloatFilter(MovingFloatFilter_Struct *filter, float x) {

	// Отнимаем от суммы [n-1] точку и прибавляем [0] точку
	filter->sum = filter->sum - filter->buf[filter->pointer] + x;

	// Добавляем новую точку в массив точек
	filter->buf[filter->pointer] = x;

	if (++filter->pointer >= MAX_MOVING_FLOAT_SIZE)
		filter->pointer = 0;
	return filter->sum * (1.f / MAX_MOVING_FLOAT_SIZE);
}

int cmp(const void *a, const void *b) {

	float *af = (float*) a;
	float *bf = (float*) b;
	if (*af > *bf)
		return 1;
	if (*af < *bf)
		return -1;
	return 0;

}

float MedianFloatFilter(MedianFloatFilter_Struct *filter, float x) {

// Добавляем новую точку в массив точек
	filter->buf[filter->pointer] = x;

	if (++filter->pointer >= MAX_MEDIAN_FLOAT_SIZE)
		filter->pointer = 0;

// Делаем копию массива точек
	for (int i = 0; i < MAX_MEDIAN_FLOAT_SIZE; i++)
		filter->buf_sorted[i] = filter->buf[i];

	__ISB();
	__DSB();

// Сортируем массив
	qsort(filter->buf_sorted, MAX_MEDIAN_FLOAT_SIZE,
			sizeof(filter->buf_sorted[0]),
			(int (*)(const void*, const void*)) cmp);

	return filter->buf_sorted[MAX_MEDIAN_FLOAT_SIZE >> 1];
}

/**
 * \brief Функция фильтра нижних частот первого порядка
 */

float Low_Filter_1st_Order(Low_Filter_1st_Order_Struct * filter, float x){

  float y = x * filter->b0 - filter->yn * filter->a1;

  //Сохраняем выходную переменную для следующего такта
  filter->yn = y;
  return y;

}

/**
 * \brief Функция цифрового фильтра в прямой форме II
 * \param filter: структура с параметрами фильтра
 * \param х: выходная переменная
 * \return y: значение после фильтрации
 */
float DirectFormII_FloatFilter(DigitalFilter_Struct * filter, float x){
	float y = 0;

	// w = x
	filter ->z[0][0] = x;
	for (int i = MAX_ORDER_DIGITAL_FILTER; i >= 1; i--){

		// Суммирование ветвей с коэффициентами а
		filter -> z[0][0] -= filter ->z[0][i] * filter->a[i];

		// Суммироание ветвей с коэффициентами b
		y += filter->z[0][i] * filter ->b[i];

		// Сохранение предыдущих значений
		filter->z[0][i] = filter->z[0][i-1];


	}

	// y += w * b0
	y += filter ->z[0][0] * filter ->b[0];

	return y;
}


float DirectFormI_FloatFilter(DigitalFilter_Struct * filter, float x){
	filter -> z[0][0] = x;
	filter -> z[1][0] = x * filter->b[0];

	// Вычисление первого каскада с коэффициентами b
	for (int i = MAX_ORDER_DIGITAL_FILTER; i >= 1; i--){
		// v = v + bi * x * z^(-i)
		filter -> z[1][0] += filter ->z[0][i] * filter ->b[i];
		filter-> z[0][i] = filter -> z[0][i-1];
	}

	// Вычисление второго каскада с коэффициентами а
	for (int i = MAX_ORDER_DIGITAL_FILTER; i >= 1; i--){

		filter -> z[1][0] -= filter ->z[1][i] * filter ->a[i];
		filter-> z[1][i] = filter -> z[1][i-1];
		}
	return filter ->z[1][0];
}
