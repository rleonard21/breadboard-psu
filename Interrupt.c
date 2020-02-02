// Interrupt.c
// Created by Robert Leonard on 10/27/19.
//

#include "Interrupt.h"

#include <avr/interrupt.h>
#include "Debounce.h"

// EFFECTS: inits the system tick
void Interrupt_init() {
	TIMSK1 |= _BV(TOIE1); // Enable timer0 overflow interrupt vector
	TCCR1A |= _BV(WGM11) | _BV(WGM10); // fast PWM
	TCCR1B |= _BV(CS10);
	sei();
}

// EFFECTS: executes on overflow of timer0 (3.9kHz)
ISR(TIM1_OVF_vect) {
	debounce();
}
