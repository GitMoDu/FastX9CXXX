/*
Example project, using a X9CXXX digital potentiometer as a voltage divider,
and measuring the output with an analog pin.

*/

#include <FastX9CXXX.h>

#define X9_CS_PIN 3
#define X9_UD_PIN 4
#define X9_INC_PIN 5

#define WIPER_ANALOG_PIN A0

#define STEP_DELAY_MILLIS 20

FastX9C104 Potentiometer(X9_CS_PIN, X9_UD_PIN, X9_INC_PIN);

void setup() {
	Serial.begin(9600);
	Serial.println();
	//Serial.print(F("X9C104 Digital Potentiometer Sine Wave Test."));

	pinMode(WIPER_ANALOG_PIN, INPUT);

	// Reset potentiometer to known position (Step == 0).
	Potentiometer.Reset();

	// Plot Header.
	Serial.println(F("Step\tOutput"));
}

uint16_t Angle = 0;

void loop() {
	// Step in sine wave.
	const uint8_t step = (0.5 * sin(Angle / 5.0) * X9CXXX::X9_STEPS) + (X9CXXX::X9_STEPS / 2);
	Potentiometer.JumpToStep(step);

	const uint8_t result = map(analogRead(WIPER_ANALOG_PIN), 0, 1023, 0, X9CXXX::X9_STEPS);

	Serial.print(step);
	Serial.print('\t');
	Serial.print(result);
	Serial.print('\t');
	Serial.println();

	// Skip some steps to slow down sine wave.
	Angle++;

	delay(STEP_DELAY_MILLIS);    // wait for a bit before stepping.
}