#ifndef __LIGHTS_H_INCLUDED__
#define __LIGHTS_H_INCLUDED__

#include <FastLED.h>
#include "waveforms.h"
#include "PWM_Controller.h"

typedef struct {
	bool active;
	uint8_t next_index;
	CRGB * rgb;
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
void startPulseAnimation(uint8_t c);
void setValues();

void testLights();
#endif 