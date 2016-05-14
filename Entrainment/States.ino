

enum State
{
	IDLE,
	PLAYING
};

uint8_t pin_P1_ready;
uint8_t pin_P2_ready;
uint8_t pin_P1_pulse;
uint8_t pin_P2_pulse;
State current_state;
State next_state;
bool player_ready[2];
bool found_pulse[2];

int entrain_count = 0;

void loop() {

	getInputs();

	switch (current_state)
	{
	case IDLE:

		doIdle();

		if (player_ready[0] || player_ready[1]) {
			next_state = PLAYING;
		}

		break;
	case PLAYING:

		doPlaying();


		if (!player_ready[0] && !player_ready[1]) {
			next_state = IDLE;
		}

		break;
	default:
		break;
	}

	current_state = next_state;
}

void getInputs() {
	player_ready[0] = digitalRead(pin_P1_ready);
	player_ready[1] = digitalRead(pin_P2_ready);
	found_pulse[0] = digitalRead(pin_P1_pulse);
	found_pulse[1] = digitalRead(pin_P2_pulse);
}