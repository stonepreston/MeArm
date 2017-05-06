/*
 * Chuck.h
 *
 *  Created on: May 3, 2017
 *      Author: stonepreston
 */

#ifndef Chuck_H
#define Chuck_H

#include <Arduino.h>

class Chuck {

  public:

    // Methods
    Chuck();
    void init();
    int getData();

    // Getters
    int getX();
    int getY();
    int getZ();
    int getC();

    // Setters
    void setX(int x);
    void setY(int y);
    void setZ(int z);
    void setC(int c);

  private:

    // Methods
    void sendRequest();
    char decodeByte(char x);

    // Properties
    int x;
    int y;
    int z;
    int c;
    uint8_t buffer[6];
};

#endif /* CHUCK_H_ */
