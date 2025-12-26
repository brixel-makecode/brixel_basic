#ifndef _TCS34725_H_
#define _TCS34725_H_

#include <Arduino.h>
#include <Wire.h>

/**************************************************************************************************
                   TCS34725 registers
				   
There are other registers,
but only this are needed for geting colour for Mblock
***************************************************************************************************/
#define REG_ENABLE           0x00              // enable register
#define REG_ATIME            0x01              // timing register
#define REG_WTIME            0x03              // wait time  register
#define REG_CONFIG           0x0D              // configuration register
#define REG_CONTROL          0x0F              // controll register
#define REG_ID               0x12              // ID register (constant value 0x44)
#define REG_STATUS           0x13              // ID register (constant value 0x44)
#define REG_CDATAL           0x14              // clear component (low byte)
#define REG_CDATAH           0x15              // clear component (high byte)
#define REG_RDATAL           0x16              // red component (low byte)
#define REG_RDATAH           0x17              // red component (high byte)
#define REG_GDATAL           0x18              // green component (low byte)
#define REG_GDATAH           0x19              // green component (high byte)
#define REG_BDATAL           0x1A              // blue component (low byte)
#define REG_BDATAH           0x1B              // blue component (high byte)




/**************************************************************************************************
                   TCS34725 CONSTANTS
				   
There are other constants,
but only this are needed for geting colour for Mblock
***************************************************************************************************/
#define SLAVE_ADDRESS        0x29               // color sensor I2C address
#define COMMAND_HEADER       0x80               // bit 7 must be 1 when writing commands

#define COMPONENT_RED           1               // for reading only red component
#define COMPONENT_GREEN         2               // for reading only green component
#define COMPONENT_BLUE          3               // for reading only blue component

#define COMPONENT_HUE           4               // for reading only hue component
#define COMPONENT_SATURATION    5               // for reading only saturatio component
#define COMPONENT_VALUE         6               // for reading only value component

// RGB +/- 45 degrees 
// CMY +/- 15 degrees 
#define HUE_RED_MIN           -45  
#define HUE_RED_MAX            45
#define HUE_YELLOW_MIN         45
#define HUE_YELLOW_MAX         75
#define HUE_GREEN_MIN          75
#define HUE_GREEN_MAX         165
#define HUE_CYAN_MIN          165
#define HUE_CYAN_MAX          195
#define HUE_BLUE_MIN          195
#define HUE_BLUE_MAX          285
#define HUE_MAGENTA_MIN       285
#define HUE_MAGENTA_MAX       315


#define COLOR_ID_RED        1
#define COLOR_ID_GREEN      2
#define COLOR_ID_BLUE       3
#define COLOR_ID_YELLOW     4
#define COLOR_ID_CYAN       5
#define COLOR_ID_MAGENTA    6
       

class Color_sensor {
  public:
    Color_sensor();
  
    void      setup(void);
    void      init(void);
    
    void      triggerColorSensor(bool adjust);
    uint8_t   getRGBcomponent(uint8_t component);
    float     getHSVcomponent(uint8_t component);    
    bool      isColor(uint8_t colorID);



  private:
    void      write(uint8_t reg, uint8_t value);  
    uint16_t  read(uint8_t reg);
   
    uint16_t  componentRed;
    uint16_t  componentGreen;
    uint16_t  componentBlue;
   
    float     componentHue;
    float     componentSaturation;
    float     componentValue;

};


#endif