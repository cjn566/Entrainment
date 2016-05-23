
#include "lights.h"






bool lights_ready = false;
GameInfo * GM;
LightElement strand[NUM_STRANDS];
LightElement rgb[1];// NUM_RGBS];
CRGB leds[NUM_RGBS];
PWM_ControllerClass pwm = PWM_ControllerClass(NUM_TLC5974, pwm_clock, pwm_data, pwm_latch);
bool pattern_initialized = false;
unsigned long idle_time = 0;

void lights_init(GameInfo * info) {

	Serial.print("A\n");
	GM = info;
	pwm.init();
	FastLED.addLeds<WS2812B, RGB_data, RGB>(leds, NUM_RGBS);
	for (int i = 0; i < NUM_RGBS; i++)
	{
		rgb[i].rgb = &leds[i];
	}
	resetLights(strand, NUM_STRANDS);
	resetLights(rgb, NUM_RGBS);

};

void testLights() {
	for (int i = 0; i < NUM_RGBS_PP; i++)
	{
		leds[i] = CRGB::Amethyst;
		leds[i + NUM_RGBS_PP] = CRGB::Crimson;
	}
	FastLED.show();
	Serial.print("Show\n");
}

void resetLights(LightElement *e, uint8_t count) {
	for (uint8_t j = 0; j < count; j++) {
		e[j].active = false;
		e[j].next_index = 0;
		*e[j].rgb = CRGB::Black;
	}
}

void setIdleTime() {
	idle_time = millis() - GM->in_use_time;
}


void startPulseAnimation(uint8_t c) {
	// Strands
	LightElement *s = &strand[NUM_STRANDS_PP*c];
	for (uint8_t j = 0; j < NUM_STRANDS_PP; j++) {
		s[j].active = true;
		s[j].next_index = 0;
	}
	// RGBs
	s = &rgb[NUM_RGBS_PP*c];
	for (uint8_t j = 0; j < NUM_RGBS_PP; j++) {
		s[j].active = true;
		s[j].next_index = 0;
	}
}


	/*
// TODO:
void initStrandsFor(int pattern) {
	strand_animation = pattern;
	Serial.println("Init strands\nPattern: ");
	Serial.println(strand_animation);

	switch (strand_animation)
	{
	case S_ON_PULSE:
	break;
	case S_CHASE_2:
	break;

	case S_ALL_SINE:
	Serial.println("Init strands for SINE");
	LightElement *s;
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
/*
*/


/*
void setValues() {
	if (!lights_ready) {

		// Strands
		LightElement *s;
		for (uint8_t j = 0; j < NUM_STRANDS; j++) {
			s = &strand[j];
			if (s->active) {
				if (s->next_index) {
					s->next_index ++;
					if (s->next_index > 255) {
						s->next_index = 0;
					}
				}
				else
				{
					if (s->delay_ctr) {
						s->delay_ctr--;
					}
					else
					{
						if (s->) {
							s->next_index = 1;
							s->delay_ctr = s->delay_rst;
						}
						else {
							s->active = false;
						}
						s->delay_ctr = s->delay_rst;
					}
				}
			}
		}

		// RGBs
		for (uint8_t j = 0; j < NUM_RGBS; j++) {
			s = &rgb[j];
			if (s->active) {
				s->rgb_val.v = waves[s->waveform][s->next_index];
				s->next_index += s->animation_speed;
				if (s->next_index > 255) {
					if (s->repeat) {
						s->next_index = 0;
					}
					else {
						s->active = false;
					}
					s->brightness = CRGB::Black;
				}
			}
		}

		// TURN OFF INTERRUPTS WHEN CHANGING DATA STRUCTURES INTERRUPTS USE
		// Copy values from light structs to bus data
		noInterrupts();
		for (uint8_t j = 0; j < NUM_STRANDS; j++) {
			pwm.setPWM(j, strand[j].brightness);
		}
		for (uint8_t j = 0; j < NUM_RGBS; j++) {
			leds[j] = rgb[j].rgb_val;
		}
		interrupts();

		// Let 60Hz tick know the lights are ready to go
		lights_ready = true;
	}
}


// TODO
void doLights() {

	// Pulse animation
	if (GM->in_use) {
		for (uint8_t i = 0; i < 2; i++)
		{
			// Start a pulse animation
			if (GM->start_pulse_animation[i])
			{
				resetLights(&strand[i*NUM_STRANDS_PP], NUM_STRANDS_PP);
				resetLights(&rgb[i*NUM_RGBS_PP], NUM_RGBS_PP);
				startPulseAnimation(i);
				GM->start_pulse_animation[i] = false;
			}
		}
	}

	// Idle animations
	else
	{
		// If a pattern hasn't been started and we have been idle long enough
		setIdleTime();
		if (!pattern_initialized && (idle_time > IDLE_DELAY)) {
			initStrandsFor(chooseIdlePattern());
		}
	}



	pwm.write();
	FastLED.show();

* /
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
}
	*/