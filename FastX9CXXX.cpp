/// 
///  Created for personal use, use it at your own risk and benefit.
///  https://github.com/GitMoDu/FastX9CXXX
///  Depends on Fast for IO https://github.com/GitMoDu/Fast
/// 

#include "FastX9CXXX.h"

#ifdef USE_PIN_DEFINITIONS
bool FastX9CXXX::Begin()
{
	PinCS.Setup(X9_CS_PIN, LOW);
	PinUD.Setup(X9_UD_PIN, LOW);
	PinINC.Setup(X9_INC_PIN, HIGH);

	Reset();
}
#endif // USE_PIN_DEFINITIONS


bool FastX9CXXX::Setup(const byte csPin, const byte udPin, const byte incPin)
{
	PinCS.Setup(csPin, LOW);
	PinUD.Setup(udPin, LOW);
	PinINC.Setup(incPin, HIGH);

	Reset();
}

void FastX9CXXX::JumpToStep(uint8_t step)
{
	while (CurrentStep != step)
	{
		if (CurrentStep > step)
		{
			Down();
		}
		else
		{
			Up();
		}
	}
}

void FastX9CXXX::Reset()
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


void FastX9CXXX::Store()
{
	PinINC.Set(HIGH);
	PinCS.Set(HIGH);
	//delayMicroseconds(NCS_DESELECT_TIME_STORE);//This is way too long to wait for storage, better check elapsed outside if needed.
	PinCS.Set(LOW);
}

void FastX9CXXX::Up(bool store)
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

uint8_t FastX9CXXX::GetStep()
{
	return CurrentStep;
}

void FastX9CXXX::Down(bool store = true)
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


