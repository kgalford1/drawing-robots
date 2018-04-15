#include "mbed.h"
#include "m3pi.h"

#ifndef VL6180x_h
#define VL6180x_h

// =====================================
// OPERATION PARAMETERS
#define LOW_THRESHOLD               0xB0
#define INTERMEASURE_T              0x18
#define AVG_SAMPLING_T              0x30
// =====================================

#define SYS_HOLD_OPEN               0x01
#define SYS_HOLD_CLOSE              0x00
#define GPIO1_ENABLE                0x30
#define LOW_THRESH_ENABLE           0x01
#define INTERRUPT_CLR               0x07
#define CONT_RANGE_START            0x03
#define NOT_FRESH                   0x00

#define VL6180x_FAILURE_RESET       -1

#define VL6180x_IDENTIFICATION_MODEL_ID              0x0000
#define VL6180x_IDENTIFICATION_MODEL_REV_MAJOR       0x0001
#define VL6180x_IDENTIFICATION_MODEL_REV_MINOR       0x0002
#define VL6180x_IDENTIFICATION_MODULE_REV_MAJOR      0x0003
#define VL6180x_IDENTIFICATION_MODULE_REV_MINOR      0x0004
#define VL6180x_IDENTIFICATION_DATE                  0x0006
#define VL6180x_IDENTIFICATION_TIME                  0x0008

#define VL6180x_SYSTEM_MODE_GPIO0                    0x0010
#define VL6180x_SYSTEM_MODE_GPIO1                    0x0011
#define VL6180x_SYSTEM_HISTORY_CTRL                  0x0012
#define VL6180x_SYSTEM_INTERRUPT_CONFIG_GPIO         0x0014
#define VL6180x_SYSTEM_INTERRUPT_CLEAR               0x0015
#define VL6180x_SYSTEM_FRESH_OUT_OF_RESET            0x0016
#define VL6180x_SYSTEM_GROUPED_PARAMETER_HOLD        0x0017

#define VL6180x_SYSRANGE_START                       0x0018
#define VL6180x_SYSRANGE_THRESH_HIGH                 0x0019
#define VL6180x_SYSRANGE_THRESH_LOW                  0x001A
#define VL6180x_SYSRANGE_INTERMEASUREMENT_PERIOD     0x001B
#define VL6180x_SYSRANGE_MAx_CONVERGENCE_TIME        0x001C
#define VL6180x_SYSRANGE_CROSSTALK_COMPENSATION_RATE 0x001E
#define VL6180x_SYSRANGE_CROSSTALK_VALID_HEIGHT      0x0021
#define VL6180x_SYSRANGE_EARLY_CONVERGENCE_ESTIMATE  0x0022
#define VL6180x_SYSRANGE_PART_TO_PART_RANGE_OFFSET   0x0024
#define VL6180x_SYSRANGE_RANGE_IGNORE_VALID_HEIGHT   0x0025
#define VL6180x_SYSRANGE_RANGE_IGNORE_THRESHOLD      0x0026
#define VL6180x_SYSRANGE_MAx_AMBIENT_LEVEL_MULT      0x002C
#define VL6180x_SYSRANGE_RANGE_CHECK_ENABLES         0x002D
#define VL6180x_SYSRANGE_VHV_RECALIBRATE             0x002E
#define VL6180x_SYSRANGE_VHV_REPEAT_RATE             0x0031

#define VL6180x_SYSALS_START                         0x0038
#define VL6180x_SYSALS_THRESH_HIGH                   0x003A
#define VL6180x_SYSALS_THRESH_LOW                    0x003C
#define VL6180x_SYSALS_INTERMEASUREMENT_PERIOD       0x003E
#define VL6180x_SYSALS_ANALOGUE_GAIN                 0x003F
#define VL6180x_SYSALS_INTEGRATION_PERIOD            0x0040

#define VL6180x_RESULT_RANGE_STATUS                  0x004D
#define VL6180x_RESULT_ALS_STATUS                    0x004E
#define VL6180x_RESULT_INTERRUPT_STATUS_GPIO         0x004F
#define VL6180x_RESULT_ALS_VAL                       0x0050
#define VL6180x_RESULT_HISTORY_BUFFER                0x0052 
#define VL6180x_RESULT_RANGE_VAL                     0x0062
#define VL6180x_RESULT_RANGE_RAW                     0x0064
#define VL6180x_RESULT_RANGE_RETURN_RATE             0x0066
#define VL6180x_RESULT_RANGE_REFERENCE_RATE          0x0068
#define VL6180x_RESULT_RANGE_RETURN_SIGNAL_COUNT     0x006C
#define VL6180x_RESULT_RANGE_REFERENCE_SIGNAL_COUNT  0x0070
#define VL6180x_RESULT_RANGE_RETURN_AMB_COUNT        0x0074
#define VL6180x_RESULT_RANGE_REFERENCE_AMB_COUNT     0x0078
#define VL6180x_RESULT_RANGE_RETURN_CONV_TIME        0x007C
#define VL6180x_RESULT_RANGE_REFERENCE_CONV_TIME     0x0080

#define VL6180x_READOUT_AVERAGING_SAMPLE_PERIOD      0x010A
#define VL6180x_FIRMWARE_BOOTUP                      0x0119
#define VL6180x_FIRMWARE_RESULT_SCALER               0x0120
#define VL6180x_I2C_SLAVE_DEVICE_ADDRESS             0x0212
#define VL6180x_INTERLEAVED_MODE_ENABLE              0x02A3

class VL6180x
{
    public:
        VL6180x(PinName sda, PinName scl, uint8_t addr);
        
        // Setup sensor with default threshold.
        int SetupSensor(DigitalOut &pin, uint8_t addr);
        
        // Setup sensor with custom threshold.
        int SetupSensor(DigitalOut &pin, uint8_t addr, uint8_t threshold);
        
        // Used by SetupSensor().
        void PowerUp(DigitalOut pin);
        int Init();
        void DefaultSettings();
        void ChangeAddress(uint8_t addr);
        void ReadyUp(DigitalOut pin);
        void Start();
        
        // Clears interrupts to allow another to be generated.
        void ClearInterrupts();
        
        // Gets the current distance to barrier as fraction of 0-255.
        uint8_t GetDistance();
        
        // Sets the threshold at which an interrupt is generated.
        void SetThreshold(uint8_t threshold);

    private:
        I2C m_i2c;
        int m_addr;
        uint8_t getRegister(uint16_t addr);
        void setRegister(uint16_t addr, uint8_t data);
};

#endif
