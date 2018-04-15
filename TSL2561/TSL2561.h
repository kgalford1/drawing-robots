#ifndef TSL2561_h
#define TSL2561_h

#include "mbed.h"

#define TSL2561_ADDR_0 0x29
#define TSL2561_ADDR   0x39
#define TSL2561_ADDR_1 0x49

// TSL2561 Registers
#define TSL2561_CMD           0x80
#define TSL2561_CMD_CLEAR     0xC0
#define TSL2561_REG_CONTROL   0x00
#define TSL2561_REG_TIMING    0x01
#define TSL2561_REG_THRESH_LL 0x02
#define TSL2561_REG_THRESH_LH 0x03
#define TSL2561_REG_THRESH_HL 0x04
#define TSL2561_REG_THRESH_HH 0x05
#define TSL2561_REG_INTCTL    0x06
#define TSL2561_REG_ID        0x0A
#define TSL2561_REG_DATA_0    0x0C
#define TSL2561_REG_DATA_1    0x0E

class TSL2561
{
    public:
        TSL2561(PinName sda, PinName scl);
        
        TSL2561(PinName sda, PinName scl, char i2c_address);

        void setPowerUp(void);

        void setPowerDown(void);

        void setTiming(bool gain, uint8_t time);

        void getData(uint16_t *CH0, uint16_t *CH1);
        
        void getVisData(uint16_t *data);

        void setInterruptControl(uint8_t control, uint8_t persist);

        void setInterruptThreshold(uint16_t low, uint16_t high);

        void clearInterrupt(void);
        
        void defaultSettings(uint16_t high);
        
    private:
        I2C i2c;
        int addr;
        
        uint8_t getRegister8(uint8_t address);
    
        void setRegister8(uint8_t address, uint8_t value);

        uint16_t getRegister16(uint8_t address);

        void setRegister16(uint8_t address, uint16_t value);
};
#endif
