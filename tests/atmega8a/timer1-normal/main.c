#define F_CPU			16000000UL
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define SET_BIT(byte, bit)	((byte)|=(1UL<<(bit)))
#define CLEAR_BIT(byte, bit)	((byte)&=~(1UL<<(bit)))
#define IS_SET(byte, bit)	(((byte)&(1UL<<(bit)))>>(bit))

void timer1_init(void);

static volatile int ocr2_up = 0;

int main(void)
{
	timer1_init();
	sei();

	while (1) {
	}
	return 0;
}

void timer1_init(void)
{
	/* Toggle OC1A/OC1B on Compare Match */
	TCCR1A |= (1<<COM1A0);
	TCCR1A &= (uint8_t)(~(1<<COM1A1));
	TCCR1A |= (1<<COM1B0);
	TCCR1A &= (uint8_t)(~(1<<COM1B1));

	/* Normal mode */
	TCCR1A &= (uint8_t)(~(1<<WGM10));
	TCCR1A &= (uint8_t)(~(1<<WGM11));
	TCCR1B &= (uint8_t)(~(1<<WGM12));
	TCCR1B &= (uint8_t)(~(1<<WGM13));
	DDRB |= (1<<PB3);

	/* Set initial values */
	TCNT2 = 0;
	OCR1A = 1700;
	OCR1B = 1500;
	TIMSK |= (1<<TOIE1)|(1<<OCIE1A)|(1<<OCIE1B);

	/* Prescaler to clk_io/8 */
	TCCR1B &= (uint8_t)(~(1<<CS10));
	TCCR1B |= (1<<CS11);
	TCCR1B &= (uint8_t)(~(1<<CS12));
}

ISR(TIMER1_OVF_vect)
{
	ocr2_up = (ocr2_up != 0) ? 0 : 1;
}
