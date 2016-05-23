#ifndef __LIGHTS_H_INCLUDED__
#define __LIGHTS_H_INCLUDED__

#include <FastLED.h>
#include "waveforms.h"
#include "PWM_Controller.h"

// Settings
#define PULSE_ANIM_PORTION(a)	(a*4)/5
#define IDLE_DELAY				10000
#define NUM_RGBS		192
#define NUM_RGBS_PP		(NUM_RGBS/2)
#define NUM_STRANDS		12
#define NUM_STRANDS_PP	(NUM_STRANDS/2)

// Misc.
#define PLAYER_1				0
#define PLAYER_2				1

// GPIO Pins
// PWM
#define pwm_data    11
#define pwm_clock   13
#define pwm_latch	10
#define pwm_oe		-1  // set to -1 to not use the enable pin (its optional)
#define NUM_TLC5974 1
// RGB
#define RGB_data 9
// Sound
#define SOUND_1 5
#define SOUND_2 6

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

const uint8_t chase_sequence[24][2] = { 0 };

typedef struct {
	bool repeat;
	bool active;
	uint8_t waveform;
	uint8_t next_index;
	uint8_t animation_speed;
	uint8_t pwm_val;
	uint8_t delay_ctr;
	uint8_t delay_rst;
	CHSV rgb_val;
} LightElement;

typedef struct {
	bool in_use;
	bool entrainment;
	bool start_pulse_animation[2];
	int entrain_count;
	unsigned long in_use_time;
	unsigned long IBI[2];
} GameInfo;

void lights_init(GameInfo * info);
void doLights();
void updateLEDs();
void resetLights(LightElement * e, uint8_t c);
void setIdleTime();
Animation chooseIdlePattern();
void startPulseAnimation(uint8_t c);
int calcPulseSpeedOnIBI(uint8_t c);
void setValues();

void testLights();
#endif 