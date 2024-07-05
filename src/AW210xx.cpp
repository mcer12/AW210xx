#include "AW210xx.h"

const uint16_t cie[256] = {
	0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 
	2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 
	4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 
	7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 
	10, 11, 11, 12, 12, 12, 13, 13, 14, 14, 
	15, 15, 16, 16, 17, 17, 18, 18, 19, 20, 
	20, 21, 21, 22, 23, 23, 24, 25, 25, 26, 
	27, 28, 28, 29, 30, 31, 31, 32, 33, 34, 
	35, 36, 36, 37, 38, 39, 40, 41, 42, 43, 
	44, 45, 46, 47, 48, 49, 50, 52, 53, 54, 
	55, 56, 57, 59, 60, 61, 62, 64, 65, 66, 
	68, 69, 70, 72, 73, 75, 76, 77, 79, 80, 
	82, 83, 85, 87, 88, 90, 91, 93, 95, 96, 
	98, 100, 102, 103, 105, 107, 109, 111, 113, 115, 
	116, 118, 120, 122, 124, 126, 128, 131, 133, 135, 
	137, 139, 141, 143, 146, 148, 150, 152, 155, 157, 
	160, 162, 164, 167, 169, 172, 174, 177, 179, 182, 
	185, 187, 190, 193, 195, 198, 201, 204, 206, 209, 
	212, 215, 218, 221, 224, 227, 230, 233, 236, 239, 
	242, 245, 249, 252, 255, 258, 262, 265, 268, 272, 
	275, 279, 282, 286, 289, 293, 296, 300, 303, 307, 
	311, 315, 318, 322, 326, 330, 334, 338, 342, 346, 
	350, 354, 358, 362, 366, 370, 374, 378, 383, 387, 
	391, 396, 400, 404, 409, 413, 418, 422, 427, 432, 
	436, 441, 446, 450, 455, 460, 465, 470, 475, 480, 
	485, 490, 495, 500, 505, 510, 
};

void AW210xx::begin(uint8_t addr, uint32_t i2cSpeed, PWMFrequency freq, bool phaseShift)
{
    _addr = addr;
    Wire.begin();
    Wire.setClock(i2cSpeed);
    reset();
    setFreq(FREQ_2K);
    setPowerSaving(true);
    setChipEnable(true);
}

uint8_t AW210xx::readRegister(uint8_t reg)
{
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(_addr, 1);
    while (Wire.available() < 1)
        ;
    return Wire.read();
}

void AW210xx::setRegister(uint8_t registerAddress, uint8_t value)
{
    Wire.beginTransmission(_addr);
    Wire.write(registerAddress);
    Wire.write(value);
    Wire.endTransmission();
}

void AW210xx::setGlobalBri(uint8_t value)
{
    setRegister(REG_GCCR, value);
}

uint8_t AW210xx::getGlobalBri()
{
    return readRegister(REG_GCCR);
}

uint8_t AW210xx::getBri(uint8_t channel)
{
    if (channel >= NUM_BR_REGISTERS)
    {
        return 0;
    }
    uint8_t registerAddress = REG_BR_BASE + channel;
    return readRegister(registerAddress);
}

uint8_t AW210xx::getCol(uint8_t channel)
{
    if (channel >= NUM_COL_REGISTERS)
    {
        return 0;
    }
    uint8_t registerAddress = REG_COL_BASE + channel;
    return readRegister(registerAddress);
}

void AW210xx::setPhaseShift(bool en)
{
    if (en)
    {
        setRegister(REG_PHCR, B10000000);
    }
    else
    {
        setRegister(REG_PHCR, B00000000);
    }
}

void AW210xx::setSlewRate(uint8_t riseTime, uint8_t fallTime)
{
    byte value = B00000000;
    if (riseTime == 6)
        bitSet(value, 2);
    if (fallTime == 3)
        bitSet(value, 0);
    if (fallTime == 6)
        bitSet(value, 1);
    if (fallTime == 10)
    {
        bitSet(value, 0);
        bitSet(value, 1);
    }
    setRegister(REG_GCR4, value);
}

void AW210xx::setBri(uint8_t channel, uint8_t value)
{
    if (channel >= NUM_BR_REGISTERS)
    {
        return;
    }
    uint8_t registerAddress = REG_BR_BASE + channel;
    setRegister(registerAddress, value);
}

void AW210xx::setCol(uint8_t channel, uint8_t value)
{
    if (channel >= NUM_COL_REGISTERS)
    {
        return;
    }
    uint8_t registerAddress = REG_COL_BASE + channel;
    setRegister(registerAddress, value);
}

void AW210xx::setCombinedBri(uint8_t channel, uint16_t value)
{
    if (channel >= NUM_COL_REGISTERS || value > 510)
    {
        return;
    }
    uint8_t brightness1 = value / 2;
    uint8_t brightness2 = value / 2 + value % 2;

    uint8_t briRegisterAddress = REG_BR_BASE + channel;
    uint8_t colRegisterAddress = REG_COL_BASE + channel;
    setRegister(briRegisterAddress, brightness2);
    setRegister(colRegisterAddress, brightness1);
    update();
}

void AW210xx::setCombinedBriCIE(uint8_t channel, uint16_t value)
{
    setCombinedBri(channel, cie[value]);
}
/*
void AW210xx::setBalance(uint8_t r, uint8_t g, uint8_t b)
{
    setRegister(REG_WBR, r);
    setRegister(REG_WBG, g);
    setRegister(REG_WBB, b);
}
*/
void AW210xx::setFreq(PWMFrequency freq)
{
    uint8_t data = readRegister(REG_GCR);

    bitWrite(data, 4, bitRead(freq, 0));
    bitWrite(data, 5, bitRead(freq, 1));
    bitWrite(data, 6, bitRead(freq, 2));

    setRegister(REG_GCR, data);
}

void AW210xx::setChipEnable(bool enable)
{
    uint8_t data = readRegister(REG_GCR);

    bitWrite(data, 0, enable);

    setRegister(REG_GCR, data);
}

void AW210xx::setPowerSaving(bool enable)
{
    uint8_t data = readRegister(REG_GCR);

    bitWrite(data, 7, enable);

    setRegister(REG_GCR, data);
}

void AW210xx::update()
{
    setRegister(REG_UPDATE, 0);
}

void AW210xx::reset()
{
    setRegister(REG_RESET, 0);
}
