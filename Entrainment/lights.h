#ifndef __LIGHTS_H_INCLUDED__
#define __LIGHTS_H_INCLUDED__


#include "waveforms.h"
#include "GameInfo.h"
#include "PWM_Controller.h"

// Settings
#define PULSE_ANIM_PORTION(a)	(a*4)/5
#define IDLE_DELAY				10000

// GPIO Pins
// PWM
#define pwm_data    4
#define pwm_clock   5
#define pwm_latch	6
#define pwm_oe		-1  // set to -1 to not use the enable pin (its optional)
#define NUM_TLC5974 1

enum Animation
{
	UNSET,

	S_ON_PULSE,
	S_CHASE_2,
	S_ALL_SINE,

	R_ON_PULSE,
	R_ALL_SINE
};

const int speed_map[2][20] = 
{ 
	{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20 },
	{ 4267,2133,1422,1067,853,711,610,533,474,427,388,356,328,305,284,267,251,237,225,213 } 
};

const uint8_t chase_sequence[24][2] = {0};

typedef struct {
	bool repeat;
	bool active;
	uint8_t value;
	uint8_t waveform;
	uint8_t next_index;
	uint8_t animation_speed;
} LED_strand;

void PWM_init();
void doLights();
void updateLEDs();
void resetStrands(uint8_t c);
void setIdleTime();
Animation chooseIdlePattern();
void startPulseAnimation(uint8_t c);
int calcPulseSpeedOnIBI(uint8_t c);
void setValues();


#endif 