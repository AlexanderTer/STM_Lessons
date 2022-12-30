#include "stm32f7xx.h"
#include "control.h"
#include "dsp.h"
#include "timer.h"

Control_Struct Boost_Control = {
		.pid_current = {
				// Пропорциональный коэффициент
				.kp =  0.055769,

				.integrator = {

						// Интегральный коэффициент
						.k = 471.19 * (TS / 2),
						.sat = {.min = 0, .max = 0.98}
				},
				.diff = {

						// Дифференциальный коэффициент
						.k = 0 * FS,
				},
				.sat = {.min = 0, .max = 0.98}
		},

		.pid_voltage = {
						// Пропорциональный коэффициент
						.kp =   1.0130,

						.integrator = {

								// Интегральный коэффициент
								.k = 2530.6 * (TS / 2),
								.sat = {.min = 0, .max = 7}
						},
						.diff = {

								// Дифференциальный коэффициент
								.k =  0* FS,
						},
						.sat = {.min = -7, .max = 7}
				},

};
Measure_Struct Boost_Measure = {

#define V25 0.76
#define AV_SLOPE (2.5 * 0.001)

		.shift = {
				.inj = 0,		// Устанавливается по нажатию SW1
				.u2 = 100.4,
				.iL = 0,
				.temperature = 25.f - V25 / AV_SLOPE,
				.u1 = 0, .in = 0 },

		.scale = { .inj = 3.3 / 4095.,
				.u2 = -0.001880341880341881,
				.iL =
				1.9794e-03, .temperature = 3.3 / (4095. * AV_SLOPE),
				.u1 = 0.024982,
				.in = 9.8970e-04 },

		.dac[0] = {.shift = 0., .scale = 4095./5. },

		.dac[1] = {.shift = 0, .scale = 4095./5.},
};

Protect_Struct Boost_Protect = {
		.iL_max = 8.,
		.in_max = 3.,
		.u1_max = 90.,
		.u2_max = 100.,

		.iL_n = 6.,
		.iL_int_max = 6. * 10.,

		.sat = {
				.duty_min = 0.02,
				.duty_max = 0.98,
		},
};


LinearRamp_Struct LINEAR_RAMP =
{
		.integrator =
		{
				.k = 0.25 * TS,
				.sat = {.min = -999999., .max = 999999.},
		},
};

SSHapedRamp_Struct SSHAPED_RAMP =
{
		.integrator[0] =
		{
				.k = 1.*TS,
				.sat = { .min = -999999., .max = 999999.}
		},

		.integrator[1] =
		{
				.k = 0.25 *TS,
				.sat = { .min = -999999., .max = 999999.}
		},
		.k3 = 0.125

};

// Усредняющий фильтр выходного тока
MovingFloatFilter_Struct IN_FILTER;

float REF_CURRENT = 0.;
float REF_VOLTAGE = 97.;


volatile float TEMPERATURE;
void shift_and_scale(void);
void set_shifts(void);
void protect_software(void);
void integral_protect(void);

void timer_PWM_Off(void);
void DMA2_Stream0_IRQHandler(void) {



	// Сброс флага DMA2 по окончанию передачи данных
	DMA2->LIFCR |= DMA_LIFCR_CTCIF0;

	// Ожидание выполнения инструкций
	__ISB();

	shift_and_scale();

	protect_software();

	set_shifts();

	// Усреднение выходного тока
	Boost_Measure.data.in_av = MovingFloatFilter(&IN_FILTER, Boost_Measure.data.in);

//	static unsigned int cnt = 0; // 50 = 0.01 / 2 * Fs
//	cnt++;
//	if (cnt < 50)
//		REF_CONTROLLER = 3.5f;
//	else if (cnt < 100)
//		REF_CONTROLLER = 4.5f;
//	else
//		cnt = 0;

// Voltage loop calculate
	// Ошибка регулирования по напряжению
	float error_Voltage = U_REF -  Boost_Measure.data.u2 + Boost_Measure.data.in_av * R_COMP;
	// Расчёт ПИД - регулятора
     REF_CURRENT = PID_Controller(&Boost_Control.pid_voltage, error_Voltage) + Boost_Measure.data.inj * (0.0f / 1.65f );
	// Уставка на ток реактора = 4А
	//REF_CONTROLLER = 3.5f + Boost_Measure.data.inj * (0.4f / 1.65f );

	// Ошибка регулирования
	float error_current = REF_CURRENT - Boost_Measure.data.iL;


	// Расчёт ПИД - регулятора
	float PID_output = PID_Controller(&Boost_Control.pid_current, error_current);
	Boost_Control.duty = PID_output;

	// Холостой ход (Burst mode)
	if ((Boost_Control.duty < Boost_Protect.sat.duty_min) || (REF_CURRENT < 0))
		TIM8->CCR1 = 0;
	else if(Boost_Control.duty > Boost_Protect.sat.duty_max )
		TIM8->CCR1 = TIM8->ARR * Boost_Protect.sat.duty_max;
	else
		TIM8->CCR1 = TIM8->ARR * Boost_Control.duty;

	// Регистр сравнения: ARR * (коэффициент заполнения)
		// TIM8->CCR1 = TIM8->ARR * LIMIT(Boost_Control.duty, Boost_Protect.sat.duty_min,Boost_Protect.sat.duty_max);

	/* Вывод сигнала на цап*/
	unsigned int dac1, dac2;

	//
	Boost_Measure.dac[0].data =  REF_CURRENT;

	// Выводим переменную на ЦАП2
	Boost_Measure.dac[1].data = REF_CURRENT - Boost_Measure.data.inj * (0.0f / 1.65f );

	// Фильтруем переменную
	//dac2 = MovingFloatFilter(&FILTER_MOV, Boost_Measure.data.inj) * (4095.f / 100.f);

	// Пересчитываем внутренние переменные в значения регистров ЦАП1 и ЦАП2
	dac1 = Boost_Measure.dac[0].scale * Boost_Measure.dac[0].data
			+ Boost_Measure.dac[0].shift;
	dac2 = Boost_Measure.dac[1].scale * Boost_Measure.dac[1].data
			+ Boost_Measure.dac[1].shift;

	// Запись чисел в ЦАП1 и ЦАП2
	DAC->DHR12RD = dac1 | (dac2 << 16);

}

/**
 * \brief Функция пересчёта значений в физические величины
 */
void shift_and_scale(void) {
	extern volatile unsigned int ADC_Buffer[];
	Boost_Measure.data.inj = Boost_Measure.scale.inj * ADC_Buffer[0]
			+ Boost_Measure.shift.inj;
	Boost_Measure.data.u2 = Boost_Measure.scale.u2 * ADC_Buffer[1]
			+ Boost_Measure.shift.u2;
	Boost_Measure.data.iL = Boost_Measure.scale.iL * ADC_Buffer[2]
			+ Boost_Measure.shift.iL;
	Boost_Measure.data.temperature = Boost_Measure.scale.temperature
			* ADC_Buffer[3] + Boost_Measure.shift.temperature;
	Boost_Measure.data.u1 = Boost_Measure.scale.u1 * ADC_Buffer[4]
			+ Boost_Measure.shift.u1;
	Boost_Measure.data.in = Boost_Measure.scale.in * ADC_Buffer[5]
			+ Boost_Measure.shift.in;
}

// Функция автоопределения смещения для ацп
void set_shifts(void) {
	if (Boost_Measure.count == 0)
		return;

	// Обнуление текущего смещения и суммы при старте алгоритма автоопределения смещения.
	if (Boost_Measure.count == SET_SHIFTS_MAX_COUNT)
		Boost_Measure.shift.inj = Boost_Measure.sum.inj = 0;

	// Накапливаем сумму.
	Boost_Measure.sum.inj += Boost_Measure.data.inj
			* (1.f / SET_SHIFTS_MAX_COUNT);

	// Декремент счётчика и проверка окончания автоопределения смещений.
	if (--Boost_Measure.count == 0)
		Boost_Measure.shift.inj = -Boost_Measure.sum.inj;
}

// Функция программных защит
void protect_software(void) {

	if (Boost_Measure.data.iL > Boost_Protect.iL_max) {
		timer_PWM_Off();
		GPIOD->ODR |= 1 << 2;
	}

	if (Boost_Measure.data.in > Boost_Protect.in_max) {
		timer_PWM_Off();
		GPIOD->ODR |= 1 << 3;
	}

	if (Boost_Measure.data.u1 > Boost_Protect.u1_max) {
		timer_PWM_Off();
		GPIOD->ODR |= 1 << 4;
	}

	if (Boost_Measure.data.u2 > Boost_Protect.u2_max) {
		timer_PWM_Off();
		GPIOD->ODR |= 1 << 5;
	}

	// Интегральная токовая защита
	integral_protect();
}

/**
 * \brief Функция интегрально-токовой защиты по току реактора
 */
void integral_protect(void){

	// Разница между током реатора и его номинальным значением
	float x = Boost_Measure.data.iL - Boost_Protect.iL_n;

	// Расчёт выхода интегратора
	Boost_Protect.iL_int_sum = Boost_Protect.iL_int_sum + x * TS;

	// Обнуляем интегратор в нормальном режиме работы
	if (Boost_Protect.iL_int_sum < 0) Boost_Protect.iL_int_sum = 0;

	// Проверяем условие срабаотывания защиты
	if(Boost_Protect.iL_int_sum > Boost_Protect.iL_int_max){
		Boost_Protect.iL_int_sum = 0;
		timer_PWM_Off();
		GPIOD->ODR &= ~((1 << 2) | (1 << 3) | (1 << 4) | (1 << 5));

	}
}


/**
 *  \brief Функция обработчик прерывания EXTI1 (1 линия), PB1.
 */
void EXTI1_IRQHandler(void){

	// Сброс флага прерывания EXTI1.
	EXTI->PR |= EXTI_PR_PR1;
	__ISB();

	// Если на PB1 логический 0, включаем ШИМ
	//if(!(GPIOB->IDR & (1<<1)))
	//	timer_PWM_Off();
	//else
	//	timer_PWM_On();


}
