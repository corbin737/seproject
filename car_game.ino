/*******************************************************************************
 * Program: car_game.ino
 * Author: Corbin Mcelhinney, Kalvin Thye
 * Description: Main program
 * Last Modified: November 24, 2016
 ******************************************************************************/

#include <OrbitOledGrph.h>
#include <Wire.h>
#include "vc.h"

static TwoWire orbitWire(0);

static float const    SensorMaximumReading= 512.0;
static float const    SensorMaximumAccel  = 9.81 * 4.0;
static uint8_t const  SensorAccelerometer = 0x1D;
static float          ShakeAccumulator    = 0;

void WireInit()
{
  orbitWire.begin();
}

void WireWriteByte(int address, uint8_t value)
{
  orbitWire.beginTransmission(address);
  orbitWire.write(value);
  orbitWire.endTransmission();
}

void WireWriteRegister(int address, uint8_t reg, uint8_t value)
{
  orbitWire.beginTransmission(address);
  orbitWire.write(reg);
  orbitWire.write(value);
  orbitWire.endTransmission();
}

void WireRequestArray(int address, uint32_t* buffer, uint8_t amount)
{
  orbitWire.requestFrom(address, amount);
  do
  {
    while(!orbitWire.available());
    *(buffer++) = orbitWire.read();
  } while(--amount > 0);
}

void ShakeInit()
{
  WireWriteRegister(SensorAccelerometer, 0x31, 1);
  WireWriteRegister(SensorAccelerometer, 0x2D, 1 << 3);
}

void ShakeTick()
{
  size_t const DataLength = 6;
  uint32_t data[DataLength] = { 0 };

  WireWriteByte(SensorAccelerometer, 0x32);
  WireRequestArray(SensorAccelerometer, data, DataLength);

  uint16_t xi = (data[1] << 8) | data[0];
  uint16_t yi = (data[3] << 8) | data[2];
  uint16_t zi = (data[5] << 8) | data[4];
  float x = *(int16_t*)(&xi) / SensorMaximumReading * SensorMaximumAccel;
  float y = *(int16_t*)(&yi) / SensorMaximumReading * SensorMaximumAccel;
  float z = *(int16_t*)(&zi) / SensorMaximumReading * SensorMaximumAccel;

  ShakeAccumulator = y;
}

const int leftBtnPin = PE_0;
const int rightBtnPin = PD_2;
const int bottomSwitchPin = PA_6;
const int topSwitchPin = PA_7;
const int tivaBtnPin = PUSH1;

HardwareState state;

void setup() {
  pinMode(leftBtnPin, INPUT);
  pinMode(rightBtnPin, INPUT);
  pinMode(bottomSwitchPin, INPUT);
  pinMode(topSwitchPin, INPUT);
  pinMode(tivaBtnPin, INPUT_PULLUP);

  WireInit();
  ShakeInit();
  OrbitOledInit();
  vcInit();
}

void loop() {
  ShakeTick();
  state.leftBtn = digitalRead(leftBtnPin);
  state.rightBtn = digitalRead(rightBtnPin);
  state.bottomSwitch = digitalRead(bottomSwitchPin);
  state.topSwitch = digitalRead(topSwitchPin);
  state.accel = ShakeAccumulator;
  state.tivaBtn = digitalRead(tivaBtnPin);
  vcLoop(state);
}
