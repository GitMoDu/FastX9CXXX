///
/// Example project to control a X9C102 digital potentiometer.
///

#include <FastX9CXXX.h>

#define X9_CS_PIN 3
#define X9_UD_PIN 4
#define X9_INC_PIN 5

FastX9C102 Potentiometer;

void setup() {
	Serial.begin(9600);
	Serial.println();
	Serial.print(F("X9C102 Digital Potentiometer setup..."));

	Potentiometer.Setup(X9_CS_PIN, X9_UD_PIN, X9_INC_PIN);

	// Reset potentiometer to known position (Step == 0).
	Potentiometer.Reset();

	Serial.println(F(" complete."));
}

void loop() {
	Potentiometer.JumpToStep(random(X9CXXX::X9_STEPS));
	Serial.print(F("Potentiometer current resistance: "));
	Serial.print(Potentiometer.GetEstimatedResistance(), DEC);
	Serial.println(F(" Ohm"));
	delay(1000);    // wait for a second
}