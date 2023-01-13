#include "stm32f7xx.h"
#include "control.h"
#include "dsp.h"
#include "timer.h"
#include "crc.h"
#include "uart.h"


Control_Struct Boost_Control = {
		.pid_current = {
		// Пропорциональный коэффициент
		.kp = F2Q(0.055769),

		.integrator = {

		// Интегральный коэффициент
				.k = F2Q(471.19 * (TS / 2)),
				.sat = { .min = F2Q(0), .max = F2Q(0.98) } },
		.diff = {

		// Дифференциальный коэффициент
				.k = F2Q(0 * FS), },
				.sat = { .min = 0, .max = F2Q(0.98) } },

};
Measure_Struct Boost_Measure = {

#define V25 F2Q(0.76)
#define AV_SLOPE (F2Q(2.5 * 0.001))

		.shift = {
				.inj = 0,		// Устанавливается по нажатию SW1
				.u2 = F2Q(100.4),
				.iL = 0,
				.temperature = F2Q(25.f - V25 / AV_SLOPE),
				.u1 = 0,
				.in = 0 },

		.scale = {
				.inj = F2Q(3.3 / 4095.),
				.u2 = F2Q(-0.001880341880341881),
				.iL = F2Q(1.9794e-03),
						.temperature =F2Q( 3.3 / (4095. * AV_SLOPE)),
						.u1 = F2Q(0.024982),
				.in =F2Q( 9.8970e-04)
		},

		.dac[0] = { .shift = 0., .scale = F2Q(4095. / 5.) },

		.dac[1] = { .shift = 0, .scale =F2Q( 4095. / 5.) }, };

Protect_Struct Boost_Protect = {
		.iL_max = F2Q(8.),
		.in_max =F2Q( 3.),
		.u1_max = F2Q(90.),
		.u2_max = F2Q(100.),

		.iL_n = F2Q(6.),
		.iL_int_max = F2Q(6. * 10.),

		.sat = {
				.duty_min = F2Q(0.02),
				.duty_max = F2Q(0.98),
},
};


// Усредняющий фильтр выходного тока
MovingFloatFilter_Struct IN_FILTER;

float REF_CURRENT = 0.;


volatile float TEMPERATURE;
void shift_and_scale(void);
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




	// Ошибка регулирования тока ректора
		float error_current = REF_CURRENT - Boost_Measure.data.iL;

	// Расчёт ПИД - регулятора
	float PID_output = PID_Controller_Q(&Boost_Control.pid_current,
			error_current);

	Boost_Control.duty = PID_output;

	// Холостой ход (Burst mode)
	if ((Boost_Control.duty < Boost_Protect.sat.duty_min))
		TIM8->CCR1 = 0;
	else if (Boost_Control.duty > Boost_Protect.sat.duty_max)
		TIM8->CCR1 = Q2F(MUL_Q(F2Q(TIM8->ARR) , Boost_Protect.sat.duty_max));
	else
		TIM8->CCR1 = Q2F(MUL_Q(F2Q(TIM8->ARR) , Boost_Protect.sat.duty));
}

/**
 * \brief Функция пересчёта значений в физические величины
 */
void shift_and_scale(void) {
	extern volatile unsigned int ADC_Buffer[];
	Boost_Measure.data.inj = MUL_Q(Boost_Measure.scale.inj, F2Q(ADC_Buffer[0]))
			+ Boost_Measure.shift.inj;
	Boost_Measure.data.u2 = MUL_Q(Boost_Measure.scale.u2 , F2Q(ADC_Buffer[1]))
			+ Boost_Measure.shift.u2;
	Boost_Measure.data.iL =MUL_Q( Boost_Measure.scale.iL , F2Q(ADC_Buffer[2])
			+ Boost_Measure.shift.iL);
	Boost_Measure.data.temperature = MUL_Q(Boost_Measure.scale.temperature
			, F2Q(ADC_Buffer[3]) + Boost_Measure.shift.temperature);
	Boost_Measure.data.u1 = MUL_Q(Boost_Measure.scale.u1 , F2Q(ADC_Buffer[4])
			+ Boost_Measure.shift.u1);
	Boost_Measure.data.in = MUL_Q(Boost_Measure.scale.in , F2Q(ADC_Buffer[5])
			+ Boost_Measure.shift.in);
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
void integral_protect(void) {

	// Разница между током реатора и его номинальным значением
	int32_t x = Boost_Measure.data.iL - Boost_Protect.iL_n;

	// Расчёт выхода интегратора
	Boost_Protect.iL_int_sum = Boost_Protect.iL_int_sum + MUL_Q(x , F2Q(TS));

	// Обнуляем интегратор в нормальном режиме работы
	if (Boost_Protect.iL_int_sum < 0)
		Boost_Protect.iL_int_sum = 0;

	// Проверяем условие срабаотывания защиты
	if (Boost_Protect.iL_int_sum > Boost_Protect.iL_int_max) {
		Boost_Protect.iL_int_sum = 0;
		timer_PWM_Off();
		GPIOD->ODR &= ~((1 << 2) | (1 << 3) | (1 << 4) | (1 << 5));

	}
}

