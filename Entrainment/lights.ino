#include "lights.h"

#define PULSE_ANIM_PORTION (4/5)
#define IDLE_DELAY 10000

enum Animation
{
	UNSET,

	S_ON_PULSE,
	S_CHASE_2,
	S_ALL_SINE,

	R_ON_PULSE,
	R_ALL_SINE
};

int speed_map[2][20] = 
{ 
	{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20 },
	{ 4267,2133,1422,1067,853,711,610,533,474,427,388,356,328,305,284,267,251,237,225,213 } 
};

uint8_t chase_sequence[24][2];// TODO = {};
uint16_t chase_speed;
uint16_t chase_counter;
uint32_t timer;
unsigned long idle_time;

bool lights_ready = false;
bool pattern_initialized = false;
bool start_pulse_animation[2] = {false, false};
Animation strand_animation = UNSET;
Animation rgb_animation = UNSET;

typedef struct {
	bool repeat;
	bool active;
	uint8_t value;
	uint8_t waveform;
	uint8_t next_index;
	uint8_t animation_speed;
} LED_strand;

LED_strand strand[2][12];


void doLights() {

	if (in_use) {
		for (uint8_t i = 0; i < 2; i++)
		{
			// Start a pulse animation
			if (start_pulse_animation[i])
			{
				resetStrands(i);
				resetRGBs(i);
				startPulseAnimation(i);
				start_pulse_animation[i] = false;
			}
		}
	}

	// Idle animations
	else
	{
		// If a pattern hasn't been started and we have been idle long enough
		if (!pattern_initialized && (millis() - in_use_time > IDLE_DELAY)) {
			chooseIdlePattern()
		}
	}


	/*
	switch (pattern_strands)
	{
	case UNSET:
		resetStrands(0);
		resetStrands(1);

		// Set the new pattern based on game state
		switch (current_state)
		{
		case IDLE:
			initStrandsFor(CHASE_2);

			break;
		case PLAYING:
			pattern_podium = ON_PULSE;
			break;
		default:
			break;
		}
		break;

	case CHASE_2:

		break;

	case ON_PULSE:
		break;

	default:
		break;
	}
	*/
}

setIdleTime() {
	idle_time = millis() - in_use_time;
}

StrandAnimation chooseIdlePattern() {
	StrandAnimation ret;
	switch (switch_on)
	{
	default:
		break;
	}
	return ret;
}

void startPulseAnimation(uint8_t c) {
	//Strands
	uint8_t speed = calcPulseSpeedOnIBI(c);
	LED_strand *s;
	for (uint8_t j = 0; j < 12; j++) {
		s = &strand[c][j];
		s->active = true;
		s->waveform = PULSE;
		s->animation_speed = speed;
	}

	//TODO: RGBs
}

int calcPulseSpeedOnIBI(uint8_t c) {
	int val = IBI[c] * PULSE_ANIM_PORTION;
	int ret = 8;
	for (uint8_t i = 19; i >= 0; i--)
	{
		if (val < speed_map[1][i]) {
			ret = speed_map[0][i];
			break;
		}
	}
	return ret;
}

void initStrandsFor(LED_Pattern pattern) {
	pattern_strands = pattern;

	switch (pattern_strands)
	{
	case ON_PULSE:
		break;
	case CHASE_2:
		break;

	case ALL_SINE:
		LED_strand *s;
		for (uint8_t i = 0; i < 2; i++) {
			for (uint8_t j = 0; j < 12; j++) {
				s = &strand[i][j];

				s->active = true;
				s->repeat = true;
				s->waveform = SINE;
			}
		}
		break;

	default:
		break;
	}
}

void resetStrands(uint8_t c) {
	LED_strand *s;
	for (uint8_t j = 0; j < 12; j++) {
		s = &strand[c][j];
		s->active = false;
		s->repeat = false;
		s->next_index = 0;
		s->value = 0;
	}
}

void setValues() {
	if (!lights_ready) {

		// Strands
		LED_strand *s;
		for (uint8_t i = 0; i < 2; i++) {
			for (uint8_t j = 0; j < 12; j++) {
				s = &strand[i][j];
				if (s->active) {
					s->value = animations[s->waveform][s->next_index];
					s->next_index += s->animation_speed;
					if(s->next_index > 255){
						if (s->repeat) {
							s->next_index = 0;
						}
						else {
							s->active = false;
						}
						s->value = 0;
					}
				}
			}
		}
		// TODO: copy to pwm data structure

		// RGBs
		// TODO


		lights_ready = true;
	}
}

void ISR(){
	if (lights_ready) {
		update_PWMs();
		update_RGBs();
		lights_ready = false;
	}
	else {
		// TODO: interrupts were too fast.
	}
}