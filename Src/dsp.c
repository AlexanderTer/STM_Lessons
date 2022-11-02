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
