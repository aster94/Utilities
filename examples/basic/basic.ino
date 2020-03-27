#include <Arduino.h>
#include "Utilities.h"

void setup()
{
  Serial.begin(115200);
  delay(100);

  // Create a group of pins
  byte group[] = {LED_BUILTIN, 12, 11, 10, 9, 8};

  // Print this group
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

  // Another array for printArray
  int array[] = {1, 11, 89, 34, 9};

  // Print the array as decimal values with a ", " between the items and invert them (from the last to the first)
  printArray(array, LEN(array), ", ", DEC, true);

  // Print the array as hexadecimal values with a ":" between the items and invert them (from the last to the first)
  printArray(array, LEN(array), ":", HEX, true);

  // Print the array as binary values with a newline ("\n") between the items and write the index before every element
  printArray(array, LEN(array), "\n", BIN, false, true);

  // If you have more then one serial (like Serial2 or SoftwareSerial) you can make an echo between them
  //echo(&Serial, &Serial2);

  // Now some examples of string manipulation

  // The string we want to cute
  const char *long_text = "This is a very\n\
long text that you want\n\
to divide in lines";

  // Some values that we need to initialize
  char **lines = nullptr;
  size_t number_of_lines;

  // Finally we can split the string
  lines = stringSplit(&lines, &number_of_lines, long_text, "\n");

  // Print it line by line
  for (uint8_t n = 0; n < number_of_lines; n++)
  {
    Serial.println(lines[n]);
  }

  // Assign part of it to a new variable
  char *second_line = lines[1];

  // This way we delete the variable lines and the memory it was occupy
  free(lines);

  // Check if a string start with another
  if (stringStartWith(second_line, "long"))
  {
    Serial.println("Yes it is!");
  }

  // Cut/substring a piece of text
  second_line = stringCut(second_line, 0, -5);

  Serial.print("2° line after cut: ");
  Serial.println(second_line);

  // Check if a string ends with another
  if (!stringEndWith(second_line, "want"))
  {
    Serial.println("Too late, \"want\" was cutted!");
  }

  Serial.print("Sub string start at: ");
  Serial.println(stringSearch(long_text, "want"));

  Serial.println(stringReverse("em trever"));
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
