/*
 * 	DHT12
 *  esp32 implicit read on second i2c channel
 *  by Mischianti Renzo <http://www.mischianti.org>
 *
 *  https://www.mischianti.org/2019/01/01/dht12-library-en/
 *
 *
 * DHT12      ----- esp32
 * SDA        ----- 21
 * SCL        ----- 22
 *
 */

#include "Arduino.h"
#include "Wire.h"
#include <DHT12.h>

// Instantiate Wire for generic use at 400kHz
TwoWire I2Cone = TwoWire(0);
// Instantiate Wire for generic use at 100kHz
TwoWire I2Ctwo = TwoWire(1);

// Set dht12 i2c comunication with second Wire using 21 22 as SDA SCL
DHT12 dht12(&I2Ctwo);
//DHT12 dht12(&I2Ctwo, 21,22);
//DHT12 dht12(&I2Ctwo, 0x5C);
//DHT12 dht12(&I2Ctwo, 21,22,0x5C);

void setup()
{
	I2Cone.begin(16,17,400000); // SDA pin 16, SCL pin 17, 400kHz frequency

	Serial.begin(112560);
	// Start sensor
	dht12.begin();
}
int timeSinceLastRead = 0;

void loop()
{
	// Report every 2 seconds.
	if(timeSinceLastRead > 2000) {
		// Read temperature as Celsius (the default)
		float t12 = dht12.readTemperature();
		// Read temperature as Fahrenheit (isFahrenheit = true)
		float f12 = dht12.readTemperature(true);
		// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
		float h12 = dht12.readHumidity();

		bool dht12Read = true;
		// Check if any reads failed and exit early (to try again).

		if (isnan(h12) || isnan(t12) || isnan(f12)) {
		  Serial.println("Failed to read from DHT12 sensor!");

		  dht12Read = false;
		}

		if (dht12Read){
			// Compute heat index in Fahrenheit (the default)
			float hif12 = dht12.computeHeatIndex(f12, h12);
			// Compute heat index in Celsius (isFahreheit = false)
			float hic12 = dht12.computeHeatIndex(t12, h12, false);
			// Compute dew point in Fahrenheit (the default)
			float dpf12 = dht12.dewPoint(f12, h12);
			// Compute dew point in Celsius (isFahreheit = false)
			float dpc12 = dht12.dewPoint(t12, h12, false);


			Serial.print("DHT12=> Humidity: ");
			Serial.print(h12);
			Serial.print(" %\t");
			Serial.print("Temperature: ");
			Serial.print(t12);
			Serial.print(" *C ");
			Serial.print(f12);
			Serial.print(" *F\t");
			Serial.print("  Heat index: ");
			Serial.print(hic12);
			Serial.print(" *C ");
			Serial.print(hif12);
			Serial.print(" *F");
			Serial.print("  Dew point: ");
			Serial.print(dpc12);
			Serial.print(" *C ");
			Serial.print(dpf12);
			Serial.println(" *F");


		}
		timeSinceLastRead = 0;

	}
	delay(100);
	timeSinceLastRead += 100;
}
