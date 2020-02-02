#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Interrupt.h"
#include "Debounce.h"

#define CURRENT_THRESHOLD 600

#define AMP_LED_COM  (_BV(COM0A1) | _BV(COM0A0))
#define AMP_LED_CS   (_BV(CS02) | _BV(CS00))

// EFFECTS: sets up every pin for i/o
void setup_pins();

// EFFECTS: sets up the amp led timer
void setup_led_timer();

// EFFECTS: enables the ADC, auto trigger
void setup_ADC();

// EFFECTS: blinks some LEDs
void LED_starting_sequence();

// EFFECTS: toggles the output of channel 1
void output_1_toggle();

// EFFECTS: toggles the output of channel 2
void output_2_toggle();

// EFFECTS: disables output of channel 1
void output_1_off();

// EFFECTS: disables output of channel 2
void output_2_off();

// EFFECTS: turns on status led 1
void volt_led_on();

// EFFECTS: turns off status led 1
void volt_led_off();

// EFFECTS: turns on status led 2
void amp_led_on();

// EFFECTS: turns off status led 2
void amp_led_off();

// EFFECTS: acknowledges over current condition, turns off amp led
void ack_over_current();

volatile uint16_t current_reading = 0;
uint8_t over_current_status = 0;

int main() {
	setup_pins();
	LED_starting_sequence();
	setup_ADC();
	debounce_init();
	Interrupt_init();
	setup_led_timer();

	while (1) {
		if (button_down(CH1_ENABLE)) {
			if (over_current_status) {
				ack_over_current();
			} else {
				output_1_toggle();
			}
		}

		if (button_down(CH2_ENABLE)) {
			if (over_current_status) {
				ack_over_current();
			} else {
				output_2_toggle();
			}
		}

		if (current_reading >= CURRENT_THRESHOLD) {
			over_current_status = 1;

			output_1_off();
			output_2_off();
			amp_led_on();
		}
	}
}

// ISR: triggers on ADC conversion ready
ISR(ADC_vect) {
	// move sensor reading into correct global buffer
	current_reading = ADCL;
	current_reading |= (ADCH << 8);
}

// EFFECTS: sets up every pin for i/o
void setup_pins() {
	// Status LED 1 (output, default off)
	DDRA |= _BV(PORTA1);

	// Status LED 2 (output, default off)
	DDRB |= _BV(PORTB2);

	// Channel 1 Enable Button (input, pullup enabled)
	PORTA |= _BV(PORTA3);

	// Channel 2 Enable Button (input, pullup enabled)
	PORTA |= _BV(PORTA2);

	// Load Switch 1 (output, default off)
	DDRB |= _BV(PORTB0);

	// Load Switch 2 (output, default off)
	DDRB |= _BV(PORTB1);
}

// EFFECTS: sets up the amp led timer
void setup_led_timer() {
	TCCR0A |= _BV(WGM01) | _BV(WGM00); // fast PWM
	OCR0A = 128; // 50% duty cycle
}

// EFFECTS: enables the ADC, auto trigger
void setup_ADC() {
	ADCSRA |= _BV(ADEN) | _BV(ADIE) | _BV(ADATE) | _BV(ADSC);
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

// EFFECTS: blinks some LEDs
void LED_starting_sequence() {
	volt_led_on();
	PORTB |= _BV(PORTB2);
	_delay_ms(800);
	volt_led_off();
	PORTB &= ~_BV(PORTB2);
}

// EFFECTS: toggles the output of channel 1
void output_1_toggle() {
	PORTB ^= _BV(PORTB0);
}

// EFFECTS: toggles the output of channel 2
void output_2_toggle() {
	PORTB ^= _BV(PORTB1);
}

// EFFECTS: disables output of channel 1
void output_1_off() {
	PORTB &= ~_BV(PORTB0);
}

// EFFECTS: disables output of channel 2
void output_2_off() {
	PORTB &= ~_BV(PORTB1);
}

// EFFECTS: turns on status led 1
void volt_led_on() {
	PORTA |= _BV(PORTA1);
}

// EFFECTS: turns off status led 1
void volt_led_off() {
	PORTA &= ~_BV(PORTA1);
}

// EFFECTS: turns on status led 2
void amp_led_on() {
	TCCR0A |= AMP_LED_COM;  // connect LED
	TCCR0B |= AMP_LED_CS;   // enable clk
}

// EFFECTS: turns off status led 2
inline void amp_led_off() {
	TCCR0B &= ~AMP_LED_CS;   // disable led blink
	TCCR0A &= ~AMP_LED_COM;  // disconnect LED
	PORTB &= ~_BV(PORTB2);   // disable LED
	TCNT0 = 127;			 // make the LED turn on faster...
}

// EFFECTS: acknowledges over current condition, turns off amp led
inline void ack_over_current() {
	amp_led_off();
	over_current_status = 0;
}
