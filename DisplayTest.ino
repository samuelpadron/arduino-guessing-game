#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define KEYA D3
#define KEYB D4
#define OLED_RESET 1  //GPIO1

Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  // put your setup code here, to run once:
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(1000);

  pinMode(KEYA, INPUT);
  pinMode(KEYB, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.println("NDL");
  display.setTextSize(1);
  display.print("Key A = ");
  display.println(digitalRead(KEYA));
  display.print("Key B = ");
  display.println(digitalRead(KEYB));
  display.display();
  delay(200);
}
