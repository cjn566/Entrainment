/***************************************************
This is an example for our Adafruit 24-channel PWM/LED driver

Pick one up today in the adafruit shop!
------> http://www.adafruit.com/products/1429

These drivers uses SPI to communicate, 3 pins are required to
interface: Data, Clock and Latch. The boards are chainable

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.
BSD license, all text above must be included in any redistribution
****************************************************/

#include <TimerOne.h>
#include "PWM_Controller.h"



uint8_t pulse_wave[255] = { 0,7,18,29,41,52,63,74,85,96,107,118,128,138,148,158,167,176,184,192,200,207,214,220,226,231,236,240,243,246,249,251,252,253,254,254,254,253,252,250,249,247,244,242,239,236,233,230,227,223,220,216,212,208,205,201,197,193,190,186,182,178,175,171,168,164,161,157,154,151,147,144,141,138,135,132,129,126,124,121,118,116,113,111,109,106,104,102,100,97,95,93,91,89,87,86,84,82,80,79,77,75,74,72,71,69,68,67,65,64,63,61,60,59,58,57,55,54,53,52,51,50,49,48,47,46,45,44,44,43,42,41,40,39,39,38,37,36,36,35,34,34,33,32,32,31,30,30,29,29,28,28,27,27,26,25,25,24,24,23,23,23,22,22,21,21,20,20,20,19,19,18,18,18,17,17,16,16,16,15,15,15,14,14,14,13,13,13,13,12,12,12,11,11,11,11,10,10,10,10,9,9,9,9,8,8,8,8,7,7,7,7,7,6,6,6,6,6,5,5,5,5,5,4,4,4,4,4,4,3,3,3,3,3,3,2,2,2,2,2,2,2,1,1,1,1,1,1,1,0 };


// How many boards do you have chained?
#define NUM_TLC5974 1

#define data   4
#define clock   5
#define latch   6
#define oe  -1  // set to -1 to not use the enable pin (its optional)

PWM_ControllerClass pwm = PWM_ControllerClass(NUM_TLC5974, clock, data, latch);


const int led = 3;  // the pin with a LED

void setup(void)
{
	pinMode(led, OUTPUT);
	//Timer1.initialize(150000);
	//Timer1.attachInterrupt(blinkLED); // blinkLED to run every 0.15 seconds
	Serial.begin(9600);
}


// The interrupt will blink the LED, and keep
// track of how many times it has blinked.
int ledState = LOW;
volatile unsigned long blinkCount = 0; // use volatile for shared variables

void blinkLED(void)
{
	if (ledState == LOW) {
		ledState = HIGH;
		blinkCount = blinkCount + 1;  // increase when LED turns on
	}
	else {
		ledState = LOW;
	}
	digitalWrite(led, ledState);
}

uint8_t i = 0;

// The main program will print the blink count
// to the Arduino Serial Monitor
void loop(void)
{
	analogWrite(led, pulse_wave[i]);
	i++;
	delay(4);
}