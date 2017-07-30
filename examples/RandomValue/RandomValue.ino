/*
Example project to control a X9CXXX digital potentiometer

https://github.com/GitMoDu/FastX9CXXX

modified 30 Aug 2017
by MoDu
*/

#include <FastX9CXXX.h>

#define X9_CS_PIN 3
#define X9_UD_PIN 4
#define X9_INC_PIN 5

FastX9CXXX Potentiometer;

void setup() {
	Serial.begin(9600);
	Serial.println();
	Serial.print(F("X9C104 Digital Potentiometer setup..."));
	randomSeed(analogRead(0));
	//Potentiometer.Begin();
	Potentiometer.Setup(X9_CS_PIN, X9_UD_PIN, X9_INC_PIN);
	Serial.println(F(" complete."));

}

void loop() {
	Potentiometer.JumpToStep(random(100));
	Serial.print(F("Potentiometer current step: "));
	Serial.print(Potentiometer.GetStep(), DEC);
	Serial.println();
	delay(1000);    // wait for a second
}