#ifndef __ENTRAIN_H_INCLUDED__
#define __ENTRAIN_H_INCLUDED__

#include <Arduino.h>

struct {
	bool in_use;
	bool entrainment;
	bool start_pulse_animation[2];
	uint8_t entrain_count;
	unsigned long in_use_time;
	unsigned long IBI[2];
} info;

#endif