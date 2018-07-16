/// 
///  Created for personal use, use it at your own risk and benefit.
///  https://github.com/GitMoDu/FastX9CXXX
///  Depends on Fast for IO https://github.com/GitMoDu/Fast
/// 

#ifndef _FASTX9CXXX_h
#define _FASTX9CXXX_h

//#define USE_PIN_DEFINITIONS //Enable this to set the pins at compile time, from a definitions file.

#include <Arduino.h>
#include <Fast.h>

#ifdef USE_PIN_DEFINITIONS 
#include "PinDefinitions.h"
#endif // USE_PIN_DEFINITIONS

#define X9_STEPS 100 //100 Wiper Tap Points

#define X9C102_RESISTANCE   1000 //X9C102 = 1kOhm
#define X9C103_RESISTANCE  10000 //X9C103 = 10kOhm
#define X9C503_RESISTANCE  50000 //X9C503 = 50kOhm
#define X9C104_RESISTANCE 100000 //X9C104 = 100kOhm


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


class FastX9CXXX
{
private:
	FastOut PinCS, PinUD;
	FastShifter PinINC;

	uint8_t CurrentStep = 0;
	float ResistanceStep = 0;
	bool ResistanceUpdated = false;

protected:
	virtual uint32_t GetMaxResistance()
	{
		return 0;
	}
	
	void UpdateResistanceStep()
	{
		if(!ResistanceUpdated && GetMaxResistance() != 0)
		{
			ResistanceStep = (float)GetMaxResistance() / (float) X9_STEPS;
			ResistanceUpdated = true;
		}		
	}

public:
#ifdef USE_PIN_DEFINITIONS
	bool Begin()
	{
		PinCS.Setup(X9_CS_PIN, LOW);
		PinUD.Setup(X9_UD_PIN, LOW);
		PinINC.Setup(X9_INC_PIN, HIGH);

		UpdateResistanceStep();
		
		Reset();
	}
#endif // USE_PIN_DEFINITIONS

	uint32_t GetEstimatedResistance()
	{
		UpdateResistanceStep();
		
		return uint32_t(round((float)CurrentStep * ResistanceStep));
	}

	FastX9CXXX()
	{
		ResistanceUpdated = false;
		UpdateResistanceStep();
	}

	FastX9CXXX(const byte csPin, const byte udPin, const byte incPin)
	{
		ResistanceUpdated = false;
		Setup(csPin, udPin, incPin);
		UpdateResistanceStep();
	}

	void Setup(const byte csPin, const byte udPin, const byte incPin)
	{
		PinCS.Setup(csPin, LOW);
		PinUD.Setup(udPin, LOW);
		PinINC.Setup(incPin, HIGH);

		Reset();
	}

	void Reset()
	{
		PinCS.Set(LOW);
		PinUD.Set(LOW);
		PinINC.Set(HIGH);
		for (uint8_t i = 0; i < X9_STEPS; i++)
		{
			PinINC.PulseLow(NINC_HIGH_PERIOD);
			delayMicroseconds(NINC_LOW_PERIOD);
		}
		PinCS.Set(HIGH);
		PinUD.Set(HIGH);
		CurrentStep = 0;
	}

	//Input step [0 ; X9_STEPS]
	void JumpToStep(const uint8_t step, const bool store = true)
	{
		if (step > X9_STEPS)
		{
			return;//Invalid step.
		}
		while (CurrentStep != step)
		{
			if (CurrentStep > step)
			{
				Down(false);
			}
			else
			{
				Up(false);
			}
		}
		
		if(store)
		{
			Store();
		}
	}

	void Down(const bool store = true)
	{
		PinINC.Set(HIGH);
		PinCS.Set(LOW);
		PinUD.Set(LOW);
		delayMicroseconds(NINC_HIGH_TO_UND_CHANGE);
		PinINC.Set(LOW);

		if (store)
		{
			Store();
		}

		if (CurrentStep > 1)
		{
			CurrentStep--;
		}
	}
	
	void Up(bool store = true)
	{
		PinINC.Set(HIGH);
		PinCS.Set(LOW);
		PinUD.Set(HIGH);
		delayMicroseconds(NINC_HIGH_TO_UND_CHANGE);
		PinINC.Set(LOW);

		if (store)
		{
			Store();
		}

		if (CurrentStep < X9_STEPS)
		{
			CurrentStep++;
		}
	}

	uint8_t GetStep()
	{
		return CurrentStep;
	}

	void Store()
	{
		PinINC.Set(HIGH);
		PinCS.Set(HIGH);
		//delayMicroseconds(NCS_DESELECT_TIME_STORE);//This is way too long to wait for storage, better check elapsed outside if needed.
		PinCS.Set(LOW);
	}
};

class FastX9C102 : public FastX9CXXX
{
protected:
	virtual uint32_t GetMaxResistance()
	{
		UpdateResistanceStep();
		return X9C102_RESISTANCE;
	}
};

class FastX9C103 : public FastX9CXXX
{
protected:
	virtual uint32_t GetMaxResistance()
	{
		return X9C103_RESISTANCE;
	}
};

class FastX9C503 : public FastX9CXXX
{
protected:
	virtual uint32_t GetMaxResistance()
	{
		return X9C503_RESISTANCE;
	}
};

class FastX9C104 : public FastX9CXXX
{
protected:
	virtual uint32_t GetMaxResistance()
	{
		return X9C104_RESISTANCE;
	}
};

#endif

