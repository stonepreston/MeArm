#include <Servo.h>
#include "Chuck.h"

const int middlePin = 11;
const int leftPin = 10;
const int rightPin = 9;

// Create 3 Servo objects
Servo middle, left, right;

// Create Chuck object
Chuck chuck;

void setup()
{

	Serial.begin(9600);
	middle.attach(11);
	left.attach(10);
	right.attach(9);

	chuck.init();

	Serial.print("Finished setup");

}

void loop()
{
	// Check for local control data
	int success = chuck.getData();

	if (success == 1) {

		// Ensure we arent at neutral stick position (or relatively close to it)
		if ( !(chuck.getX() >= 120 && chuck.getX() <= 130) ) {

		  int servoAngle = (chuck.getX()-25) * .6;

		  // No buttons pressed
		  if (chuck.getC() == 0 && chuck.getZ() == 0) {
			middle.write(servoAngle);
		  }

		  // Z button pressed
		  if (chuck.getC() == 0 && chuck.getZ() == 1) {
			left.write(servoAngle);
		  }

		  // C button pressed
		  if (chuck.getC() == 1 && chuck.getZ() == 0) {
			right.write(servoAngle);
		  }

		}

		// Delay a bit so we dont burn out servo motors
		delay(15);

	}

}
