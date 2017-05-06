/*
 * Chuck.cpp
 *
 *  Created on: May 3, 2017
 *      Author: stonepreston
 */

#include "Chuck.h"
#include <Wire.h>

// Constructor
Chuck::Chuck() {
  x = 0;
  y = 0;
  c = 0;
  z = 0;

}

void Chuck::init() {

  Wire.begin();                 // join i2c bus as master
  Wire.beginTransmission(0x52); // transmit to device 0x52
  Wire.write(0x40);   // sends memory address
  Wire.write(0x00);   // sends sent a zero.
  Wire.endTransmission(); // stop transmitting

}
void Chuck::sendRequest() {
  Wire.beginTransmission(0x52); // transmit to device 0x52
  Wire.write(0x00);   // sends one byte
  Wire.endTransmission(); // stop transmitting
}

int Chuck::getData() {
  int i = 0;
  Wire.requestFrom(0x52, 6);  // request data from nunchuck
  while (Wire.available ()) {
    // receive byte as an integer
    buffer[i] = decodeByte(Wire.read());
    i++;
  }
  sendRequest();  // send request for next data payload
  // If we received the 6 bytes, then go print them
  if (i >= 5) {
    setX(buffer[0]);
    setY(buffer[1]);
    setZ(1);
    setC(1);
    // byte nunchuck_buf[5] contains bits for z and c buttons
    // it also contains the least significant bits for the accelerometer data
    // so we have to check each bit of byte outbuf[5]
    if ((buffer[5] >> 0) & 1)
      setZ(0);
    if ((buffer[5] >> 1) & 1)
      setC(0);
    return 1;   // success
  }
  return 0; //failure
}

char Chuck::decodeByte(char x) {

  x = (x ^ 0x17) + 0x17;
  return x;
}


// Getters
int Chuck::getX() {

  return x;
}

int Chuck::getY() {

  return y;
}

int Chuck::getZ() {

  return z;
}

int Chuck::getC() {

  return c;
}

// Setters
void Chuck::setX(int x) {

  this->x = x;
}

void Chuck::setY(int y) {

  this->y = y;
}

void Chuck::setZ(int z) {

  this->z = z;
}

void Chuck::setC(int c) {

  this->c = c;
}



