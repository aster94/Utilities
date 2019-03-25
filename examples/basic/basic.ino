#include <Utilities.h>

void setup()
{
  Serial.begin(115200);
  delay(1000);

  // Create a group of pins
  byte group[] = {LED_BUILTIN, 12, 11, 10, 9, 8};

  // Print that group
  printArray(group, LEN(group));

  // Set their pinMode
  pinModeGroup(group, LEN(group), OUTPUT);

  // Write their state
  digitalWriteGroup(group, LEN(group), HIGH);
  delay(1000);
  digitalWriteGroup(group, LEN(group), LOW);

  // Toggle the state of a pin
  for (byte n = 0; n < 5; n++)
  {
    digitalToggle(LED_BUILTIN);
    delay(500);
  }

  // Or toggle a group of pins
  for (byte n = 0; n < 5; n++)
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
  int array[] = {1, 11, 89, 34, 9};

  // Print the array as decimal values with a ", " between the items and invert them (from the last to the first)
  printArray(array, LEN(array), ", ", DEC, true);

  // Print the array as hexadecimal values with a ":" between the items and invert them (from the last to the first)
  printArray(array, LEN(array), ":", HEX, true);

  // Print the array as binary values with a newline ("\n") between the items and write the index before every element
  printArray(array, LEN(array), "\n", BIN, false, true);

  // If you have more then one serial (like Serial2 or SoftwareSerial) you can make an echo between them
  //echo(&Serial, &Serial2);
}

unsigned long task1, task2;

void loop()
{
  // This will be run every 3500 milliseconds (3.5 seconds)
  if (doEvery(&task1, 3500))
  {
    Serial.println("Hello, from task1");
  }

  // This every 10 seconds
  if (doEvery(&task2, 10000))
  {
    Serial.println("Also from task2!");
  }
}