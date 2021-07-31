///
///	Example project, using a X9CX104 digital potentiometer as a voltage divider
///	and measuring the output with an analog pin.
/// 

#include <FastX9CXXX.h>

#define X9_CS_PIN 3
#define X9_UD_PIN 4
#define X9_INC_PIN 5

#define WIPER_ANALOG_PIN A0

#define STEP_DELAY_MILLIS 20

FastX9C104 Potentiometer(X9_CS_PIN, X9_UD_PIN, X9_INC_PIN);

uint16_t Angle = 0;

void setup() {
	Serial.begin(9600);
	Serial.println();

	//Serial.print(F("X9C104 Digital Potentiometer Sine Wave Test."));

	// Setup output read, analog pin.
	pinMode(WIPER_ANALOG_PIN, INPUT);

	// Reset potentiometer to known position (Step == 0).
	Potentiometer.Reset();

	// Plot Header.
	Serial.println(F("Step\tOutput"));
}


void loop() {
	// Step from sine wave, ranged to available steps.
	const uint8_t step = (0.5 * sin(Angle / 5.0) * X9CXXX::X9_STEPS) + (X9CXXX::X9_STEPS / 2);
	Potentiometer.JumpToStep(step);

	// Get the estimated step from the mapped output voltage.
	const uint8_t result = map(analogRead(WIPER_ANALOG_PIN), 0, 1023, 0, X9CXXX::X9_STEPS);

	// Plot the sample.
	Serial.print(step);
	Serial.print('\t');
	Serial.print(result);
	Serial.print('\t');
	Serial.println();


	Angle++;

	delay(STEP_DELAY_MILLIS);    // wait for a bit before stepping.
}