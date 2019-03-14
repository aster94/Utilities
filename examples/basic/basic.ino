#include <Utilities.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  uint8_t group[] = {LED_BUILTIN, 12, 11, 10, 9, 8};
  pinModeGroup(group, LEN(group), OUTPUT);

  digitalWriteGroup(group, LEN(group), HIGH);
  delay(1000);
  digitalWriteGroup(group, LEN(group), LOW);

  for (uint8_t n = 0; n < 5; n++) {
    digitalToggle(LED_BUILTIN);
    delay(500);
  }

  printArray(group, LEN(group));

  echo(&Serial, &Serial2);
}

void loop() {}
