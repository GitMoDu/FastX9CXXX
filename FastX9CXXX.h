/// 
///  Created for personal use, use it at your own risk and benefit.
///  https://github.com/GitMoDu/FastX9CXXX
///  Depends on Fast for IO https://github.com/GitMoDu/Fast
/// 

#ifndef _FASTX9CXXX_h
#define _FASTX9CXXX_h

//#define USE_PIN_DEFINITIONS

#include <Arduino.h>
#include <Fast.h>
#ifdef USE_PIN_DEFINITIONS
#include "PinDefinitions.h"
#endif // USE_PIN_DEFINITIONS

#define X9_STEPS 100

class FastX9CXXX
{
private:
#define NCS_TO_NINC_SETUP 1
#define NINC_HIGH_TO_UND_CHANGE 1
#define UND_TO_NINC_SETUP 3
#define NINC_LOW_PERIOD 1
#define NINC_HIGH_PERIOD 1
#define NINC_INACTIVE_TO_NCS_INACTIVE 1
#define NCS_DESELECT_TIME_STORE 20000
#define NCS_DESELECT_TIME_NO_STORE 1
#define NINC_TO_VWRW_CHANGE 100
#define NINC_CYCLE_TIME 2
#define POWER_UP_TO_WIPER_STABLE 500


	FastOut PinCS, PinUD;
	FastShifter PinINC;

	uint8_t CurrentStep = 0;

public:
#ifdef USE_PIN_DEFINITIONS
	bool Begin();
#endif // USE_PIN_DEFINITIONS
	bool Setup(const byte csPin, const byte udPin, const byte incPin);

	void Reset();
	void JumpToStep(uint8_t step);

	void Down(bool store = true);
	void Up(bool store = true);

	uint8_t GetStep();

	void Store();
	
};

#endif

