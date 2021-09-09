#include <Wire.h>
#include <paj7620.h>

#define GES_REACTIONE_TIME 500
#define GES_ENTRY_TIME 800
#define GES_QUIT_TIME 1000
#define I2C_ADDRESS 0x43
#define I2C_ADDRESS2 0x44

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  uint8_t error = paj7620Init();  //init Paj7620 registers
  if (error) {
    Serial.print("INIT ERROR,CODE: ");
    Serial.println(error);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t data = 0, data1 = 0, error;
  error = paj7620ReadReg(I2C_ADDRESS, 1, &data); // Read gesture result
  if (!error) {
    switch (data) {
      case GES_RIGHT_FLAG:
        Serial.println("Right");
        break;
      case GES_LEFT_FLAG:
        Serial.println("Left");
        break;
      case GES_UP_FLAG:
        Serial.println("Up");
        break;
      case GES_DOWN_FLAG:
        Serial.println("Down");
        break;
      case GES_FORWARD_FLAG:
        Serial.println("Forward");
        break;
      case GES_BACKWARD_FLAG:
        Serial.println("Backward");
        break;
      case GES_CLOCKWISE_FLAG:
        Serial.println("Clockwise");
      case GES_COUNT_CLOCKWISE_FLAG:
        Serial.println("Anti-clockwise");
        break;
      default:
        paj7620ReadReg(I2C_ADDRESS2, 1,&data);
        if (data == GES_WAVE_FLAG) {
          Serial.println("wave");
        } else {
          Serial.print(".");
        }
        break;
    }
  }
  delay(100);
}
