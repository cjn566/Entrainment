#define CLOCK_SPEED (16*1000000)

int getMatchValue(uint16_t freq, uint16_t p) {
	return CLOCK_SPEED / (freq * p) - 1;
}

void timer0_init(uint16_t f) {
	uint16_t prescaler = 1;
	uint16_t match = (CLOCK_SPEED / f)-1;

	if (match > 255) {
		while (match /= 8 > 255) {
			prescaler *= 8;
		}
	}

	cli();//stop interrupts

	
	TCCR0A = 0;					// set entire TCCR2A register to 0
	TCCR0B = 0;					// same for TCCR2B
	TCNT0 = 0;					// initialize counter value to 0
								// set compare match register for 2khz increments
	OCR0A = match;				// = (16*10^6) / (2000*64) - 1 (must be <256)
	TCCR0A |= (1 << WGM01);		// turn on CTC mode
	TCCR0B |= (1 << CS01) | (1 << CS00);	// Set CS01 and CS00 bits for 64 prescaler
	TIMSK0 |= (1 << OCIE0A);	// enable timer compare interrupt


	sei();//allow interrupts
}

ISR(TIMER0_COMPA_vect) { timer0(); }