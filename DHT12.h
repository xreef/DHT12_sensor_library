/* DHT library

 MIT license
 written by Adafruit Industries
 */
#ifndef DHT12_h
#define DHT12_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Wire.h"

#define DEFAULT_DHT12_ADDRESS 0x5C;
#define DEFAULT_SDA SDA;
#define DEFAULT_SCL SCL;

#define MIN_ELAPSED_TIME 2000

// Uncomment to enable printing out nice debug messages.
//#define DHT_DEBUG

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef DHT_DEBUG
	#define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
	#define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
	#define DEBUG_PRINT(...) {}
	#define DEBUG_PRINTLN(...) {}
#endif

#ifndef __AVR
	#define DHTLIB_TIMEOUT 10000  // should be approx. clock/40000
#else
	#ifndef F_CPU
		#define DHTLIB_TIMEOUT 1000  // should be approx. clock/40000
	#else
		#define DHTLIB_TIMEOUT (F_CPU/40000)
	#endif
#endif

class DHT12 {
public:
	enum ReadStatus {
		OK,
		ERROR_CHECKSUM,
		ERROR_TIMEOUT,
		ERROR_CONNECT,
		ERROR_ACK_L,
		ERROR_ACK_H,
		ERROR_UNKNOWN,
		NONE
	};

	DHT12(void);
	DHT12(uint8_t addressORPin, bool oneWire = false);
#ifndef __AVR
	DHT12(uint8_t sda, uint8_t scl);
	DHT12(uint8_t sda, uint8_t scl, uint8_t address);
#endif
	void begin(void);
	float readTemperature(bool scale = false, bool force = false);
	float convertCtoF(float);
	float convertFtoC(float);
	float computeHeatIndex(float temperature, float percentHumidity, bool isFahrenheit = true);
	float readHumidity(bool force = false);
	float dewPoint(float temperature, float humidity, bool isFahrenheit = true);
	ReadStatus readStatus(bool force = false);bool read(bool force = false);

private:
	bool _isOneWire = false;

	uint8_t data[5];
	uint8_t _address = DEFAULT_DHT12_ADDRESS
	;
	uint8_t _sda = DEFAULT_SDA
	;
	uint8_t _scl = DEFAULT_SCL
	;

	uint32_t _lastreadtime = 0;
	ReadStatus _lastresult = NONE;

	uint8_t _pin = 3;
#ifdef __AVR
	// Use direct GPIO access on an 8-bit AVR so keep track of the port and bitmask
	// for the digital pin connected to the DHT.  Other platforms will use digitalRead.
	uint8_t _bit, _port;
#endif
	uint32_t _maxcycles = 0;

	ReadStatus _checksum(void);
	uint32_t expectPulse(bool level);
	ReadStatus _readSensor(uint8_t wakeupDelay, uint8_t leadingZeroBits);

};

class InterruptLock {
public:
	InterruptLock() {
		noInterrupts();
	}
	~InterruptLock() {
		interrupts();
	}

};

#endif
