#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <hidboot.h>
#include <usbhub.h>
#include <Usb.h>



/**
 ***************************************************************************************************************
 ************************************** Keyboard Hardware Initialization ***************************************
 ***************************************************************************************************************
 */

USB Usb;
HIDBoot<USB_HID_PROTOCOL_KEYBOARD> Keyboard(&Usb);


class KbdRptParser : public KeyboardReportParser
{
  void OnKeyDown(uint8_t mod, uint8_t key);
};

KbdRptParser KbdPrs;

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  uint8_t c = OemToAscii(mod, key);
  if (c)
  {
    Serial.print("Key pressed: ");
    Serial.println((char)c);
    // Here, do whatever you want on keypress
  }
}

/**
 ***************************************************************************************************************
 ************************************* Encoding and Light Sequence Logic ***************************************
 ***************************************************************************************************************
 */

const int NUM_LEDS = 3; // Define number of LEDs
char encodedMessage[100];

// Store LED pin mappings
int ledPins[NUM_LEDS][3] = {
    {13, 12, 11}, // LED 1: Red, Green, Blue
    {9, 8, 6},    // LED 2: Red, Green, Blue
    {4, 3, 2}     // LED 3: Red, Green, Blue
};

// Define a struct for colors
struct Color
{
  int r, g, b;
};

// Declare color definitions using C-style strings
Color red = {255, 0, 0};
Color blue = {0, 0, 255};
Color green = {0, 255, 0};
Color white = {255, 255, 255};
Color off = {0, 0, 0};

const char RGB_CHARS[3] = {'R', 'G', 'B'};

void encode(const char *input, char *output)
{
  int index = 0;

  for (size_t i = 0; i < strlen(input); i++)
  {
    char letter = input[i];

    // Convert to uppercase (manual implementation)
    if (letter >= 'a' && letter <= 'z')
    {
      letter -= 32; // Convert lowercase to uppercase
    }

    // Ensure it's an uppercase letter A-Z
    if (letter >= 'A' && letter <= 'Z')
    {
      int letterIndex = letter - 'A'; // Convert A-Z to 0-25

      // Convert index to base-3 (RGB triplet)
      output[index++] = RGB_CHARS[(letterIndex / 9) % 3]; // First tier
      output[index++] = RGB_CHARS[(letterIndex / 3) % 3]; // Second tier
      output[index++] = RGB_CHARS[(letterIndex % 3)];     // Third tier
    }
  }

  // Append the end marker "BBB"
  output[index++] = 'B';
  output[index++] = 'B';
  output[index++] = 'B';
  output[index] = '\0'; // Null-terminate the output string
}

// Function to set an LED to a given color
void setColor(int led, char color)
{
  if (led < 1 || led > NUM_LEDS)
    return; // Validate LED index
  // Serial.print("LED ");
  // Serial.print(led);
  // Serial.print(" -> color: ");
  // Serial.println(color);
  // Select the right color using C-style string comparison
  Color chosenColor;
  if (color == 'R')
  {
    chosenColor = red;
  }
  else if (color == 'G')
  {
    chosenColor = green;
  }
  else if (color == 'B')
  {
    chosenColor = blue;
  }
  else
  {
    chosenColor = off; // Default to turning off the LED
  }

  // Write the RGB values to the LED's pins
  int index = led - 1; // Convert to zero-based index
  // Serial.println("LED " + String(index + 1) + " -> color: " + String(color) + " (R:" + String(chosenColor.r) + ", G:" + String(chosenColor.g) + ", B:" + String(chosenColor.b) + ")");
  digitalWrite(ledPins[index][0], chosenColor.r ? LOW : HIGH);
  digitalWrite(ledPins[index][1], chosenColor.g ? LOW : HIGH);
  digitalWrite(ledPins[index][2], chosenColor.b ? LOW : HIGH);
}
/**
 * * Function to display a sequence frame
 * * This function takes an input string and an index, and sets the LEDs to the corresponding colors
 * * * It reads three characters from the input string starting at the given index
 * * * and sets the LEDs to the colors represented by those characters.
 * * * It uses the setColor function to set the colors of the LEDs.
 */
void displaySequenceFrame(const char input[], size_t index)
{
  // Guard against reading beyond null terminator
  if (input[index] == '\0' || input[index + 1] == '\0' || input[index + 2] == '\0')
  {
    return;
  }

  setColor(1, input[index]);
  setColor(2, input[index + 1]);
  setColor(3, input[index + 2]);
}

// GLOBALS:
unsigned long lastUpdateTime = 0;
const unsigned long interval = 1000; // 1 second
size_t currentIndex = 0;
bool messageComplete = false;

/**
 ***************************************************************************************************************
 ********************************************** Main Loop + Setup **********************************************
 ***************************************************************************************************************
 */

/**
 * * Main loop function
 * function will continuously flash the LEDs in a sequence based on the encoded message.
 * IF shift+r is pressed from keyboard, it will break out into "modify mode"
 *  In "modify mode", the user can do multiple things:
 * 1: change the cipher/encoding by shifting the "wheel" up [n] % 26
 * 2: change the message by typing in a new message
 */
void loop()
{
  unsigned long currentTime = millis();

  if (!messageComplete && currentTime - lastUpdateTime >= interval)
  {
    // Check ahead for end of message
    if (encodedMessage[currentIndex + 2] == '\0')
    {
      messageComplete = true;
    }
    else
    {
      displaySequenceFrame(encodedMessage, currentIndex);
      lastUpdateTime = currentTime;
      currentIndex += 3;
    }
  }
  Usb.Task();
  // Later: check for keypresses here
}

void setup()
{
  Serial.begin(9600);
  // Set LED pins as outputs
  for (int i = 0; i < NUM_LEDS; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      pinMode(ledPins[i][j], OUTPUT);
    }
  }

  // Initialize USB Host Shield
  if (Usb.Init() == -1)
  {
    Serial.println("USB Host Shield initialization failed!");
    while (1)
      ; // halt execution if shield not found
  }
  delay(200); // Give shield time to initialize

  // Attach the parser to the keyboard
  Keyboard.SetReportParser(0, &KbdPrs);

  encode("BBBBB", encodedMessage);
  // Serial.println("Encoded message:");
  // Serial.println(encodedMessage);
}
