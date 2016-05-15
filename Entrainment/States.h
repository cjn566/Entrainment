

/*
void loop() {

	// Check if anyone is there and grab the timestamp if they are
	in_use = digitalRead(pin_P1_ready) || digitalRead(pin_P2_ready);

	if (in_use)
	{
		in_use_time = millis();

		// Check pulses
		pulse_read[0] = digitalRead(pin_P1_pulse);
		pulse_read[1] = digitalRead(pin_P2_pulse);


		//check for entrainment and get pulse rate for both players
		for (uint8_t i = 0; i < 2; i++)
		{
			// End of pulse
			if (pulse_state[i] && !pulse_read[i]) {
				pulse_state[i] = false;
			}

			// pulse begins
			else if(!pulse_state[i] && pulse_read[i])
			{
				pulse_state[i] = true;
				start_pulse_animation[i] = true;
				
				// Track pulse times
				unsigned long t = millis();
				IBI[i] = t - pulse_time[i];
				pulse_time[i] = t;
				
				// Check for entrainment
				signed long diff = pulse_time[1] - pulse_time[0];
				diff = abs(diff);

				// Entrainment occurs
				if (diff < ENTRAIN_DIFF_MILLIS) {
					entrain_count++;

					// Sequential entrainment
					if (entrain_count >= MIN_ENTRAIN_COUNT) {
						entrainment = true;
					}
				}
				else
				{
					entrain_count = 0;
					entrainment = false;
				}
			}
		}
	}

	//send signals to lights
	doLights();
}
*/