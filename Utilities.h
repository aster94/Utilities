#ifndef Utilities_h
#define Utilities_h

#include <Arduino.h>

#if defined (ARDUINO_ARCH_SAMD)
#define UniversalSerial Serial_
#else
#define UniversalSerial HardwareSerial
#endif

// MACROS
#define TO_FAHRENHEIT(x) x * 1.8 + 32													// the formula for the conversion
#define LEN(x) ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x]))))) // complex but safe macro for the lenght, won't work with matrix
#define maybe 2				
															// true or false is not enough
// Toggle the state of a pin
void digitalToggle(uint8_t pin)
{
	digitalWrite(pin, !digitalRead(pin));
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

// Echo between two serial ports
void echo(UniversalSerial *one, UniversalSerial *two) 
{
	if (one->available())
	{
		two->write(one->read());
	}
	if (two->available())
	{
		one->write(two->read());
	}
}

// Print an array of any kind
template <class T>
void printArray(T array, size_t len, UniversalSerial *_port = &Serial)
{
	for (uint16_t i = 0; i < len; i++)
	{
		_port->println(array[i]);
		if (i == 65535)
		{
			_port->println("MAX SIZE REACHED");
		}
	}
}

#endif // Utilities_h