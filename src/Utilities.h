#ifndef Utilities_h
#define Utilities_h

#include <Arduino.h>

#if defined(ARDUINO_ARCH_SAMD)
#define UniversalSerial Serial_
#else
#define UniversalSerial HardwareSerial
#endif

// MACROS
#define LEN(x) ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x]))))) // complex but safe macro for the lenght
#define TO_FAHRENHEIT(x) x * 1.8 + 32
#define TO_CELSIUS(x) (x - 32) * 0.55
#define maybe 2 // true or false is not enough

// Run a task every given time without blocking the rest of the code
template <class T1, class T2>
bool doEvery(T1 *start_time, T2 interval)
{
  if (millis() > *start_time + interval)
  {
    *start_time = millis();
    return true;
  }
  else
  {
    return false;
  }
}

// Change the state of a group of pins
// The template is needed for compatibility with STM32 core
template <class T>
void pinModeGroup(uint8_t pins[], size_t len, T state)
{
  for (uint8_t i = 0; i < len; i++)
  {
    pinMode(pins[i], state);
  }
}

// Write the state of a group of pins
void digitalWriteGroup(uint8_t pins[], size_t len, uint8_t state)
{
  for (uint8_t i = 0; i < len; i++)
  {
    digitalWrite(pins[i], state);
  }
}

// Toggle the state of a pin
void digitalToggle(uint8_t pin) { digitalWrite(pin, !digitalRead(pin)); }

// Toggle the state of a group of pins
void digitalToggleGroup(uint8_t pins[], size_t len)
{
  for (uint8_t i = 0; i < len; i++)
  {
    digitalWrite(pins[i], !digitalRead(pins[i]));
  }
}

// Echo between two serial ports, bi or mono directional
void echo(UniversalSerial *one, UniversalSerial *two,
          bool mono_directional = false)
{
  if (one->available())
  {
    two->write(one->read());
  }
  if (mono_directional == false)
  {
    if (two->available())
    {
      one->write(two->read());
    }
  }
}

// Print an array of any kind
template <class T>
void printArray(T array, size_t len, const char *delimiter = "\n",
                uint8_t formatter = DEC, bool invert = false,
                bool index = false, UniversalSerial *_port = &Serial)
{
  if (len >= 65535)
  {
    _port->println("ARRAY TOO BIG");
  }

  if (invert == false)
  {
    for (uint16_t i = 0; i < len; i++)
    {
      if (index)
      {
        _port->print(i);
        _port->print(": ");
      }
      _port->print(array[i], formatter);
      if (i < len - 1)
      {
        _port->print(delimiter);
      }
    }
  }
  else
  {
    for (uint16_t i = len; i > 0; i--)
    {
      if (index)
      {
        _port->print(i - 1);
        _port->print(": ");
      }
      _port->print(array[i - 1], formatter);
      if (i > 1)
      {
        _port->print(delimiter);
      }
    }
  }
  _port->println();
}

// The following functions are useful to work with char array
// without loosing hours debugging memory leak
/**
 * @brief Revert a char array
 *
 * @param source The char to revert
 * @return char* the reverted char array
 */
char *stringReverse(const char *source)
{
  uint8_t str_len = strlen(source);
  char *str = new char[str_len];
  strcpy(str, source);

  char temp;
  for (uint8_t i = 0; i < str_len / 2; i++)
  {
    temp = str[i];
    str[i] = str[str_len - i - 1];
    str[str_len - i - 1] = temp;
  }

  return str;
}

/**
 * @brief Split a char array in sub strings
 *
 * @param dest_arr the 2D array where to save the substrings
 * @param len_dest_arr a value where to store the number of substrings
 * @param str the string to split
 * @param the chars used as delimiters
 * @return char** same as dest_arr
 */
char **stringSplit(char ***dest_arr, size_t *len_dest_arr, const char *str,
                   const char *delimiters)
{
  int str_len = strlen(str) + 1; // add null terminator
  char str_copy[str_len];        // we work on a copy
  strcpy(str_copy, str);

  *dest_arr = (char **)malloc(sizeof(char *) * str_len); // over size

  uint8_t counter = 0;                        // limited to 255 sub strings
  char *token = strtok(str_copy, delimiters); // split until first token
  while (token != nullptr)
  {
    (*dest_arr)[counter] =
        (char *)malloc(sizeof(char) * strlen(token) + 1); // add null terminator
    strcpy((*dest_arr)[counter], token);                  // copy token to dest_array
    token = strtok(NULL, delimiters);                     // continue splitting
    counter++;
  }

  *dest_arr = (char **)realloc(
      (*dest_arr),
      sizeof(char *) * counter); // reallocate the right amount of memory
  *len_dest_arr = counter;       // save size
  return (*dest_arr);
}

/**
 * @brief Check if a string start with a given string
 *
 * @param The char* to to check
 * @param The characters looked for
 * @return true if char* starts with the given char
 * @return false if char* doesn't starts with the given char
 */
bool stringStartWith(const char *str, const char *prefix)
{
  size_t strLen = strlen(str);
  size_t prefixLen = strlen(prefix);
  if (prefixLen <= strLen)
  {
    return strncmp(str, prefix, prefixLen) == 0;
  }
  return 0;
}
/**
 * @brief Check if a string ends with a given string
 *
 * @param The char* to to check
 * @param The characters looked for
 * @return true if char* ends with the given char
 * @return false if char* don't ends with the given char
 */
bool stringEndWith(const char *str, const char *suffix)
{
  size_t strLen = strlen(str);
  size_t suffixLen = strlen(suffix);
  if (suffixLen <= strLen)
  {
    return strncmp(str + strLen - suffixLen, suffix, suffixLen) == 0;
  }
  return 0;
}

/**
 * @brief Search a sub string in a string
 *
 * @param The string in which we are searching
 * @param The sub string to find
 * @param The starting position from where to search
 * @return The index of the sub string
 */
int16_t stringSearch(const char *string, const char *sub_string, int16_t start = 0)
{
  char *index;
  if (start == -1)
  {

    index = strstr(stringReverse(string), sub_string);
    if (index == nullptr)
    {
      return -1;
    }

    return strlen(index) - 1;
  }
  index = strstr((char *)string, sub_string);
  if (index != nullptr)
  {
    while ((index - string) < start)
    {
      index = strstr((char *)string + start, sub_string);
      if (index == nullptr)
      {
        return -1;
      }
    }
    return index - string;
  }
  return -1;
}

/**
 * @brief Cut a given string from start to end
 *
 * @param The char array to cut
 * @param The starting point, accept also negative value
 * @param The ending point, accept also negative value
 * @return The resulting char array
 */
char *stringCut(const char *str, int16_t start, int16_t end)
{
  // print << "Cutting: \"" << str << "\" from: " << (int) start << " to " <<
  // (int)end << "\n";
  uint16_t len = strlen(str);
  int16_t offset = start;

  // print << "str len: " << (int)len << "\n";

  if (end < 0) // start from the back
  {
    end = len + end;
    // print << "new end: " << (int)end << "\n";
  }
  if (start < 0) // start from the back
  {
    start = len + start;
    // print << "new start: " << (int)start << "\n";
    offset = start;
  }
  if (start > end || start == end) // return an empty char
  {
    // print << "what?\n";
    return (char *)'\0';
  }
  if (end > (int16_t)len) // cut until the end
  {
    // print << "out\n";
    end = len;
  }

  char *cutted = (char *)malloc(
      sizeof(char) * (end - start));          // allocate the right amount of memory
  strncpy(cutted, str + start, end - offset); // make the cut
  cutted[end - start] = '\0';                 // add null-terminator

  return cutted;
}

#if defined(_GLIBCXX_VECTOR) && defined(_GLIBCXX_STRING)

using std::string;
using std::vector;

/**
 * @brief Similar to the stringSplit() but using std::string
 *
 * @param The std::string to split
 * @param A std::string of one or more delimeters
 * @return A std::vector
 */
vector<string> stringSplit(const string &src, const string &delimiters)
{
  vector<string> result;
  size_t start = 0;
  for (size_t end = src.find_first_of(delimiters, start); end != string::npos;
       start = end + 1, end = src.find_first_of(delimiters, start))
  {
    if (end != start)
    {
      result.push_back(src.substr(start, end - start));
    }
  }
  if (start < src.size())
  {
    result.push_back(src.substr(start));
  }
  return result;
}

#endif // _GLIBCXX_VECTOR && _GLIBCXX_STRING

#endif // Utilities_h
