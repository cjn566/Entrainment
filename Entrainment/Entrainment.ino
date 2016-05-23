


#include "Arduino.h"
#include "lights.h"
#include "TimerOne.h"
#include "fastLED.h"


// Settings
#define ENTRAIN_DIFF_MILLIS    350
#define MIN_ENTRAIN_COUNT   3
#define MIN_IBI   480


// GPIOS
#define P1_ready A0
#define P2_ready A1
#define P1_pulse 2
#define P2_pulse 3

// Timing
unsigned long pulse_time[2];

// Game states
bool pulse_read[2] = { false, false };
bool pulse_state[2] = { false, false };
bool finger_det[2] = { false, false };
GameInfo info;

void setup(void)
{
  Serial.begin(115200);
	Serial.print("B\n");

  // GPIO
    // Sensors
  pinMode(P1_ready, INPUT_PULLUP);
  pinMode(P2_ready, INPUT_PULLUP);
  pinMode(P1_pulse, INPUT_PULLUP);
  pinMode(P2_pulse, INPUT_PULLUP);
    // RGB

  // 60 Hz tick
  //Timer1.initialize(16666);     // 60 Hz update
  //Timer1.attachInterrupt(updateLEDs); // Display function

  // Lights
  lights_init(&info);

  // Debug


}

bool boop = true;
void loop() {
	//
	boop = !boop;
	digitalWrite(13, pulse_read[0]);
	testLights();
	delay(10);


	finger_det[0] = digitalRead(P1_ready);
	finger_det[1] = digitalRead(P2_ready);
	pulse_read[0] = digitalRead(P1_pulse);
	pulse_read[1] = digitalRead(P2_pulse);


	/*
	if (info.in_use = finger_det[1] || finger_det[0])
	{
		info.in_use_time = millis();
	}

  //check for entrainment and get pulse rate for both players
	for (uint8_t i = 0; i < 2; i++)
	{
		if (finger_det[i] && (pulse_state[i] != pulse_read[i])) {
			// Pulse starts
			if (pulse_read[i]) {
				pulse_state[i] = true;

				info.start_pulse_animation[i] = true;

				// Track pulse times
				unsigned long t = millis();
				info.IBI[i] = t - pulse_time[i];

				// Filter false positives
				if (info.IBI[i] > MIN_IBI) {
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
			// Pulse ends
			else {

				Serial.print("Entrained:\t\t"); if (i) { Serial.print("\t"); } Serial.println(info.entrainment);
				Serial.print("Entrain count:\t\t"); if (i) { Serial.print("\t"); } Serial.println(info.entrain_count);
				Serial.print("IBI:\t\t\t"); if (i) { Serial.print("\t"); } Serial.println(info.IBI[i]);
				Serial.print("Currently in use:\t"); if (i) { Serial.print("\t"); } Serial.println(info.in_use);
				Serial.print("Use Timestamp:\t\t"); if (i) { Serial.print("\t"); } Serial.println(info.in_use_time);
				Serial.println();

				pulse_state[i] = false;
			}
		}
		else
		{
			pulse_state[i] = false;
		}

	}*/
}

  //send signals to lights
  // DEBUG: doLights();