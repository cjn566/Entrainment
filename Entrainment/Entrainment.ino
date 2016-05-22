


#include "Arduino.h"
#include "lights.h"
#include "TimerOne.h"
#include "fastLED.h"


// Settings
#define ENTRAIN_DIFF_MILLIS		100
#define MIN_ENTRAIN_COUNT		3


// Sensors
#define P1_ready 1
#define P2_ready 1
#define P1_pulse 1
#define P2_pulse 1

// Timing
unsigned long pulse_time[2];

// Game states
bool pulse_read[2] = { false, false };
bool pulse_state[2] = { false, false };
GameInfo info;

void setup(void)
{
	// GPIO
		// Sensors
	pinMode(P1_ready, INPUT);
	pinMode(P2_ready, INPUT);
	pinMode(P1_pulse, INPUT);
	pinMode(P2_pulse, INPUT);
		// RGB

	// 60 Hz tick
	Timer1.initialize(16666);			// 60 Hz update
	Timer1.attachInterrupt(updateLEDs);	// Display function

	// Lights
	lights_init(&info);

	// Debug
	Serial.begin(9600);
}

void loop() {

	// Check if anyone is there and grab the timestamp if they are
	info.in_use = digitalRead(P1_ready) || digitalRead(P2_ready);

	if (info.in_use)
	{
		info.in_use_time = millis();

		// Check pulses
		pulse_read[0] = digitalRead(P1_pulse);
		pulse_read[1] = digitalRead(P2_pulse);


		//check for entrainment and get pulse rate for both players
		for (uint8_t i = 0; i < 2; i++)
		{
			// End of pulse
			if (pulse_state[i] && !pulse_read[i]) {
				pulse_state[i] = false;
			}

			// pulse begins
			else if (!pulse_state[i] && pulse_read[i])
			{
				pulse_state[i] = true;
				info.start_pulse_animation[i] = true;

				// Track pulse times
				unsigned long t = millis();
				info.IBI[i] = t - pulse_time[i];
				pulse_time[i] = t;

				// Check for entrainment
				signed long diff = pulse_time[1] - pulse_time[0];
				diff = abs(diff);

				// Entrainment occurs
				if (diff < ENTRAIN_DIFF_MILLIS) {
					info.entrain_count++;

					// Sequential entrainment
					if (info.entrain_count >= MIN_ENTRAIN_COUNT) {
						info.entrainment = true;
					}
				}
				else
				{
					info.entrain_count = 0;
					info.entrainment = false;
				}
			}
		}
	}

	//send signals to lights
	doLights();
}




