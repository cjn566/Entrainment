
#include "lights.h"


uint16_t chase_speed;
uint16_t chase_counter;
unsigned int timer;
unsigned long idle_time;
bool lights_ready;
bool pattern_initialized;
Animation strand_animation;
Animation rgb_animation;
LED_strand strand[2][12];
PWM_ControllerClass pwm = PWM_ControllerClass(NUM_TLC5974, pwm_clock, pwm_data, pwm_latch);

void PWM_init(){
	pwm.init();
};

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



void setIdleTime() {
	idle_time = millis() - info.in_use_time;
}


// TODO:
Animation chooseIdlePattern() {
	Animation ret;
	switch (idle_time)
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
	int val = PULSE_ANIM_PORTION(info.IBI[c]);
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

/*
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
*/



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
					if (s->next_index > 255) {
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

		noInterrupts();
		uint8_t chan = 0;
		for (uint8_t i = 0; i < 2; i++) {
			for (uint8_t j = 0; j < 12; j++) {
				pwm.setPWM(chan, strand[i][j].value);
				chan++;
			}
		}
		interrupts();

		// TODO: copy to pwm data structure

		// TURN OFF INTERRUPTS WHEN CHANGING DATA STRUCTURES INTERRUPTS USE

		// RGBs
		// TODO


		lights_ready = true;
	}
}



void doLights() {

	if (info.in_use) {
		for (uint8_t i = 0; i < 2; i++)
		{
			// Start a pulse animation
			if (info.start_pulse_animation[i])
			{
				resetStrands(i);
				//resetRGBs(i);
				startPulseAnimation(i);
				info.start_pulse_animation[i] = false;
			}
		}
	}

	// Idle animations
	else
	{
		// If a pattern hasn't been started and we have been idle long enough
		setIdleTime();
		if (!pattern_initialized && (idle_time > IDLE_DELAY)) {
			chooseIdlePattern();
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


void updateLEDs() {

	if (lights_ready) {
		pwm.write();
		// update_RGBs();
		lights_ready = false;
	}
	else {
		// TODO: interrupts were too fast.
	}
}