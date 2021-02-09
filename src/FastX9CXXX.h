/// 
///  Created for personal use, use it at your own risk and benefit.
///  https://github.com/GitMoDu/FastX9CXXX
/// 

#ifndef _FASTX9CXXX_h
#define _FASTX9CXXX_h

#include <Arduino.h>

class X9CXXX
{
public:
	static const uint8_t X9_STEPS = 100; //100 Wiper Tap Points

	static const uint32_t X9C102_RESISTANCE = 1000; //X9C102 = 1kOhm
	static const uint32_t X9C103_RESISTANCE = 10000; //X9C103 = 10kOhm
	static const uint32_t X9C503_RESISTANCE = 50000; //X9C503 = 50kOhm
	static const uint32_t X9C104_RESISTANCE = 100000; //X9C104 = 100kOhm
};

template<const uint32_t Resistance>
class FastX9CXXX
{
private:
	//In microseconds.
	const uint32_t NCS_TO_NINC_SETUP = 1;
	const uint32_t NINC_HIGH_TO_UND_CHANGE = 1;
	const uint32_t UND_TO_NINC_SETUP = 3;
	const uint32_t NINC_LOW_PERIOD = 1;
	const uint32_t NINC_HIGH_PERIOD = 1;
	const uint32_t NINC_INACTIVE_TO_NCS_INACTIVE = 1;
	const uint32_t NCS_DESELECT_TIME_STORE = 20000;
	const uint32_t NCS_DESELECT_TIME_NO_STORE = 1;
	const uint32_t NINC_TO_VWRW_CHANGE = 100;
	const uint32_t NINC_CYCLE_TIME = 2;
	const uint32_t POWER_UP_TO_WIPER_STABLE = 500;

private:
	constexpr uint32_t GetResistanceStep()
	{
		return Resistance / X9CXXX::X9_STEPS;
	}

private:
	uint8_t PinCS = UINT8_MAX;
	uint8_t PinUD = UINT8_MAX;
	uint8_t PinINC = UINT8_MAX;

	uint8_t CurrentStep = 0;
	const uint32_t ResistanceStep = GetResistanceStep();

public:
	uint32_t GetEstimatedResistance()
	{
		return CurrentStep * ResistanceStep;
	}

	FastX9CXXX()
	{}

	FastX9CXXX(const uint8_t csPin, const uint8_t udPin, const uint8_t incPin)
	{
		Setup(csPin, udPin, incPin);
	}

	const bool Setup(const uint8_t csPin, const uint8_t udPin, const uint8_t incPin)
	{
		PinCS = csPin;
		PinUD = udPin;
		PinINC = incPin;

		if (PinCS == UINT8_MAX || PinUD == UINT8_MAX || PinINC == UINT8_MAX)
		{
			return false;
		}

		pinMode(PinCS, OUTPUT);
		pinMode(PinUD, OUTPUT);
		pinMode(PinINC, OUTPUT);

		digitalWrite(PinCS, LOW);
		digitalWrite(PinUD, LOW);
		digitalWrite(PinINC, HIGH);

		return true;
	}

	void Reset()
	{
		digitalWrite(PinCS, LOW);
		digitalWrite(PinUD, LOW);
		digitalWrite(PinINC, HIGH);

		for (uint8_t i = 0; i < X9CXXX::X9_STEPS; i++)
		{
			digitalWrite(PinINC, LOW);
			delayMicroseconds(NINC_HIGH_PERIOD);
			digitalWrite(PinINC, HIGH);
			delayMicroseconds(NINC_LOW_PERIOD);
		}
		digitalWrite(PinCS, HIGH);
		digitalWrite(PinUD, HIGH);

		CurrentStep = 0;
	}

	//Input step [0 ; X9_STEPS]
	void JumpToStep(const uint8_t step, const bool store = false)
	{
		if (step > X9CXXX::X9_STEPS - 1)
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

		if (store)
		{
			Store();
		}
	}

	void Down(const bool store = false)
	{
		digitalWrite(PinINC, HIGH);
		digitalWrite(PinCS, LOW);
		digitalWrite(PinUD, LOW);

		delayMicroseconds(NINC_HIGH_TO_UND_CHANGE);
		digitalWrite(PinINC, LOW);

		if (store)
		{
			Store();
		}

		if (CurrentStep > 0)
		{
			CurrentStep--;
		}
	}

	void Up(const bool store = false)
	{
		digitalWrite(PinINC, HIGH);
		digitalWrite(PinCS, LOW);
		digitalWrite(PinUD, HIGH);

		delayMicroseconds(NINC_HIGH_TO_UND_CHANGE);
		digitalWrite(PinINC, LOW);

		if (store)
		{
			Store();
		}

		if (CurrentStep < X9CXXX::X9_STEPS)
		{
			CurrentStep++;
		}
	}

	const uint8_t GetStep()
	{
		return CurrentStep;
	}

	void Store()
	{
		digitalWrite(PinINC, HIGH);
		digitalWrite(PinCS, HIGH);
		delayMicroseconds(NCS_DESELECT_TIME_STORE);//This is way too long to wait for storage, better check elapsed outside if needed.
		digitalWrite(PinCS, LOW);
	}
};

class FastX9C102 : public FastX9CXXX<X9CXXX::X9C102_RESISTANCE>
{
public:
	FastX9C102() :
		FastX9CXXX<X9CXXX::X9C102_RESISTANCE>()
	{}
	FastX9C102(const uint8_t csPin, const uint8_t udPin, const uint8_t incPin) :
		FastX9CXXX<X9CXXX::X9C102_RESISTANCE>(csPin, udPin, incPin)
	{}
};

class FastX9C103 : public FastX9CXXX<X9CXXX::X9C103_RESISTANCE>
{
public:
	FastX9C103() :
		FastX9CXXX<X9CXXX::X9C103_RESISTANCE>()
	{}
	FastX9C103(const uint8_t csPin, const uint8_t udPin, const uint8_t incPin) :
		FastX9CXXX<X9CXXX::X9C103_RESISTANCE>(csPin, udPin, incPin)
	{}
};

class FastX9C104 : public FastX9CXXX<X9CXXX::X9C104_RESISTANCE>
{
public:
	FastX9C104() :
		FastX9CXXX<X9CXXX::X9C104_RESISTANCE>()
	{}
	FastX9C104(const uint8_t csPin, const uint8_t udPin, const uint8_t incPin) :
		FastX9CXXX<X9CXXX::X9C104_RESISTANCE>(csPin, udPin, incPin)
	{}
};

class FastX9C503 : public FastX9CXXX<X9CXXX::X9C503_RESISTANCE>
{
public:
	FastX9C503() :
		FastX9CXXX<X9CXXX::X9C503_RESISTANCE>()
	{}
	FastX9C503(const uint8_t csPin, const uint8_t udPin, const uint8_t incPin) :
		FastX9CXXX<X9CXXX::X9C503_RESISTANCE>(csPin, udPin, incPin)
	{}
};
#endif
