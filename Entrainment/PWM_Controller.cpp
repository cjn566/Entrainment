
#include "PWM_Controller.h"

PWM_ControllerClass::PWM_ControllerClass(uint8_t n, uint8_t c, uint8_t d, uint8_t l) {
	numdrivers = n;
	_clk = c;
	_dat = d;
	_lat = l;

	//pwmbuffer = (uint16_t *)calloc(2, 24*n);
	pwmbuffer = (uint8_t *)malloc(24 * n);
	memset(pwmbuffer, 0, 24 * n);
}


void PWM_ControllerClass::write(void) {
	digitalWrite(_lat, LOW);
	// 24 channels per TLC5974
	for (int16_t c = 24 * numdrivers - 1; c >= 0; c--) {
		// 8 bits per channel, send MSB first
		for (int8_t b = 7; b >= 0; b--) {
			digitalWrite(_clk, LOW);

			if (pwmbuffer[c] & (1 << b))
				digitalWrite(_dat, HIGH);
			else
				digitalWrite(_dat, LOW);

			digitalWrite(_clk, HIGH);
		}
		// Remining LSB 4 bits are 0
		digitalWrite(_dat, LOW);
		for (int8_t b = 3; b >= 0; b--) {
			digitalWrite(_clk, LOW);
			digitalWrite(_clk, HIGH);
		}
	}
	digitalWrite(_clk, LOW);

	digitalWrite(_lat, HIGH);
	digitalWrite(_lat, LOW);
}


void PWM_ControllerClass::setPWM(uint8_t chan, uint8_t pwm) {
	pwmbuffer[chan] = pwm;
}


boolean PWM_ControllerClass::init()
{
	if (!pwmbuffer) return false;

	pinMode(_clk, OUTPUT);
	pinMode(_dat, OUTPUT);
	pinMode(_lat, OUTPUT);
	digitalWrite(_lat, LOW);

	return true;
}



