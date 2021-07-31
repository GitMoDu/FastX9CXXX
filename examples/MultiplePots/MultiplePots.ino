///
///	Example project, using multiple X9CXXX digital potentiometers.
/// 

#include <FastX9CXXX.h>

#define X9_1_CS_PIN 3
#define X9_1_UD_PIN 4
#define X9_1_INC_PIN 5


#define X9_2_CS_PIN 6
#define X9_2_UD_PIN 7
#define X9_2_INC_PIN 8

FastX9C104 Potentiometer1(X9_1_CS_PIN, X9_1_UD_PIN, X9_1_INC_PIN);
FastX9C503 Potentiometer2(X9_2_CS_PIN, X9_2_UD_PIN, X9_2_INC_PIN);


void setup() {
	Serial.begin(9600);
	Serial.println();

	Serial.print(F("X9C104 and X9C503 Digital Potentiometers setup..."));

	// Reset potentiometers to known position (Step == 0).
	Potentiometer1.Reset();
	Potentiometer2.Reset();

	Serial.println(F(" complete."));
}

void loop() {
	Potentiometer1.JumpToStep(random(X9CXXX::X9_STEPS));
	Potentiometer2.JumpToStep(random(X9CXXX::X9_STEPS));
	Serial.print(F("Potentiometers current resistance: "));
	Serial.print(Potentiometer1.GetEstimatedResistance(), DEC);
	Serial.print(F(" Ohm\t"));
	Serial.print(Potentiometer2.GetEstimatedResistance(), DEC);
	Serial.println(F(" Ohm"));
	delay(1000);    // wait for a second
}