#include <Wire.h>
#include <LOLIN_I2C_MOTOR.h>

#define PWMFREQUENCY 1000

LOLIN_I2C_MOTOR motor(DEFAULT_I2C_MOTOR_ADDRESS);

void setup() {
  // put your setup code here, to run once:
  while (motor.PRODUCT_ID != PRODUCT_ID_I2C_MOTOR) {
    motor.getInfo();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  motor.changeFreq(MOTOR_CH_BOTH, PM_FREQUENCY);
  motor.changeStatus(MOTOR_CH_A, MOTOR_STATUS_CCW);
  for (int duty = 40; duty <= 100; duty += 1) {
    motor.changeDuty(MOTOR_CH_A, duty);
    delay(200);
  }
  motor.changeStatus(MOTOR_CH_A, MOTOR_STATUS_STANDBY);
  delay(500);
}
