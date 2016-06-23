


#include "Arduino.h"
#include "TimerOne.h"
#include "fastLED.h"
#include "PWM_Controller.h"
#include "waveforms.h"


// Settings
#define ENTRAIN_DIFF_MILLIS    80
#define MIN_ENTRAIN_COUNT   3
#define MIN_IBI   400
#define MAX_IBI   1500
#define SOUND_PULSE_DURATION   50


// GPIOS
#define P1_ready A0
#define P2_ready A1
#define P1_pulse 2
#define P2_pulse 3
#define P1_out 5
#define P2_out 6


// Settings
#define IDLE_DELAY		10000
#define NUM_RGBS		192
#define NUM_RGBS_PP		(NUM_RGBS/2)
#define NUM_STRANDS		12
#define NUM_STRANDS_PP	(NUM_STRANDS/2)
#define MAX_ENTRAIN		6
#define ENT_SAT_D		255 / MAX_ENTRAIN

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


// TESTING
uint8_t var_val;
#define POT_PIN 4

// Inlines
#define WIND(a,b,c) (b>=a? ((b <= c)? b:c):(a))

// Game states
int out_pins[2] = { P1_out, P2_out };
bool pulse_read[2] = { false, false };
bool pulse_state[2] = { false, false };
bool finger_det[2] = { false, false };
bool in_use = false; 
bool lights_ready = false;
bool go_lights = false;
bool entrainment = false;
bool start_entrain = false;
bool start_pulse_animation[2]; 
bool go_pulse[2] = { false, false };
bool idle = true;
bool reset = false;
int entrain_count;
int pulse_wave_idx[2] = { 0,0 };
unsigned long in_use_time;
unsigned long entrain_time;
unsigned long prev_entrain_time;
unsigned long sound_pulse_active[2] = {0,0};
unsigned long idle_time = IDLE_DELAY;
unsigned long IBI[2]; 
uint8_t pulse_val;
uint8_t pulse_speed = 6; 
uint8_t idle_speed = 3;
uint8_t hue[2];
uint16_t idle_hue = 0;
uint8_t idle_idx = 0;

CRGB leds[NUM_RGBS];
PWM_ControllerClass pwm = PWM_ControllerClass(NUM_TLC5974, pwm_clock, pwm_data, pwm_latch);

// Timing
unsigned long pulse_time[2];
void setup(void)
{
	//Serial.begin(115200);
	pwm.init();
	FastLED.addLeds<WS2812B, RGB_data, RGB>(leds, NUM_RGBS);

  // GPIO
    // Sensors
  pinMode(P1_ready, INPUT_PULLUP);
  pinMode(P2_ready, INPUT_PULLUP);
  pinMode(P1_pulse, INPUT_PULLUP); 
  pinMode(P2_pulse, INPUT_PULLUP);
  pinMode(P1_out, OUTPUT);
  pinMode(P2_out, OUTPUT);
  digitalWrite(out_pins[0], LOW);
  digitalWrite(out_pins[1], LOW);

  attachInterrupt(digitalPinToInterrupt(P1_pulse), pulse0, RISING);
  attachInterrupt(digitalPinToInterrupt(P2_pulse), pulse1, RISING);
    // RGB

  // 60 Hz tick
  Timer1.initialize(16666);     // 60 Hz update
  Timer1.attachInterrupt(tick); // Display function

  // Lights
  resetLights();

  // Debug
}

void pulse0() {
	if (finger_det[0]) pulse(0);
}

void pulse1(){
	if(finger_det[1]) pulse(1);
}

void pulse(uint8_t pin) {
	unsigned long ts_pulse = millis();
	unsigned long diff_pulse = ts_pulse - pulse_time[pin];

	// Filter false positives and first beats
	if (diff_pulse > MIN_IBI) {
		pulse_time[pin] = ts_pulse;
		IBI[pin] = diff_pulse;



		digitalWrite(out_pins[pin], HIGH);
		sound_pulse_active[pin] = ts_pulse;

		if (go_pulse[pin]) {
			pulse_wave_idx[pin] = 0;
		}
		else
		{
			go_pulse[pin] = true;
		}	

		//DEBUG
		Serial.println(var_val);


		// Entrainment expires
		if (entrainment && (pulse_time[pin] - entrain_time > MAX_IBI)) {
			entrainment = false;
			entrain_count = 0;
		}

		// Check for entrainment
		signed long diff = pulse_time[1] - pulse_time[0];
		diff = abs(diff);

		// Entrainment occurs
		if (diff < var_val)
		{
			entrain_count++;
			entrain_time = pulse_time[pin];
			// Sequential entrainment
			if (entrain_count >= MIN_ENTRAIN_COUNT) {
				entrainment = true;
			}
		}
	}
}

void loop() {
	finger_det[0] = digitalRead(P1_ready);
	finger_det[1] = digitalRead(P2_ready);
	if (in_use = finger_det[1] || finger_det[0]) {
		in_use_time = millis();
		if (idle) {
			resetLights();
			idle = false;
		}
		if (!lights_ready) {
			// DO PULsE SHIT

			for (uint8_t i = 0; i < 2; i++)
			{
				if (go_pulse[i]) {
					reset = false;

					// PWM
					for (uint8_t j = 0; j < 12; j++)
					{
						pwm.setPWM(j+(12*i), waves[pulse_wave_idx[i]]);
					}
					// IA RGB

					uint8_t sat = entrainment ? (255 - (WIND(0, (entrain_count - MIN_ENTRAIN_COUNT) * ENT_SAT_D, 255))) : 255;

					for (uint8_t j = 0; j < 91; j++)
					{
						leds[j + (91 * i)] = CHSV(96, sat, waves[pulse_wave_idx[i]]);
					}

					// Pulse is done, clear everything
					if ((pulse_wave_idx[i] = pulse_wave_idx[i] + pulse_speed) >= 255) {
						pulse_wave_idx[i] = 0;
						go_pulse[i] = false;
						resetLights();
						reset = true;

					}
				}
			}


			lights_ready = true;
		}
	}
	else
	{
		if (!reset) {
			resetLights();
			reset = true;
		}
		if (!idle && (millis() - in_use_time) >= IDLE_DELAY) {
			idle = true;
		}
	}

	if(idle){
		if (!lights_ready) {
			reset = false;

			// PWM
			for (uint8_t j = 0; j < 24; j++)
			{
				pwm.setPWM(j, waves[ uint8_t(10*j + idle_idx) ] );
			}
			idle_idx += idle_speed;
			// IA RGB

			uint8_t sat = entrainment ? (255 - (WIND(0, (entrain_count - MIN_ENTRAIN_COUNT) * ENT_SAT_D, 255))) : 255;

			for (uint8_t j = 0; j < 182; j++)
			{
				leds[j] = CHSV((idle_hue >> 2), 255, 255);
			}
			idle_hue++;


			lights_ready = true;
		}
	}


	// Timming gate for light effects
	if (go_lights) {
		pwm.write();
		FastLED.show();
		go_lights = false;
		lights_ready = false;
	}


	// End of sound pulse
	for (int i = 0; i < 2; i++)
	{
		if (sound_pulse_active[i]) {
			if (millis() - sound_pulse_active[i] > SOUND_PULSE_DURATION) {
				digitalWrite(out_pins[i], LOW);
				sound_pulse_active[i] = 0;
			}
		}
	}

	var_val = analogRead(POT_PIN) + 50;
}

void resetLights() {
	// PWM
	for (uint8_t j = 0; j < 24; j++)
	{
		pwm.setPWM(j, 0);
	}
	// IA RGB
	for (uint8_t j = 0; j < 182; j++)
	{
		leds[j] = CRGB::Black;
	}
}

// Serial.print("Entrained:\t\t"); if (i) { Serial.print("\t"); } Serial.println(info.entrainment);
// Serial.print("Entrain count:\t\t"); if (i) { Serial.print("\t"); } Serial.println(info.entrain_count);
// Serial.print("IBI:\t\t\t"); if (i) { Serial.print("\t"); } Serial.println(info.IBI[i]);
// Serial.print("Currently in use:\t"); if (i) { Serial.print("\t"); } Serial.println(info.in_use);
// Serial.print("Use Timestamp:\t\t"); if (i) { Serial.print("\t"); } Serial.println(info.in_use_time);
// Serial.println();

void tick() {
	go_lights = true;
}

