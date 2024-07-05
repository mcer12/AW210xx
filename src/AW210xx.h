#ifndef AW210XX_H
#define AW210XX_H

#include <Arduino.h>
#include <Wire.h>

#define ADDR_AW 0x34
#define I2C_SPEED 400000

// Register addresses
#define REG_GCR 0x00

// BR register base address
#define REG_BR_BASE 0x01
#define NUM_BR_REGISTERS 36

// Update register
#define REG_UPDATE 0x49

// COL register base address
#define REG_COL_BASE 0x4A
#define NUM_COL_REGISTERS 36

// Other register addresses
#define REG_GCCR 0x6E
#define REG_PHCR 0x70
#define REG_OSDCR 0x71
#define REG_OSST0 0x72
#define REG_OSST1 0x73
#define REG_OSST2 0x74
#define REG_OSST3 0x75
#define REG_OSST4 0x76
#define REG_OTCR 0x77
#define REG_SSCR 0x78
#define REG_UVCR 0x79
#define REG_GCR2 0x7A
#define REG_GCR4 0x7C
#define REG_VER 0x7E
#define REG_RESET 0x7F
#define REG_WBR 0x90
#define REG_WBG 0x91
#define REG_WBB 0x92
#define REG_PATCFG 0xA0
#define REG_PATGO 0xA1
#define REG_PATT0 0xA2
#define REG_PATT1 0xA3
#define REG_PATT2 0xA4
#define REG_PATT3 0xA5
#define REG_FADEH 0xA6
#define REG_FADEL 0xA7
#define REG_GCOLR 0xA8
#define REG_GCOLG 0xA9
#define REG_GCOLB 0xAA
#define REG_GCFG0 0xAB
#define REG_GCFG1 0xAC

// PWM Frequency Enum
enum PWMFrequency
{
    FREQ_62K = 0b000,
    FREQ_32K = 0b001,
    FREQ_4K = 0b010,
    FREQ_2K = 0b011,
    FREQ_1K = 0b100,
    FREQ_500 = 0b101,
    FREQ_244 = 0b110,
    FREQ_122 = 0b111
};

class AW210xx
{
public:
    void begin(uint8_t addr = ADDR_AW, uint32_t i2cSpeed = I2C_SPEED, PWMFrequency freq = FREQ_2K, bool phaseShift = true);
    uint8_t readRegister(uint8_t reg);
    uint8_t getGlobalBri();
    uint8_t getBri(uint8_t channel);
    uint8_t getCol(uint8_t channel);
    void setRegister(uint8_t registerAddress, uint8_t value);
    void setGlobalBri(uint8_t value);
    void setPhaseShift(bool en);
    void setSlewRate(uint8_t riseTime, uint8_t fallTime);
    void setBri(uint8_t channel, uint8_t value);
    void setCol(uint8_t channel, uint8_t value);
    void setCombinedBri(uint8_t channel, uint16_t value);
    void setCombinedBriCIE(uint8_t channel, uint16_t value);
  //  void setBalance(uint8_t r, uint8_t g, uint8_t b);
    void setFreq(PWMFrequency freq);
    void setChipEnable(bool enable);
    void setPowerSaving(bool enable);
    void update();
    void reset();

private:
    uint8_t _addr;
};

#endif
