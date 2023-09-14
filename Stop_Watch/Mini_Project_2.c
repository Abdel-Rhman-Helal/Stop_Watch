/*
 =============================================================================
 *	 Name File  : Mini_Project_2.c
 *	 Diploma    : 79
 *	 Name       : Abdel Rhman Helal
 *   Created on : Sep 12, 2023
 =============================================================================
 */
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
/*
 GLOBAL Varibales to display on 7Segmant
 g_sec    -> first seven seg
 g_sec_1  -> 2 seven seg
 .
 .
 g_hour_1 -> 6 seven seg
 */
unsigned char g_sec = 0, g_sec_1 = 0, g_min = 0, g_min_1 = 0, g_hour = 0,
		g_hour_1 = 0;
void Timer1_CTC(void) {
	TCNT1 = 0; //initial value
	OCR1A = 977;
	/* Configure timer control register
	 * 1. Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
	 * 2. FOC1A=1 FOC1B=0
	 * 3. CTC Mode WGM10=0 WGM11=0 WGM12=1 WGM13=0 (Mode Number 4)
	 */
	TCCR1A = (1 << FOC1A); //NON PWM
	/*
	 USING Prescaler 1024.
	 we have Time for one  = 1024/1Mhz = 1.024ms
	 OCR1A  = 1s/1.024ms = 976.5 = 977
	 */
	TCCR1B = (1 << WGM12) | (1 << CS10) | (1 << CS12);
	TIMSK |= (1 << OCIE1A); //for Interrupt Enable
	SREG |= (1 << 7); //G_Interrupt Enable
	// Timer 1 will go to ISR every 1s
}
ISR(TIMER1_COMPA_vect) {
	g_sec++;
	if (g_sec > 9) {
		g_sec_1++;
		g_sec = 0;
	}
	if (g_sec_1 > 5) {
		g_min++;
		g_sec = 0;
		g_sec_1 = 0;
	}
	if (g_min > 9) {
		g_min_1++;
		g_min = 0;
		g_sec = 0;
		g_sec_1 = 0;
	}
	if (g_min_1 > 5) {
		g_hour++;
		g_min = 0;
		g_sec = 0;
		g_sec_1 = 0;
		g_min_1 = 0;
	}
	if (g_hour > 9) {
		g_hour_1++;
		g_hour = 0;
		g_min = 0;
		g_sec = 0;
		g_sec_1 = 0;
		g_min_1 = 0;
	}
	if (g_hour_1 > 9) {
		g_hour_1 = 0;
		g_hour = 0;
		g_min = 0;
		g_sec = 0;
		g_sec_1 = 0;
		g_min_1 = 0;
	}

}
void INT0_Init(void) {
//	Configure External Interrupt INT0 with falling edge. Connect a push button with the
//	internal pull-up resistor. If a falling edge detected the Stop Watch time should be
//	reset.
	DDRD &= ~(1 << PD2);      // INPUT PIN.
	PORTD |= (1 << PD2);     //  internal pull-up resistor.
	MCUCR |= (1 << ISC01);	//   falling edge
	GICR |= (1 << INT0);   //    External Interrupt INT0
	SREG |= (1 << 7);     //     Global Interrupt
}
ISR(INT0_vect) {
	TCNT1 = 0;     //Reset Initial value of timer
	//Reset All counter
	g_sec = 0;
	g_sec_1 = 0;
	g_min = 0;
	g_min_1 = 0;
	g_hour = 0;
	g_hour_1 = 0;
}
void INT1_Init(void) {
//	 Configure External Interrupt INT1 with raising edge. Connect a push button with the
//	external pull-down resistor. If a raising edge detected the Stop Watch time should be
//	paused.
	DDRD &= ~(1 << PD3);                    // INPUT PIN.
	MCUCR |= (1 << ISC11) | (1 << ISC10); //External Interrupt INT1 with raising edge
	GICR |= (1 << INT1); 				   //External Interrupt INT1
	SREG |= (1 << 7);					  //Global Interrupt
}
ISR(INT1_vect) {
	//No clock source (Timer/Counter stopped)
	//CS10=0,CS11=0,CS12=0
	TCCR1B &= (~(1 << CS10)) & (~(1 << CS11)) & (~(1 << CS12));
}
void INT2_Init(void) {
//	Configure External Interrupt INT2 with falling edge. Connect a push button with the
//	internal pull-up resistor. If a falling edge detected the Stop Watch time should be
//	resumed
	DDRB &= ~(1 << PB2);      // INPUT PIN.
	PORTB |= (1 << PB2);     //  internal pull-up resistor.
	MCUCSR &= ~(1 << ISC2);	//   falling edge of INT2
	GICR |= (1 << INT2);   //    External Interrupt INT2
	SREG |= (1 << 7);     //     Global Interrupt
}
ISR(INT2_vect) {
	//clkI/O/1024 (From prescaler)
	TCCR1B |= (1 << CS10) | (1 << CS12);
}
int main(void) {
	DDRC |= 0x0F;
	DDRA |= 0x3F;
	INT0_Init();
	INT1_Init();
	INT2_Init();
	Timer1_CTC();
	for (;;) {
		//Open EN For every seven seg and polling on six seven segmant
		//to make all run and display a diff Numbers
		PORTA |= (1 << 5);
		PORTC = g_sec;
		_delay_ms(5);
		PORTA &= ~(1 << 5);
		PORTA |= (1 << 4);
		PORTC = g_sec_1;
		_delay_ms(5);
		PORTA &= ~(1 << 4);
		PORTA |= (1 << 3);
		PORTC = g_min;
		_delay_ms(5);
		PORTA &= ~(1 << 3);
		PORTA |= (1 << 2);
		PORTC = g_min_1;
		_delay_ms(5);
		PORTA &= ~(1 << 2);
		PORTA |= (1 << 1);
		PORTC = g_hour;
		_delay_ms(5);
		PORTA &= ~(1 << 1);
		PORTA |= (1 << 0);
		PORTC = g_hour_1;
		_delay_ms(5);
		PORTA &= ~(1 << 0);

	}
}
