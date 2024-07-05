# AW210xx Library for Arduino

The `AW210xx` library provides an interface to control AW21036 and AW21024 LED drivers via I2C on Arduino and compatible boards.

## Features

- 8-bit global brightness
- 8-bit individual brightness
- Secondary 8-bit individual brightness
- Programmable PWM frequency from 122hz to 62khz
- Phase shift
- Slew rate
- Atomatic power saving when all channels are off
- Programmable automatic breathing patterns (not implemented in this library)

## Compatilility
- Tested on ESP8266, should work on most Arduino compatible boards with Wire library.

## Installation

1. Download the zip archive of this repository.
2. Unzip the downloaded file and move it to your Arduino libraries directory, typically located in `Documents/Arduino/libraries`.
3. Rename the folder to `AW210xx` if it isn't already named that.

## Usage

Include the library in your sketch and initialize the device.

## API Reference

### Initialization

- `void begin(uint8_t addr = ADDR_AW, uint32_t i2cSpeed = I2C_SPEED, PWMFrequency freq = FREQ_2K, bool phaseShift = true)`
  - Initializes the device with the given I2C address, speed, frequency, and phase shift setting.

- `void update()`
  - Applies changes, required only when changing the brightness settings.

- `void reset()`
  - Resets the driver to factory defaults.

### Requesting data from the led driver

- `uint8_t getGlobalBri()`
  - Gets the global brightness value.

- `uint8_t getBri(uint8_t channel)`
  - Gets the brightness value for a specific channel (0-36 for AW21036, 0-24 for AW21024).

- `uint8_t getCol(uint8_t channel)`
  - Gets the secondary brightness value of a channel.

- `uint8_t readRegister(uint8_t reg)`
  - Reads a byte from the specified register.

### Updating data

- `void setGlobalBri(uint8_t value)`
  - Sets the global brightness.
  
- `void setBri(uint8_t channel, uint8_t value)`
  - Sets the brightness for a specific channel.

- `void setCol(uint8_t channel, uint8_t value)`
  - Sets the secondary brightness for a specific channel.

- `void setCombinedBri(uint8_t channel, uint16_t value)`
  - Combines both Col and Bri values, accepts values 0-510 (255+255)

- `void setCombinedBriCIE(uint8_t channel, uint16_t value)`
  - CIE adjusted brightness for smooth linear dimming, accepts values 0-255 (!!!)
  
- `void setPhaseShift(bool en)`
  - Enables or disables phase shift.

- `void setSlewRate(uint8_t riseTime, uint8_t fallTime)`
  - Configures the slew rate for rise and fall times. 
  - Possible values for riseTime: 1 (default), 6 (ns)
  - Possible values for fallTime: 1 (default), 3, 6, 10 (ns)

- `void setFreq(PWMFrequency freq)`
  - Sets the PWM frequency.

- `void setChipEnable(bool enable)`
  - Enables or disables the chip.

- `void setPowerSaving(bool enable)`
  - Enables or disables power saving mode.

- `void setRegister(uint8_t registerAddress, uint8_t value)`
  - Writes a byte to the specified register.

## License

This library is licensed under the MIT License. See the `LICENSE` file for more details.
