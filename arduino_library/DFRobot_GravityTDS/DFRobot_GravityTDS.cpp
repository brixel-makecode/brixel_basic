/*!
 * @file  DFRobot_GravityTDS.cpp
 * @brief DFRobot_GravityTDS 类的基础结构，基础方法的实现
 * @copyright	Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license   The MIT License (MIT)
 * @author    [TangJie](jie.tang@dfrobot.com)
 * @version   V1.0.1
 * @date      2021-12-21
 * @url       https://github.com/DFRobot/DFRobot_GravityTDS
 */

#include <EEPROM.h>
#include "DFRobot_GravityTDS.h"

#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

DFRobot_GravityTDS::DFRobot_GravityTDS()
{
  this->pin = A1;
  this->temperature = 25.0;
  this->aref = 5.0;
  this->adcRange = 1024.0;
  this->kValueAddress = 8;
  this->kValue = 1.0;
}

DFRobot_GravityTDS::~DFRobot_GravityTDS()
{
}

void DFRobot_GravityTDS::setPin(int pin)
{
	this->pin = pin;
}

void DFRobot_GravityTDS::setTemperature(float temp)
{
	this->temperature = temp;
}

void DFRobot_GravityTDS::setAref(float value)
{
	this->aref = value;
}

void DFRobot_GravityTDS::setAdcRange(float range)
{
  this->adcRange = range;
}

void DFRobot_GravityTDS::setKvalueAddress(int address)
{
  this->kValueAddress = address;
}

void DFRobot_GravityTDS::begin()
{
	pinMode(this->pin,INPUT);
	readKValues();
}

float DFRobot_GravityTDS::getKvalue()
{
	return this->kValue;
}

void DFRobot_GravityTDS::update()
{
	this->analogValue = analogRead(this->pin);
	this->voltage = this->analogValue/this->adcRange*this->aref;
	this->ecValue=(133.42*this->voltage*this->voltage*this->voltage - 255.86*this->voltage*this->voltage + 857.39*this->voltage)*this->kValue;
	this->ecValue25  =  this->ecValue / (1.0+0.02*(this->temperature-25.0));  //temperature compensation
	this->tdsValue = ecValue25 * TdsFactor;
	if(cmdSerialDataAvailable() > 0){
    ecCalibration(cmdParse());  // if received serial cmd from the serial monitor, enter into the calibration mode
  }
}

float DFRobot_GravityTDS::getTdsValue()
{
	return tdsValue;
}

float DFRobot_GravityTDS::getEcValue()
{
  return ecValue25;
}


void DFRobot_GravityTDS::readKValues()
{
  EEPROM_read(this->kValueAddress, this->kValue);  
  if(EEPROM.read(this->kValueAddress)==0xFF && EEPROM.read(this->kValueAddress+1)==0xFF && EEPROM.read(this->kValueAddress+2)==0xFF && EEPROM.read(this->kValueAddress+3)==0xFF){
    this->kValue = 1.0;   // default value: K = 1.0
    EEPROM_write(this->kValueAddress, this->kValue);
  }
}

boolean DFRobot_GravityTDS::cmdSerialDataAvailable()
{
  char cmdReceivedChar;
  static unsigned long cmdReceivedTimeOut = millis();
  while (Serial.available()>0){   
    if (millis() - cmdReceivedTimeOut > 500U){
      cmdReceivedBufferIndex = 0;
      memset(cmdReceivedBuffer,0,(ReceivedBufferLength+1));
    }
    cmdReceivedTimeOut = millis();
    cmdReceivedChar = Serial.read();
    if (cmdReceivedChar == '\n' || cmdReceivedBufferIndex==ReceivedBufferLength){
		cmdReceivedBufferIndex = 0;
		strupr(cmdReceivedBuffer);
		return true;
    }else{
      cmdReceivedBuffer[cmdReceivedBufferIndex] = cmdReceivedChar;
      cmdReceivedBufferIndex++;
    }
  }
  return false;
}

byte DFRobot_GravityTDS::cmdParse()
{
  byte modeIndex = 0;
  if(strstr(cmdReceivedBuffer, "ENTER") != NULL) 
    modeIndex = 1;
  else if(strstr(cmdReceivedBuffer, "EXIT") != NULL) 
    modeIndex = 3;
  else if(strstr(cmdReceivedBuffer, "CAL:") != NULL)   
    modeIndex = 2;
  return modeIndex;
}

void DFRobot_GravityTDS::ecCalibration(byte mode)
{
  char *cmdReceivedBufferPtr;
  static boolean ecCalibrationFinish = 0;
  static boolean enterCalibrationFlag = 0;
  float KValueTemp,rawECsolution;
  switch(mode){
    case 0: 
    if(enterCalibrationFlag)
      Serial.println(F("Command Error"));
      break;
    case 1:
      enterCalibrationFlag = 1;
      ecCalibrationFinish = 0;
      Serial.println();
      Serial.println(F(">>>Enter Calibration Mode<<<"));
      Serial.println(F(">>>Please put the probe into the standard buffer solution<<<"));
      Serial.println();
      break;
    case 2:
      cmdReceivedBufferPtr=strstr(cmdReceivedBuffer, "CAL:");
      cmdReceivedBufferPtr+=strlen("CAL:");
      rawECsolution = strtod(cmdReceivedBufferPtr,NULL)/(float)(TdsFactor);
      rawECsolution = rawECsolution*(1.0+0.02*(temperature-25.0));
      if(enterCalibrationFlag)
      {
        KValueTemp = rawECsolution/(133.42*voltage*voltage*voltage - 255.86*voltage*voltage + 857.39*voltage);  //calibrate in the  buffer solution, such as 707ppm(1413us/cm)@25^c
        if((rawECsolution>0) && (rawECsolution<2000) && (KValueTemp>0.25) && (KValueTemp<4.0)){
          Serial.println();
          Serial.print(F(">>>Confrim Successful,K:"));
          Serial.print(KValueTemp);
          Serial.println(F(", Send EXIT to Save and Exit<<<"));
          kValue =  KValueTemp;
          ecCalibrationFinish = 1;
        }else{
          Serial.println();
          Serial.println(F(">>>Confirm Failed,Try Again<<<"));
          Serial.println();
          ecCalibrationFinish = 0;
        }        
      }
      break;
    case 3:
      if(enterCalibrationFlag){
        Serial.println();
        if(ecCalibrationFinish){
          EEPROM_write(kValueAddress, kValue);
          Serial.print(F(">>>Calibration Successful,K Value Saved"));
        }else{
          Serial.print(F(">>>Calibration Failed"));       
          Serial.println(F(",Exit Calibration Mode<<<"));
          Serial.println();
          ecCalibrationFinish = 0;
          enterCalibrationFlag = 0;
        } 
      }
      break;
    }
}
