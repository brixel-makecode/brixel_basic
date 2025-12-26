#include "Arduino.h"
#include "Wire.h"

#define MLX90614_TA         0x06
#define MLX90614_TOBJ1      0x07

class DFRobot_MLX90614 {
public:
    DFRobot_MLX90614(uint8_t Addr = 0x5A);
    ~DFRobot_MLX90614();

    float  getObjectTempC(void),
           getAmbientTempC(void),
           getObjectTempF(void),
           getAmbientTempF(void);

private:
    bool     enable;
    uint8_t  DiviceAddress;

    boolean  begin();
    float    GetTemp(uint8_t Register);
    uint16_t Get_Register(uint8_t Reg);
    void     write16(uint8_t addr, uint16_t data);
};