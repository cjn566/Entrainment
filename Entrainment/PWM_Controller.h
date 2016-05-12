// PWM_Controller.h

#ifndef _PWM_CONTROLLER_h
#define _PWM_CONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class PWM_ControllerClass
{
public:
	PWM_ControllerClass(uint8_t n, uint8_t c, uint8_t d, uint8_t l);

	boolean init(void);

	void setPWM(uint8_t chan, uint8_t pwm);
	void write(void);


private:
	uint8_t *pwmbuffer;

	uint8_t numdrivers;
	uint8_t _clk, _dat, _lat;
};

extern PWM_ControllerClass PWM_Controller;

#endif

