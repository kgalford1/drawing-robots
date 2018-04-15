#include "mbed.h"

// Note when reading values from registers, we first right the address to the COMMAND
// register. When writing to this register we must set the MSB to 1.

#ifndef TCS34725_h
#define TCS34725_h

// =====================================
// OPERATION PARAMETERS
#define TCS34725_PERS_INT			0x0A    // Consecutive readings for interrupt
#define TCS34725_WTIME_TIME         0xAB	// Wait time
#define TCS34725_ATIME_TIME         0xD5    // Intergration time
#define TCS34725_CONTROL_SETTING    0x01    // Gain -> 00 = 1x, 01 = 4x, 10 = 16x, 11 = 60x
// =====================================

#define TCS34725_COMMAND_BIT    0x80
#define TCS34725_AUTO_INC		0x20

#define TCS34725_ENABLE         0x00
#define TCS34725_ATIME          0x01
#define TCS34725_WTIME          0x03
#define TCS34725_AILTL          0x04
#define TCS34725_AILTH          0x05
#define TCS34725_AIHTL          0x06
#define TCS34725_AIHTH          0x07
#define TCS34725_PERS           0x0C

#define TCS34725_CONFIG         0x0D
#define TCS34725_CONTROL        0x0F
#define TCS34725_ID             0x12
#define TCS34725_STATUS         0x13

#define TCS34725_CDATAL         0x14
#define TCS34725_CDATAH         0x15
#define TCS34725_RDATAL         0x16
#define TCS34725_RDATAH         0x17
#define TCS34725_GDATAL         0x18
#define TCS34725_GDATAH         0x19
#define TCS34725_BDATAL         0x1A
#define TCS34725_BDATAH         0x1B

class TCS34725
{
    public:
    TCS34725(PinName, PinName);
    
    // Turns on sensor and enables RGBC sensoring.
    void Setup();
    
    // Is called by Setup()
    void Enable();
    
    // Puts TCS34725 into standby
    void Disable();
    
    // Turns the interrupt output pin on or off. N.B. That the interrupt functionality
    // of the TCS34725 is ALWAYS enabled, by the pin's output is not.
    void SetInterrupt(bool on);
    
    // Sets persistence, integration time, and gain.
    void DefaultSettings();
    
    // Clears interrupts, allowing another to be made by the sensor.
    void ClearInterrupt();
    
    // Low threshold is evaluated first. If low > high, high threshold is ignored.
    void SetInterruptLimits(uint16_t low, uint16_t high);
    
    // Reads colour intensities.
    uint16_t ReadRedValue();
    uint16_t ReadGreenValue();
    uint16_t ReadBlueValue();
    uint16_t ReadClearValue();
    
    // Allows all data values to be set by one function call.
    void GetRawData(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c);

    private:
    I2C m_i2c;
    int m_addr;
    uint8_t getRegister(uint8_t registerAddr);
    uint16_t getRegister16(uint8_t registerAddr);
    void setRegister(uint8_t registerAddr, uint8_t data);
};

#endif
