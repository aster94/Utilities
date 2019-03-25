#include <Utilities.h>

void setup()
{
  Serial.begin(115200);
  delay(1000);

  // Create a group of pin
  uint8_t group[] = {LED_BUILTIN, 12, 11, 10, 9, 8};

  // Print that group
  printArray(group, LEN(group));

  // Set their pinMode
  pinModeGroup(group, LEN(group), OUTPUT);

  // Write their state
  digitalWriteGroup(group, LEN(group), HIGH);
  delay(1000);
  digitalWriteGroup(group, LEN(group), LOW);

  // Toggle the state of a pin
  for (uint8_t n = 0; n < 5; n++)
  {
    digitalToggle(LED_BUILTIN);
    delay(500);
  }

  // Or toggle a group of pins
  for (uint8_t n = 0; n < 5; n++)
  {
    digitalToggleGroup(group, LEN(group));
    delay(500);
  }

  // Create a string
  char str[] = "hello, this is a test";

  // Split it and get the 4th part (starting from 0)
  char *substring = splitString(str, 4, " ,");

  // Print it
  Serial.print("substring: ");
  Serial.println(substring); // this will output "test"

  // A more complex array for printArray
  uint8_t array[] = {1, 0x56, 0b1011};

  // Print the array as hexadecimal values with a ":" between the items and invert it (from the last to the first)
  printArray(array, LEN(array), ":", HEX, true);

  // Print the array as binary values with a newline ("\n") between the items and write the index before every element
  printArray(array, LEN(array), "\n", BIN, false, true);

  // If you have more then one serial (like Serial2 or SoftwareSerial) you can make an echo between them
  //echo(&Serial, &Serial2);
}

void loop() {}