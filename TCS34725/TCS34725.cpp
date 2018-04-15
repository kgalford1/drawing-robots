#include "TCS34725.h"

TCS34725::TCS34725(PinName sda, PinName scl) : m_i2c(sda, scl) {}

void TCS34725::Setup()
{
    m_addr = 0x29 << 1;	// LPC1768 I2C uses addresses shifted left one bit.
    Enable();
    DefaultSettings();
}

void TCS34725::Enable()
{
    setRegister(TCS34725_ENABLE, 0x01);    // Power on
    wait_ms(3);
    setRegister(TCS34725_ENABLE, 0x03);    // RGBC on
}

void TCS34725::Disable() 
{
    setRegister(TCS34725_ENABLE, 0x00);    // Power off
}

void TCS34725::DefaultSettings()
{
    setRegister(TCS34725_ATIME, TCS34725_ATIME_TIME);  // Set integration time
    setRegister(TCS34725_CONTROL, TCS34725_CONTROL_SETTING);   // Set gain
    setRegister(TCS34725_PERS, TCS34725_PERS_INT);	// Set persistence
}

void TCS34725::SetInterrupt(bool on)
{
    uint8_t current = getRegister(TCS34725_ENABLE);	// Get current register value
    if (on) 
        current |= 0x10;
    else
        current &= ~0x10;
    setRegister(TCS34725_ENABLE, current);	// Change the 4th MSB.
}

void TCS34725::ClearInterrupt()
{
    char data_write[1] = {TCS34725_COMMAND_BIT | 0x66};	// Just writing to Command register as specified in datasheet.
    m_i2c.write(m_addr, data_write, 1, 0);
}

void TCS34725::SetInterruptLimits(uint16_t low, uint16_t high)
{
    setRegister(TCS34725_AILTL, low & 0xFF);
    setRegister(TCS34725_AILTH, low >> 8 & 0xFF);
    setRegister(TCS34725_AIHTL, high & 0xFF);
    setRegister(TCS34725_AIHTH, high >> 8 & 0xFF);
}

uint16_t TCS34725::ReadRedValue()
{
    return getRegister16(TCS34725_RDATAL);
}

uint16_t TCS34725::ReadGreenValue()
{
    return getRegister16(TCS34725_GDATAL);
}

uint16_t TCS34725::ReadBlueValue()
{
    return getRegister16(TCS34725_BDATAL);
}

uint16_t TCS34725::ReadClearValue()
{
    return getRegister16(TCS34725_CDATAL);
}

void TCS34725::GetRawData(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c)
{
    *r = ReadRedValue();
    *g = ReadGreenValue();
    *b = ReadBlueValue();
    *c = ReadClearValue(); 
}


// PRIVATE REGISTER FUNCTIONS
uint8_t TCS34725::getRegister(uint8_t registerAddr) 
{
    uint8_t data;
    char data_write[1] = {TCS34725_COMMAND_BIT | registerAddr};
    char data_read[1];

    m_i2c.write(m_addr, data_write, 1, 1); 
    m_i2c.read(m_addr, data_read, 1, 0);
    
    data = data_read[0];
    return data;
}

uint16_t TCS34725::getRegister16(uint8_t registerAddr)	// Address is low byte. Auto-increments to the high byte.
{   
    uint16_t data;
    char data_write[1] = {TCS34725_COMMAND_BIT | registerAddr};
    char data_read[2];

    m_i2c.write(m_addr, data_write, 1, 1); 
    m_i2c.read(m_addr, data_read, 2, 0);
    
    data = (data_read[1] << 8) | data_read[0];
    return data;
}

void TCS34725::setRegister(uint8_t registerAddr, uint8_t data)
{
    char data_write[2] = {TCS34725_COMMAND_BIT | registerAddr, data}; 
    m_i2c.write(m_addr, data_write, 2, 0); 
}
