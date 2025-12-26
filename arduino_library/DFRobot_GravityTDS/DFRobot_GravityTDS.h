/*!
 * @file  DFRobot_GravityTDS.h
 * @brief DFRobot_GravityTDS 类的基础结构
 * @copyright	Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license   The MIT License (MIT)
 * @author    [TangJie](jie.tang@dfrobot.com)
 * @version   V1.0.1
 * @date      2021-12-21
 * @url       https://github.com/DFRobot/DFRobot_GravityTDS
 */

#ifndef GRAVITY_TDS_H
#define GRAVITY_TDS_H

#include "Arduino.h"

#define ReceivedBufferLength 15
#define TdsFactor 0.5  // tds = ec / 2

class DFRobot_GravityTDS
{
public:
    /**
     * @fn    DFRobot_GravityTDS
     * @brief DFRobot_GravityTDS构造函数
     */
    DFRobot_GravityTDS();
    ~DFRobot_GravityTDS();
    /**
     * @fn    begin
     * @brief 初始化传感器
     */
    void begin();
    /**
     * @fn    update
     * @brief 获取数据并计算准确数据
     */
    void update();
    /**
     * @fn    setPin
     * @brief 设置模拟数据采集引脚
     * @param pin 模拟数据采集引脚
     */
    void setPin(int pin);
    /**
     * @fn    setTemperature
     * @brief 设置温度补偿数据
     * @param temp 温度补偿值
     */
    void setTemperature(float temp); 
    /**
     * @fn    setAref
     * @brief 设置ADC参考电压,Arduino UNO 上默认为5.0V
     * @param value 参考电压值
     */
    void setAref(float value);  
    /**
     * @fn    setAdcRange
     * @brief 设置ADC范围 10bit为1024,12bit为4096
     * @param range ADC范围值
     */
    void setAdcRange(float range);  //1024 for 10bit ADC;4096 for 12bit ADC
    /**
     * @fn    setKvalueAddress
     * @brief 设置K值的地址
     * @param address K值地址
     */
    void setKvalueAddress(int address); //set the EEPROM address to store the k value,default address:0x08
    /**
     * @fn    getKvalue
     * @brief 获取K值
     * @return 返回获取的K值
     */
    float getKvalue();
    /**
     * @fn    getTdsValue 
     * @brief 获取TDS数据值
     * @return 返回获取的TDS数据
     */
    float getTdsValue();
    /**
     * @fn getEcValue
     * @brief 获取EC数据值
     * @return 返回获取的EC值
     */
    float getEcValue();

private:
    int pin;
    float aref;  // default 5.0V on Arduino UNO
    float adcRange;
    float temperature;
    int kValueAddress;     //the address of the K value stored in the EEPROM
    char cmdReceivedBuffer[ReceivedBufferLength+1];   // store the serial cmd from the serial monitor
    byte cmdReceivedBufferIndex;
 
    float kValue;      // k value of the probe,you can calibrate in buffer solution ,such as 706.5ppm(1413us/cm)@25^C 
    float analogValue;
    float voltage;
    float ecValue; //before temperature compensation
    float ecValue25; //after temperature compensation
    float tdsValue;

    void readKValues();
    boolean cmdSerialDataAvailable();
    byte cmdParse();
    void ecCalibration(byte mode);
};  

#endif
