#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Interrupt.h"
#include "Debounce.h"

#define CH1_EN 0
#define CH2_EN 1

// EFFECTS: sets up every pin for i/o
void setup_pins();

// EFFECTS: toggles the output of channel 1
void output_1_toggle();

// EFFECTS: toggles the output of channel 2
void output_2_toggle();

// EFFECTS: turns on status led 1
void status_1_on();

// EFFECTS: turns off status led 1
void status_1_off();

// EFFECTS: turns on status led 2
void status_2_on();

// EFFECTS: turns off status led 2
void status_2_off();

int main() {
	sei();
	setup_pins();
	debounce_init();


	Interrupt_init();

	status_1_on();
	status_2_on();

	_delay_ms(300);
	status_1_off();
	status_2_off();
	while(1) {

		/*
		 * TODO:
		 * 1. toggle outputs when buttons pressed
		 * 2. read current sensor and voltage ADC value
		 * 3. compute current from conversion
		 * 4. shut off outputs if any above threshold
		 * 5. turn on status LEDs if above threshold
		 * 6. clear AMP when any button pressed
		 */

		if(button_down(BUTTON1_MASK)) {
			output_1_toggle();
		}

		if(button_down(BUTTON2_MASK)) {
			output_2_toggle();
		}

		for(int i = 0; i < 50; i++);

	}
}

// EFFECTS: sets up every pin for i/o
void setup_pins() {
	// Analog Current Sensor
	// TODO

	// Status LED 1 (output, default on)
	DDRA |= _BV(PORTA1);
//	PORTA |= _BV(PORTA1);

	// Status LED 2 (output, default on)
	DDRB |= _BV(PORTB2);
//	PORTB |= _BV(PORTB2);

	// Channel 1 Enable Button (input, pullup enabled)
	PORTA |= _BV(PORTA3);

	// Channel 2 Enable Button (input, pullup enabled)
	PORTA |= _BV(PORTA2);

	// Load Switch 1 (output, default off)
	DDRB |= _BV(PORTB0);
	PORTB |= _BV(PORTB0);

	// Load Switch 2 (output, default off)
	DDRB |= _BV(PORTB1);
	PORTB |= _BV(PORTB1);
}

// EFFECTS: toggles the output of channel 1
void output_1_toggle() {
	PORTB ^= _BV(PORTB0);
}

// EFFECTS: toggles the output of channel 2
void output_2_toggle() {
	PORTB ^= _BV(PORTB1);
}

// EFFECTS: turns on status led 1
void status_1_on() {
	PORTA |= _BV(PORTA1);
}

// EFFECTS: turns off status led 1
void status_1_off() {
	PORTA &= ~_BV(PORTA1);
}

// EFFECTS: turns on status led 2
void status_2_on() {
	PORTB |= _BV(PORTB2);
}

// EFFECTS: turns off status led 2
void status_2_off() {
	PORTB &= ~_BV(PORTB2);
}
