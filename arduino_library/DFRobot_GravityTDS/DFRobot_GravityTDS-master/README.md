DFRobot_GravityTDS
===========================

* [中文版](./README_CN.md)

SEN0244 即插即用，使用简单方便。3.3~5.5V的宽电压供电，0~2.3V的模拟信号输出，使得这款产品兼容5V、3.3V控制系统，能非常方便的接到现成的控制系统中使用。测量用的激励源采用交流信号，可有效防止探头极化，延长探头寿命的同时，也增加了输出信号的稳定性。TDS探头为防水探头，可长期浸入水中测量。 该产品可应用于生活用水、水培等领域的水质检测。有了这个传感器，就可轻松DIY一套TDS检测仪了，轻松检测水的洁净程度，为你的水质把好关。

![产品效果图](../../resources/images/SEN0244.png)
  
## Product Link (https://www.dfrobot.com)
    SKU: SEN0244

## Table of Contents

  * [Summary](#summary)
  * [Installation](#installation)
  * [Methods](#methods)
  * [Compatibility](#compatibility)
  * [History](#history)
  * [Credits](#credits)

## Summary

这是一个TDS传感器得库，使用该库能够很方便的获取TDS数据，即插即用，使用方便，宽电压供电.


## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
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
```

## Compatibility

MCU                | SoftwareSerial | HardwareSerial |      IIC      |
------------------ | :----------: | :----------: | :----------: | 
Arduino Uno        |      √       |      X       |      √       |
Mega2560           |      √       |      √       |      √       |
Leonardo           |      √       |      √       |      √       |
ESP32              |      X       |      √       |      √       |
ESP8266            |      √       |      X       |      √       |
micro:bit          |      X       |      X       |      √       |
FireBeetle M0      |      X       |      √       |      √       |
raspberry          |      X       |      √       |      √       |

## History

- 2018-01-03 - Version 1.0.0 released.
- 2021-12-21 - Version 1.0.1 released.

## Credits

Written by TangJie(jie.tang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))