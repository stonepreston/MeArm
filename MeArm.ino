
#include <Servo.h>
#include "Chuck.h"
#include "Wifi.h"

// The config file contains Wifi name/password info. It is not in version control
#include "config.h"

const int middlePin = 11;
const int leftPin = 10;
const int rightPin = 9;

// Create 3 Servo objects
Servo middle, left, right;

// Create Chuck object
Chuck chuck;

// Create Wifi object and pass in the config values
Wifi wifi(configName, configPass);


void setup()
{
	Serial.begin(9600);
	middle.attach(11);
	left.attach(10);
	right.attach(9);

	chuck.init();

	// Wifi setup
	wifi.initializeESP8266();
	wifi.connectESP8266();
	wifi.displayConnectionInfo();
	wifi.setupServer();
	Serial.print("Finished setup");

}

void loop()
{

	// Check for network control data
	wifi.runServer();

	// Check for local control data
	int success = chuck.getData();

	if (success == 1) {

		// No buttons pressed
		if ( !(chuck.getX() >= 120 && chuck.getX() <= 130) ) {

		  if (chuck.getC() == 0 && chuck.getZ() == 0) {
			middle.write((chuck.getX()-25) * .6);
		  }

		  // Z button pressed
		  if (chuck.getC() == 0 && chuck.getZ() == 1) {
			left.write((chuck.getX()-25) * .6);
		  }

		  // C button pressed
		  if (chuck.getC() == 1 && chuck.getZ() == 0) {
			right.write((chuck.getX()-25) * .6);
		  }

		}

		delay(15);

	}

}
