#ifndef Utilities_h
#define Utilities_h

#include <Arduino.h>

#if defined(ARDUINO_ARCH_SAMD)
#define UniversalSerial Serial_
#else
#define UniversalSerial HardwareSerial
#endif

// MACROS
#define LEN(x) ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x]))))) // complex but safe macro for the lenght
#define TO_FAHRENHEIT(x) x * 1.8 + 32
#define TO_CELSIUS(x) (x - 32) * 0.55
#define maybe 2 // true or false is not enough

// Run a task every given time without blocking the rest of the code
template <class T1, class T2>
bool doEvery(T1 *start_time, T2 interval)
{
	if (millis() > *start_time + interval)
	{
		*start_time = millis();
		return true;
	}
	else
	{
		return false;
	}
}

// Change the state of a group of pins
void pinModeGroup(uint8_t pins[], size_t len, uint8_t state)
{
	for (uint8_t i = 0; i < len; i++)
	{
		pinMode(pins[i], state);
	}
}

// Write the state of a group of pins
void digitalWriteGroup(uint8_t pins[], size_t len, uint8_t state)
{
	for (uint8_t i = 0; i < len; i++)
	{
		digitalWrite(pins[i], state);
	}
}

// Toggle the state of a pin
void digitalToggle(uint8_t pin)
{
	digitalWrite(pin, !digitalRead(pin));
}

// Toggle the state of a group of pins
void digitalToggleGroup(uint8_t pins[], size_t len)
{
	for (uint8_t i = 0; i < len; i++)
	{
		digitalWrite(pins[i], !digitalRead(pins[i]));
	}
}

// Echo between two serial ports, bi or mono directional
void echo(UniversalSerial *one, UniversalSerial *two, bool mono_directional = false)
{
	if (one->available())
	{
		two->write(one->read());
	}
	if (mono_directional == false)
	{
		if (two->available())
		{
			one->write(two->read());
		}
	}
}

// Print an array of any kind
template <class T>
void printArray(T array, size_t len, char delimiter[] = "\n", uint8_t formatter = DEC, bool invert = false, bool index = false, UniversalSerial *_port = &Serial)
{
	if (len >= 65535)
	{
		_port->println("ARRAY TOO BIG");
	}

	if (invert == false)
	{
		for (uint16_t i = 0; i < len; i++)
		{
			if (index)
			{
				_port->print(i);
				_port->print(": ");
			}
			_port->print(array[i], formatter);
			if (i < len - 1)
			{
				_port->print(delimiter);
			}
		}
	}
	else
	{
		for (uint16_t i = len; i > 0; i--)
		{
			if (index)
			{
				_port->print(i - 1);
				_port->print(": ");
			}
			_port->print(array[i - 1], formatter);
			if (i > 1)
			{
				_port->print(delimiter);
			}
		}
	}
	_port->println();
}

// Split a cstring into token and get one of them
char *splitString(char str[], uint8_t index, char delimiter[] = " ")
{
	uint8_t counter = 0;
	char *token = strtok(str, delimiter);
	while (token != NULL)
	{
		if (counter == index)
		{
			return token;
		}
		token = strtok(NULL, delimiter);
		counter++;
	}
	return NULL;
}

#endif // Utilities_h