/******************************************************************************
MXC6655.cpp
Library for MXC6655 accelerometer 
Bobby Schulz @ GEMS Sensing
8/1/2022

MXC6655 is a low cost MEMS accelerometer 

Distributed as-is; no warranty is given.
******************************************************************************/

#include <MXC6655.h>


MXC6655::MXC6655()
{

}

int MXC6655::begin()
{
    #if defined(ARDUINO) && ARDUINO >= 100 
        Wire.begin();
    #elif defined(PARTICLE)
        if(!Wire.isEnabled()) Wire.begin(); //Only initialize I2C if not done already //INCLUDE FOR USE WITH PARTICLE 
    #endif
    Wire.beginTransmission(ADR);
	Wire.write(0x0D); //Write to control register
	Wire.write(0x00); //Make sure register is cleared - Self test off, FSR = 2g, Power down false
	return Wire.endTransmission(); //Write back I2C status
}

float MXC6655::getAccel(uint8_t axis, uint8_t range)
{
    int val = 0; //Place data from desired access here 
    Wire.beginTransmission(ADR);
    Wire.write(REG_XAXIS + axis*2); //Point to desired output register 
    int error = Wire.endTransmission(); //Grab error from first write

    Wire.requestFrom(ADR, 2); //Read back axis values
    val = (Wire.read() << 8); //Read in high byte 
    val = val | Wire.read(); //Read in low byte 
    val = val >> 4; //Shift out empty bits
    if(error == 0) return (float(val/2048.0))*2 + offset[axis]; //Divide by 12 bit resolution, multiply by FSR, add offset //FIX! allow for multiple gain ranges
    else return 0; 
}

int MXC6655::updateAccelAll()
{
    int16_t dataRaw[3] = {0}; //X, Y, Z accel data, raw
    Wire.beginTransmission(ADR);
    Wire.write(REG_XAXIS); //Point to first reg
    int error = Wire.endTransmission(); //Grab error from first write

    Wire.requestFrom(ADR, 6); //Get all bytes at once
    for(int i = 0; i < 3; i++) {
		dataRaw[i] = (Wire.read() << 8); //Read in high byte 
		dataRaw[i] = dataRaw[i] | Wire.read(); //Read in low byte 
		dataRaw[i] = dataRaw[i] >> 4; //Shift out empty bits
		
		// Data[i] = (float(DataRaw[i]/2048.0))*8; //Divide by 12 bit resolution, multiply by FSR = 8
        if(error == 0) data[i] = (float(dataRaw[i]/2048.0))*2 + offset[i]; //Divide by 12 bit resolution, multiply by FSR, add offset //FIX! allow for multiple gain ranges
        else data[i] = 0; //If error has occored, set to zero
	}
    return error; //Return the error value regardless of state 
}



