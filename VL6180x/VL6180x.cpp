#include "VL6180x.h"
#include "m3pi.h"

VL6180x::VL6180x(PinName sda, PinName scl, uint8_t addr) : m_i2c(sda, scl), m_addr(addr) {}

int VL6180x::SetupSensor(DigitalOut &pin, uint8_t addr)
{
	PowerUp(pin);
    if (Init() != 0)
        return -1;
	DefaultSettings();
	ChangeAddress(addr);
	ReadyUp(pin);
	return 0;
}

int VL6180x::SetupSensor(DigitalOut &pin, uint8_t addr, uint8_t threshold)
{
	PowerUp(pin);
    if (Init() != 0)
        return -1;
	DefaultSettings();
	SetThreshold(threshold);
	ChangeAddress(addr);
	ReadyUp(pin);
    return 0;
}

void VL6180x::PowerUp(DigitalOut pin)
{
    pin = 0;
    wait_ms(1);
    pin = 1;
    wait_ms(1);
}

int VL6180x::Init()
{
    uint8_t data;
    data = getRegister(VL6180x_SYSTEM_FRESH_OUT_OF_RESET);
    wait_ms(1);
    if (data != 1)
        return VL6180x_FAILURE_RESET;
        
	setRegister(0x0207, 0x01);
	setRegister(0x0208, 0x01);
	setRegister(0x0096, 0x00);
	setRegister(0x0097, 0xfd);
	setRegister(0x00e3, 0x00);
    setRegister(0x00e4, 0x04);
    setRegister(0x00e5, 0x02);
    setRegister(0x00e6, 0x01);
    setRegister(0x00e7, 0x03);
    setRegister(0x00f5, 0x02);
    setRegister(0x00d9, 0x05);
    setRegister(0x00db, 0xce);
    setRegister(0x00dc, 0x03);
    setRegister(0x00dd, 0xf8);
    setRegister(0x009f, 0x00);
    setRegister(0x00a3, 0x3c);
    setRegister(0x00b7, 0x00);
    setRegister(0x00bb, 0x3c);
    setRegister(0x00b2, 0x09);
    setRegister(0x00ca, 0x09);  
    setRegister(0x0198, 0x01);
    setRegister(0x01b0, 0x17);
    setRegister(0x01ad, 0x00);
    setRegister(0x00ff, 0x05);
    setRegister(0x0100, 0x05);
    setRegister(0x0199, 0x05);
    setRegister(0x01a6, 0x1b);
    setRegister(0x01ac, 0x3e);
    setRegister(0x01a7, 0x1f);
    setRegister(0x0030, 0x00);
    
    return 0;
}

void VL6180x::DefaultSettings()
{
	setRegister(VL6180x_SYSTEM_GROUPED_PARAMETER_HOLD, SYS_HOLD_OPEN);
    setRegister(VL6180x_SYSRANGE_INTERMEASUREMENT_PERIOD, INTERMEASURE_T);
    setRegister(VL6180x_SYSTEM_MODE_GPIO1, GPIO1_ENABLE);
    setRegister(VL6180x_READOUT_AVERAGING_SAMPLE_PERIOD, AVG_SAMPLING_T);
    setRegister(VL6180x_SYSTEM_INTERRUPT_CONFIG_GPIO, LOW_THRESH_ENABLE);
    setRegister(VL6180x_SYSRANGE_THRESH_LOW, LOW_THRESHOLD);
    setRegister(VL6180x_SYSTEM_INTERRUPT_CLEAR, INTERRUPT_CLR);
    setRegister(VL6180x_SYSTEM_GROUPED_PARAMETER_HOLD, SYS_HOLD_CLOSE);
}

void VL6180x::ChangeAddress(uint8_t addr)
{
    setRegister(VL6180x_I2C_SLAVE_DEVICE_ADDRESS, addr);
    m_addr = addr << 1;
}

void VL6180x::ReadyUp(DigitalOut pin)
{
    setRegister(VL6180x_SYSTEM_FRESH_OUT_OF_RESET, NOT_FRESH);
}

void VL6180x::Start()
{
    setRegister(VL6180x_SYSRANGE_START, CONT_RANGE_START);
}

void VL6180x::ClearInterrupts()
{
	setRegister(VL6180x_SYSTEM_INTERRUPT_CLEAR, INTERRUPT_CLR);
}

uint8_t VL6180x::GetDistance()
{
	return getRegister(VL6180x_RESULT_RANGE_VAL);
}

void VL6180x::SetThreshold(uint8_t threshold) 
{
	setRegister(VL6180x_SYSRANGE_THRESH_LOW, threshold);
} 

// PRIVATE REGISTER FUNCTIONS
uint8_t VL6180x::getRegister(uint16_t registerAddr)
{
	uint8_t data;
	char data_write[2];
	char data_read[1];
	data_write[0] = (registerAddr >> 8) & 0xFF;
	data_write[1] = registerAddr & 0xFF;
	m_i2c.write(m_addr, data_write, 2, 1); 
	m_i2c.read(m_addr, data_read, 1, 0);
	data = data_read[0];
	return data;
}

void VL6180x::setRegister(uint16_t registerAddr, uint8_t data)
{
    char data_write[3];
    data_write[0] = (registerAddr >> 8) & 0xFF;
    data_write[1] = registerAddr & 0xFF;
    data_write[2] = data & 0xFF; 
    m_i2c.write(m_addr, data_write, 3); 
}
