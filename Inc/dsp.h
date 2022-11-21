#ifndef  __DSP_H__
#define __DSP_H__

#define MAX_MOVING_FLOAT_SIZE 500
#define MAX_MEDIAN_FLOAT_SIZE 500
#define MAX_ORDER_DIGITAL_FILTER (2)

#define FS (100000.) // Частота дискретизации
#define TS (1./(FS)) // Период дискретизации


//#define TS (1/100.e3)    // Период дискретизации
#define TAU_1ORD (0.001 / 2. / 3.1415926535) // Постоянная времени фильтра

#define LIMIT(x,min,max) (((x) < (min)) ? (min) : (((x) > (max)) ? (max):(x)))

typedef struct {
	float buf[MAX_MOVING_FLOAT_SIZE];

	unsigned int pointer;

	float sum;

} MovingFloatFilter_Struct;

typedef struct {
	float buf[MAX_MEDIAN_FLOAT_SIZE];

	float buf_sorted[MAX_MEDIAN_FLOAT_SIZE];

	unsigned int pointer;

} MedianFloatFilter_Struct;

typedef struct {

	float yn;

	float b0;
	float a1;

} Low_Filter_1st_Order_Struct;

typedef struct {
	float a[MAX_ORDER_DIGITAL_FILTER + 1]; // КОэффициенты знаменателя ПФ фильтра
	float b[MAX_ORDER_DIGITAL_FILTER + 1]; // Коэффициенты числителя ПФ фильтра

	float z[2] [MAX_ORDER_DIGITAL_FILTER + 1]; // Массив для хранения предыдущих значений

} DigitalFilter_Struct;


typedef struct{

	float k;   // Коэффициент игтегратора (включает период дискретизации)
	float sum; // Сумма интегратора (выход)

	struct{
		float min; // Минимальное значение суммы
		float max; // Максимальное значение суммы
	}sat; // Струкура с параметрами ограничителя суммы

}BackwardEuler_Integrator_Struct;	// Структура параметров интегратора (метод прямоугольников)


typedef struct{

	BackwardEuler_Integrator_Struct integrator;

}LinearRamp_Struct; // Структура парметров линейного задатчика


typedef struct{

	BackwardEuler_Integrator_Struct integrator[2];

	float k3;	// Коэффициент дополнительной нелинейной связи

}SSHapedRamp_Struct; // Структура парметров S-образного задатчика


float MovingFloatFilter(MovingFloatFilter_Struct *filter, float x);
float MedianFloatFilter(MedianFloatFilter_Struct *filter, float x);
float Low_Filter_1st_Order(Low_Filter_1st_Order_Struct *filter, float x);
float DirectFormII_FloatFilter(DigitalFilter_Struct * filter, float x);
float DirectFormI_FloatFilter(DigitalFilter_Struct * filter, float x);
float LinearRamp(LinearRamp_Struct * ramp, float x);
float SSHapedRamp(SSHapedRamp_Struct * ramp, float x);

extern MovingFloatFilter_Struct FILTER_MOV;
extern MedianFloatFilter_Struct FILTER_MED;
extern Low_Filter_1st_Order_Struct FILTER_1ORD;
extern DigitalFilter_Struct FILTER_DIG1;
extern DigitalFilter_Struct FILTER_DIG2;

#endif
