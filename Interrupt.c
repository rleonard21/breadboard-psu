// Interrupt.c
// Created by Robert Leonard on 10/27/19.
//

#include "Interrupt.h"

#include <avr/interrupt.h>
#include "Debounce.h"

// EFFECTS: inits the system tick
void Interrupt_init() {
	TIMSK0 |= _BV(TOIE0); // Enable timer0 overflow interrupt vector
	TCCR0A |= _BV(WGM01) | _BV(WGM00); // fast PWM
	TCCR0B |= _BV(CS10);
}

// EFFECTS: executes on overflow of timer0 (3.9kHz)
ISR(TIM0_OVF_vect) {
	debounce();
}
