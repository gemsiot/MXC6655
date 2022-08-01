/******************************************************************************
MXC6655.cpp
Library for MXC6655 accelerometer 
Bobby Schulz @ GEMS Sensing
8/1/2022

MXC6655 is a low cost MEMS accelerometer 

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef MXC6655_h
#define MXC6655_h

#include <Particle.h>
// #include <Arduino.h>
// #include <Wire.h>

class MXC6655
{
//   enum class Range: int
// 		{
// 			EIGHT_G = 2,
//             FOUR_G = 1,
//             TWO_G = 0
// 		};
  constexpr static int TWO_G = 0;
  const uint8_t REG_XAXIS = 0x03;
  const uint8_t REG_YAXIS = 0x05;
  const uint8_t REG_ZAXIS = 0x07;
  const uint8_t REG_TOUT = 0x09;

  public:

    MXC6655(); 
    // MCP3221(void);

    // int begin(int _ADR);
    float data[3] = {0}; //Keep global values of sync axis reads here
    int begin(void);
    float getAccel(uint8_t axis, uint8_t range = TWO_G);
    int updateAccelAll();

  private:
    const int ADR = 0x15;
    // float offset[3] = {0, 0, 0.6564454}; //Manual offset for the Z axis - took average of 10 samples after manual leveling
    float offset[3] = {0, 0, 0}; //Null
};

#endif