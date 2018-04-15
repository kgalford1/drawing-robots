#include "TSL2561.h"
#include "mbed.h"


TSL2561::TSL2561(PinName sda, PinName scl) : i2c(sda, scl),  addr(TSL2561_ADDR)
{}

TSL2561::TSL2561(PinName sda, PinName scl, char i2c_address) : i2c(sda, scl),  addr(i2c_address)
{}

void TSL2561::setPowerUp(void)
{
    setRegister8(TSL2561_REG_CONTROL, 0x03);
}

void TSL2561::setPowerDown(void)
{
    setRegister8(TSL2561_REG_CONTROL, 0x00);
}

void TSL2561::defaultSettings(uint16_t high)
{
    setTiming(0,1);
    setInterruptControl(1, 1);
    setInterruptThreshold(0x00, high);
}

void TSL2561::setTiming(bool gain, uint8_t time)
{
    unsigned char timing;
    timing = getRegister8(TSL2561_REG_TIMING);

    if (gain)
        timing |= 0x10;
    else
        timing &= ~0x10;
                                                             
    timing &= ~0x03;
    timing |= (time & 0x03);

    setRegister8(TSL2561_REG_TIMING,timing);
}

void TSL2561::getData(uint16_t *data0, uint16_t *data1)
{
    *data0 = getRegister16(TSL2561_REG_DATA_0);
    *data1 = getRegister16(TSL2561_REG_DATA_1);
}

void TSL2561::getVisData(uint16_t *data)
{
    *data = getRegister16(TSL2561_REG_DATA_0) - getRegister16(TSL2561_REG_DATA_1);
}

void TSL2561::setInterruptControl(uint8_t control, uint8_t persist)
{
    setRegister8(TSL2561_REG_INTCTL, ((control & 0b00000011) << 4) | (persist & 0b00001111));
}

void TSL2561::setInterruptThreshold(uint16_t low, uint16_t high)
{
    setRegister16(TSL2561_REG_THRESH_LL, low);
    setRegister8(TSL2561_REG_THRESH_HL, high & 0xFF);
    setRegister8(TSL2561_REG_THRESH_HH, high >> 8);    
}


void TSL2561::clearInterrupt(void)
{
    char data_write[1] = {TSL2561_CMD_CLEAR};
    i2c.write(addr << 1, data_write, 1, 0);
}


// PRIVATE REGISTER FUNCTIONS
uint8_t TSL2561::getRegister8(uint8_t address)
{
    char data_write[1] = {address | TSL2561_CMD};
    char data_read[1];
    i2c.write(addr << 1, data_write, 1, 1);
    i2c.read(addr << 1, data_read, 1, 0);
    return data_read[0];
}

void TSL2561::setRegister8(uint8_t address, uint8_t value)
{
    char data_write[2] = {address | TSL2561_CMD, value};
    i2c.write(addr << 1, data_write, 2, 0);
}

uint16_t TSL2561::getRegister16(uint8_t address)
{
    char data_write[1] = {address | TSL2561_CMD};
    char data_read[2];
    i2c.write(addr << 1, data_write, 1, 1);
    i2c.read(addr << 1, data_read, 2, 0);
    return data_read[0] & 0xFFFF | data_read[1] << 8;
}

void TSL2561::setRegister16(uint8_t address, uint16_t value)
{
    // DO NOT USE - NOT WORKING FOR UNKNOWN REASON
    char data_write[3] = {address | TSL2561_CMD, value & 0xFF, value >> 8};
    i2c.write(addr << 1, data_write, 3, 0);
}
