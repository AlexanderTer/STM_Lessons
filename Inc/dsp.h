#ifndef  __DSP_H__
#define __DSP_H__



#define MAX_MOVING_FLOAT_SIZE 500
#define MAX_MEDIAN_FLOAT_SIZE 500

#define TS (1/100.e3)    // Период дискретизации
#define TAU_1ORD (0.001 / 2. / 3.1415926535) // Постоянная времени фильтра

typedef struct{
	float buf[MAX_MOVING_FLOAT_SIZE];

	unsigned int pointer;

	float sum;

} MovingFloatFilter_Struct;


typedef struct{
	float buf[MAX_MEDIAN_FLOAT_SIZE];

	float buf_sorted[MAX_MEDIAN_FLOAT_SIZE];

	unsigned int pointer;


} MedianFloatFilter_Struct;

typedef struct{

	float yn;

	float b0;
	float a1;

}Low_Filter_1st_Order_Struct;


float MovingFloatFilter(MovingFloatFilter_Struct *filter, float x);
float MedianFloatFilter(MedianFloatFilter_Struct *filter, float x);
float Low_Filter_1st_Order(Low_Filter_1st_Order_Struct * filter, float x);

extern MovingFloatFilter_Struct FILTER_MOV;
extern MedianFloatFilter_Struct FILTER_MED;
extern Low_Filter_1st_Order_Struct FILTER_1ORD;

#endif
