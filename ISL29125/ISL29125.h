#include "mbed.h"

#ifndef ISL29125_h
#define ISL29125_h

// =====================================
// OPERATION VARIABLES
#define ISL29125_MODE                   0x05    // 1 = G, 2 = R, 3 = B, 4 = standby, 5 = RGB, 6 = RG, 7 = GB
#define ISL29125_RANGE                  0x01    // 0 = 375 lux max (12 bit), 1 = 10000 lux max (16 bit)
#define ISL29125_ADC_RESOLUTION         0x00    // 0 = 16bit, 1 = 12bit
#define ISL29125_INTERRUPT_PRESIST      0x00    // no. out of threshold for interrupt to trigger, 0 = 1, 1 = 2, 2 = 4, 3 = 8
#define ISL29125_INTERRUPT_COLOUR       0x02    // 0 = no interrupt, 1 = g, 2 = r, 3 = b
#define ISL29125_LOW_THRESHOLD          0x0000  // 16 bit
#define ISL29125_HIGH_THRESHOLD         0x03E8  // 16 bit
// =====================================

#define ISL29125_RESET                  0x46

// =====================================
// Register Addresses
#define ISL29125_ID_ADDR        0x00
#define ISL29125_CONFIG1_ADDR   0x01
#define ISL29125_CONFIG2_ADDR   0x02
#define ISL29125_CONFIG3_ADDR   0x03
#define ISL29125_LTHRESHL_ADDR  0x04
#define ISL29125_LTHRESHH_ADDR  0x05
#define ISL29125_HTHRESHL_ADDR  0x06
#define ISL29125_HTHRESHH_ADDR  0x07
#define ISL29125_STATUS_FLAGS   0x08
#define ISL29125_GDATAL         0x09
#define ISL29125_GDATAH         0x0A
#define ISL29125_RDATAL         0x0B
#define ISL29125_RDATAH         0x0C
#define ISL29125_BDATAL         0x0D
#define ISL29125_BDATAH         0x0E
// =====================================

class ISL29125 {
    public:
    ISL29125(PinName, PinName);
    void Setup();
    void Default_Settings();
    I2C m_i2c;
    int m_addr;
    uint16_t getColourVal(uint8_t);    
    void getRawData(uint16_t *r, uint16_t *g, uint16_t *b);
    void ClearInterrupts();
    
    uint8_t getRegister(uint8_t);
    void setRegister(uint8_t, uint8_t);
};
    

#endif
