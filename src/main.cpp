#include <Arduino.h>
#include <MsTimer2.h>
#include "../lib/FlexCAN_T4/CanBus/CanBus.h"
#include "../lib/GyemsRmd/GyemsRmd.h"

#if defined(__IMXRT1062__)
CanBus<CAN3, 10> &canbus = CanBus<CAN3>::getInstance();
#endif
#if defined(__MK20DX256__)
CanBus<CAN0> &canbus = CanBus<CAN0>::getInstance();
#endif

GyemsRmd motorX(0);

const int ledPin = 13;

uint8_t buf[8] = {};

void timerInt() {
  canbus.fetch();
  while (canbus.available(0x40)) {
    canbus.read(0x40, buf);
    Serial.print("g ");
    Serial.print(buf[0]);
    Serial.print("\t");

    buf[0]++;
  }
  canbus.stage(0x40, buf);
  canbus.write();
  Serial.print("s ");
  Serial.print(buf[0]);
  Serial.println();
  digitalWrite(ledPin, !digitalRead(ledPin));
}

void setup() {
  Serial.begin(2000000);
  pinMode(ledPin, OUTPUT);

  canbus.begin(1000000);

  MsTimer2::set(1, timerInt);
  MsTimer2::start();
}

void loop() {
  motorX.read(canbus);
  // Serial.println(buf[0]);
  delay(100);
}
