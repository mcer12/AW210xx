/*
  Arduino library for AW21036 and AW21024 LED controllers
*/

#include <Wire.h>
#include <AW210xx.h>

AW210xx aw210xx;

/*

  Available addresses:
  0x34 (AD to GND) - default
  0x35 (AD to VDD)
  0x36 (AD to SCL)
  0x37 (AD to SDA)

  Available PWM frequencies:
  FREQ_62K
  FREQ_32K
  FREQ_4K
  FREQ_2K - default
  FREQ_1K
  FREQ_500
  FREQ_244
  FREQ_122

*/

void setup() {
  //aw210xx.begin(ADDR_AW, I2C_SPEED, FREQ_2K, true); // Address, I2C speed in hz, pwm frequency, phase shift enable/disable
  aw210xx.begin();

  //aw210xx.setFreq(FREQ_122); // set frequency, see accepted values in the comment above setup()
  //aw210xx.setChipEnable(false); // disable the chip
  //aw210xx.setPowerSaving(false); // disable power saving (enabled by default)
  //aw210xx.setPhaseShift(false); // disable phase shift (enabled by default)
  
  //aw210xx.setSlewRate(uint8_t riseTime, uint8_t fallTime); // slew rate, possible values for riseTime: 1 (default), 6 (ns), possible values for fallTime: 1 (default), 3, 6, 10 (ns)
  //aw210xx.reset(); // resets the chip to factory defaults, run begin() again to re-initialize

  Serial.begin(115200);

  // This can be used to test i2c response from the chip
  // and should always return 0x18
  uint8_t verValue = aw210xx.readRegister(REG_RESET);
  Serial.print("Chip id: ");
  Serial.println(verValue, HEX);

  aw210xx.setGlobalBri(100); // global brightness setting, accepts values 0-255

  aw210xx.setBri(1, 255); // 8bit value for channel brightness, needs to run update() to take effect, accepts values 0-255
  aw210xx.setCol(1, 255); // 8bit value for secondary channel brightness / rgb mixing, doesn't need update(), accepts values 0-255
  aw210xx.setCombinedBri(2, 510); // Combines both Col and Bri values, accepts values 0-510 (255+255)
  aw210xx.setCombinedBriCIE(3, 255); // CIE adjusted brightness for smooth linear dimming, accepts values 0-255 (!!!)

  aw210xx.update();

  Serial.print("Global brightness: ");
  Serial.println(aw210xx.getGlobalBri());

  for (int i = 0; i < 36; i++) {
    Serial.print("CH");
    Serial.print(i);
    Serial.print(" bri:");
    Serial.print(aw210xx.getBri(i));
    Serial.print(", value:");
    Serial.println(aw210xx.getCol(i));
  }

}

void loop() {
  Serial.print("loop");

  for (int i = 0; i < 256; i++) {
    aw210xx.setCombinedBriCIE(0, i);
    delay(20);
  }
  for (int i = 0; i < 256; i++) {
    aw210xx.setCombinedBriCIE(0, 255 - i);
    delay(20);
  }

  /*
    for (int i = 0; i < 36; i++) {
      for (int ii = 0; ii < 36; ii++) {
        aw210xx.setBri(ii, 0);
        aw210xx.setCol(ii, 0);
      }

      aw210xx.setBri(i, 255);
      aw210xx.setCol(i, 255);

      aw210xx.update();

      delay(500);
    }

    for (int ii = 0; ii < 36; ii++) {
      aw210xx.setBri(ii, 0);
      aw210xx.setCol(ii, 0);
    }
    aw210xx.update();

  */
}
