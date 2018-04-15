#include "ISL29125.h"

ISL29125::ISL29125(PinName sda, PinName scl) : m_i2c(sda, scl) {}

void ISL29125::Setup()
{
    m_addr = 136;
    Default_Settings();
}

void ISL29125::Default_Settings()
{
    
    setRegister(ISL29125_ID_ADDR, ISL29125_RESET);
    wait_ms(100);
    setRegister(ISL29125_STATUS_FLAGS, 0x00);
    wait_ms(50);
    setRegister(ISL29125_CONFIG1_ADDR, ((ISL29125_ADC_RESOLUTION << 4 | ISL29125_RANGE << 3) | ISL29125_MODE));
    setRegister(ISL29125_CONFIG3_ADDR, (ISL29125_INTERRUPT_PRESIST<<2) | ISL29125_INTERRUPT_COLOUR);
    setRegister(ISL29125_LTHRESHL_ADDR, ISL29125_LOW_THRESHOLD & 0xFF);
    setRegister(ISL29125_LTHRESHH_ADDR, (ISL29125_LOW_THRESHOLD >> 8) & 0xFF);
    setRegister(ISL29125_HTHRESHL_ADDR, ISL29125_HIGH_THRESHOLD & 0xFF);
    setRegister(ISL29125_HTHRESHH_ADDR, (ISL29125_HIGH_THRESHOLD >> 8) & 0xFF);
}


uint16_t ISL29125::getColourVal(uint8_t registerAddr)
{
    int data[2];
    data[0] = getRegister(registerAddr);
    data[1] = getRegister(registerAddr + 1);
    return data[1]<<8|data[0];
}

void ISL29125::getRawData(uint16_t *r, uint16_t *g, uint16_t *b)
{
    *g = getColourVal(0x09);
    *r = getColourVal(0x0B);    
    *b = getColourVal(0x0D);
}

void ISL29125::ClearInterrupts()
{
    getRegister(ISL29125_STATUS_FLAGS);
}

uint8_t ISL29125::getRegister(uint8_t registerAddr)
{
    uint8_t data;
    char data_write[1] = {registerAddr};
    char data_read[1];

    m_i2c.write(m_addr, data_write, 1, 1); 
    m_i2c.read(m_addr|0x01, data_read, 1, 0);
    
    data = data_read[0];
    return data;
}


void ISL29125::setRegister(uint8_t registerAddr, uint8_t data)
{
    char data_write[2] = {registerAddr, data}; 
    m_i2c.write(m_addr, data_write, 2, 0); 
}
