enum LED_Pattern
{
	UNSET,
	ON_PULSE,
	CHASE_2,
	FULL_THROB
};

uint8_t chase_sequence[24][2];// TODO = {};
uint16_t chase_speed;
uint16_t chase_counter;

bool lights_ready;
LED_Pattern pattern_strands = UNSET;
LED_Pattern pattern_podium = UNSET;
uint8_t animation_speed;
uint8_t strand_waveform = SINE;

typedef struct {
	bool active;
	uint8_t value;
	uint8_t next_index;
	uint8_t counter;
} LED_strand;

LED_strand strand[2][12];


void doPWM() {
	switch (pattern_strands)
	{
	case UNSET:
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 12; j++) {
				strand[i][j].counter = 0;
				strand[i][j].value = 0;
			}
		}

		// Set the new pattern based on game state
		switch (current_state)
		{
		case IDLE:
			pattern_podium = CHASE_2;
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

void setValues() {
	if (!lights_ready) {
		LED_strand *s;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 12; j++) {
				s = &strand[i][j];
				if (s->active) {
					s->counter--;
					if (s->counter == 0) {
						s->value = animations[strand_waveform][s->next_index];
						s->counter = animation_speed;
						s->next_index++;
						if(s->next_index > 255){
							s->value = 0;
							s->active = false;
						}
					}
				}
			}
		}
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