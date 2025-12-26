#include "DFRobot_MLX90614.h"

DFRobot_MLX90614::DFRobot_MLX90614(uint8_t IICAddr) {
    DiviceAddress = IICAddr;
    enable = false;
}

DFRobot_MLX90614::~DFRobot_MLX90614() {
    enable = false;
}

boolean DFRobot_MLX90614::begin(void) {
    enable = true;
    Wire.begin();
    return true;
}

float DFRobot_MLX90614::getObjectTempC(void) {
    return GetTemp(MLX90614_TOBJ1); //Get celsius temperature of the object 
}

float DFRobot_MLX90614::getAmbientTempC(void) {
    return GetTemp(MLX90614_TA); //Get celsius temperature of the ambient
}

float DFRobot_MLX90614::getObjectTempF(void) {
    return (GetTemp(MLX90614_TOBJ1) * 9 / 5) + 32;//Get fahrenheit temperature of the object
}

float DFRobot_MLX90614::getAmbientTempF(void) {
    return (GetTemp(MLX90614_TA) * 9 / 5) + 32;//Get fahrenheit temperature of the ambient
}

float DFRobot_MLX90614::GetTemp(uint8_t Register) {
    float temp;
    temp = Get_Register(Register)*0.02-273.15;//Temperature conversion
    return temp;
}

/*********************************************************************/
uint16_t DFRobot_MLX90614::Get_Register(uint8_t Reg) {
    uint16_t result;
    if(!enable){
        begin();
    }

    Wire.setClock(100000);
    Wire.beginTransmission(DiviceAddress); //Start transmission to device 
    Wire.write(Reg); //Sends register address to read rom
    Wire.endTransmission(false); //End transmission
    
    Wire.requestFrom(DiviceAddress, (uint8_t)3);//Send data n-bytes read
    result = Wire.read(); //Receive DATA
    result |= Wire.read() << 8; //Receive DATA

    return result;
}

