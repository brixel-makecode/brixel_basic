#include "Color_sensor.h"
#include <math.h> 

/****************************************************************
Constructor
*****************************************************************/
Color_sensor::Color_sensor()
{

}

/****************************************************************
Communiocation setup (9600 bps)
*****************************************************************/
void Color_sensor::setup()
{
  Wire.begin();
  Serial.begin(9600);
}

/****************************************************************
Color sensor initialization
*****************************************************************/
void Color_sensor::init()
{
  write(REG_ENABLE, 0x01);     // power on  
  delayMicroseconds(10);

  //write(REG_ATIME, 0xF6);      // set integration time 24 ms
  write(REG_ATIME, 0xC0);      // set integration time 154 ms
  write(REG_CONFIG, 0x00);     // no long wait time
  //write(REG_CONTROL, 0x00);    // set 1 x gain 
  write(REG_CONTROL, 0x02);    // set 16 x gain 
  write(REG_ENABLE, 0x03);     // power on and activate color capturing  
}


/****************************************************************
Writing value into specified register
*****************************************************************/
void Color_sensor::write(uint8_t register_address, uint8_t value)
{

  Wire.beginTransmission(SLAVE_ADDRESS);                 // slave address
  Wire.write(COMMAND_HEADER | register_address);         // command header + register address
  Wire.write(value);                                     // value
  Wire.endTransmission();
}

/****************************************************************
Read two bytes from specified location
Used for reading raw value of RGB component
*****************************************************************/
uint16_t Color_sensor::read(uint8_t register_address)
{
  uint16_t dataL, dataH, data;
	
  Wire.beginTransmission(SLAVE_ADDRESS);                 // slave address
  Wire.write(COMMAND_HEADER | register_address);         // command header + register address
  Wire.endTransmission(); 
  Wire.requestFrom(SLAVE_ADDRESS, 2);                    // request two bytes of data
  dataL = Wire.read();                                   // received lowest 8 bits of data
  dataH = Wire.read();                                   // received highest 8 bits of data
  data = (dataH << 8) | dataL;                           // combine lowest and highest bits

   return data ;
}


/****************************************************************
Read raw data from sensor
Perform calculation and store it into RGB and HSV notation
*****************************************************************/
void Color_sensor::triggerColorSensor(bool adjust)
{
  uint16_t data16,c16,r16,g16,b16;
  float red, green,blue, max, min, h, s, v, delta;
  float f,p,q,t;
  uint8_t data8,i;
  
  //**********************************************************
  // read raw data
  //**********************************************************
  c16 = read(REG_CDATAL);
  r16 = read(REG_RDATAL);
  g16 = read(REG_GDATAL);
  b16 = read(REG_BDATAL); 
  
  
  //**********************************************************
  // convert raw data into RGB (0~1)
  //**********************************************************    
  red   = (float)r16 / (float)c16;
  green = (float)g16 / (float)c16;  
  blue  = (float)b16 / (float)c16;  
  


  //**********************************************************
  // convert RGB into HSV
  //********************************************************** 
  if( ( red   > green ) && ( red   > blue ) ) max = red;
  if( ( green > red   ) && ( green > blue ) ) max = green;
  if( ( blue  > red   ) && ( blue  > green) ) max = blue;
  v = max;
  
  if( ( red   < green ) && ( red   < blue ) ) min = red;
  if( ( green < red   ) && ( green < blue ) ) min = green;
  if( ( blue  < red   ) && ( blue  < green) ) min = blue;
  
  
  delta = max - min;
  s = delta/max;
  
  if( red == max )        h =     ( green - blue ) / delta;
  else if( green == max ) h = 2 + ( blue - red )   / delta;
  else                    h = 4 + ( red - green )  / delta;
  
  h = h * 60;
  if( h < 0 ) h = h + 360;
  
  
  //**********************************************************
  // storing RGB and HSV
  //********************************************************** 
  componentRed   = red   * 255;
  componentGreen = green * 255;
  componentBlue  = blue  * 255;
  
  componentHue = h;
  componentSaturation = s;
  componentValue = v;   
  
  
  
  
  
  
  
  //**********************************************************
  // adjust saturation and value, hue remains unchanged
  //**********************************************************
  if (adjust == true)
  {    
    v = 1.0;
    s = 1.0;
 
    componentHue = h;
    componentSaturation = s;
    componentValue = v;  
      
    //**********************************************************
    // convert back to RGB
    //********************************************************** 

    h = h / 60;
    i = floor(h);

    f = h - i;			
    p = v * ( 1 - s );
    q = v * ( 1 - s * f );
    t = v * ( 1 - s * ( 1 - f ) );
    
    switch( i )
    {
      case 0: 
      red = v; green = t; blue = p;
        break;
      case 1:
        red = q; green = v; blue = p;
        break;
      case 2:
        red = p; green = v; blue = t;
        break;
      case 3:
        red = p; green = q; blue = v;
        break;
      case 4:
        red = t; green = p; blue = v;
        break;
      default:                        // case 5:
        red = v; green = p; blue = q;
        break;
    }


    //**********************************************************
    // scale RGB data 0~255 
    //**********************************************************  
    componentRed   = red   * 255;
    componentGreen = green * 255;
    componentBlue  = blue  * 255;
  }
  
}


/****************************************************************
Get specified RGB component
R = 0 ~ 255
G = 0 ~ 255
B = 0 ~ 255
*****************************************************************/
uint8_t Color_sensor::getRGBcomponent(uint8_t component)
{
  uint8_t data8 = 0;

  switch(component)
  {
    case COMPONENT_RED:
      data8 = (uint8_t) componentRed;
      break;
    case COMPONENT_GREEN:
      data8 = (uint8_t) componentGreen;
      break;
  case COMPONENT_BLUE:
      data8 = (uint8_t) componentBlue;
      break;
  default:
      data8 = 0;
      break;
  }

   return data8;
}

/****************************************************************
Get specified HSV component
H = 0.0 ~ 360.0
S = 0.0 ~ 1.0
V = 0.0 ~ 1.0
*****************************************************************/
float Color_sensor::getHSVcomponent(uint8_t component)
{
  float retVal = 0.0;

  switch(component)
  {
    case COMPONENT_HUE:
      retVal = componentHue;
      break;
    case COMPONENT_SATURATION:
      retVal = componentSaturation;
      break;
  case COMPONENT_VALUE:
      retVal = componentValue;
      break;
  default:
      retVal = 0.0;
      break;
  }

   return retVal;
}

/****************************************************************
Check if sensor detected specified colour
*****************************************************************/
bool Color_sensor::isColor(uint8_t colorID)
{
  bool retVal = false;

  switch(colorID)
  {
    case COLOR_ID_RED:
      if((componentHue > HUE_RED_MIN) && ((componentHue < HUE_RED_MAX))) retVal = true;
      break;
    case COLOR_ID_GREEN:
      if((componentHue > HUE_GREEN_MIN) && ((componentHue < HUE_GREEN_MAX))) retVal = true;
      break;
    case COLOR_ID_BLUE:
      if((componentHue > HUE_BLUE_MIN) && ((componentHue < HUE_BLUE_MAX))) retVal = true;
      break;  
    case COLOR_ID_YELLOW:
      if((componentHue > HUE_YELLOW_MIN) && ((componentHue < HUE_YELLOW_MAX))) retVal = true;
      break;
    case COLOR_ID_CYAN:
      if((componentHue > HUE_CYAN_MIN) && ((componentHue < HUE_CYAN_MAX))) retVal = true;
      break;
    case COLOR_ID_MAGENTA:
      if((componentHue > HUE_MAGENTA_MIN) && ((componentHue < HUE_MAGENTA_MAX))) retVal = true;
      break; 
    default:
      retVal = false;
      break;
  }

   return retVal;
}
