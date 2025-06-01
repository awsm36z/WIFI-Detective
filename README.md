# Wi-Fi Detective: LED Cipher Game (Arduino + USB Keyboard)

## What is this project?
This project is an interactive game that demonstrates how computers use codes to communicate, just like Wi-Fi! Instead of using invisible radio waves, we use colored lights (LEDs) to send secret messages. Your job is to decode the light patterns using a code chart, just like a computer would.

## How does it work?
- An Arduino controls 3 colored LEDs (Red, Green, Blue).
- The LEDs light up in a special sequence. Each group of lights stands for a letter or a part of the secret message.
- A USB keyboard is attached so you can interact with the game (for example, to enter special modes or change the message).

## Main Features
- The Arduino shows a coded message by flashing the LEDs in patterns.
- Players figure out what the message is by looking at the light sequence and using a cipher chart.
- Pressing certain keys (like **Shift + R**) on the keyboard can enter a "modify mode" to change the message or the coding method.

## Code Structure
- The Arduino code controls the LEDs, listens for keyboard input, and tracks the current step in the light sequence.
- Keyboard input is handled using a USB Host Shield, which lets the Arduino read from a standard USB keyboard.
- Special callback code (called a "parser") handles what happens when you press a key.
- Most of the time, the Arduino is either displaying the coded message or waiting for keyboard commands.

## Hardware Needed
- **Arduino Uno** (or similar)
- **USB Host Shield** (to connect a USB keyboard)
- **3 common anode RGB LEDs** (wired so each LED can be red, green, or blue)
- **Resistors** for each LED color pin
- **Breadboard and jumper wires**
- **USB keyboard**

## How to Use It
1. Plug the USB keyboard into the USB Host Shield.
2. Power the Arduino via USB or battery.
3. Open the Serial Monitor on your computer (**9600 baud**).
4. Watch the LEDs flash in a repeating pattern, that's the secret message!
5. Use the provided cipher chart to decode the message (each color pattern stands for a different letter or word).
6. Press **Shift+R** on the keyboard to enter modify mode (if enabled), and follow prompts to change the message or cipher.

## How to Change the Code or the Message
- The message can be changed in the code or by keyboard input (if supported).
- You can experiment with new ciphers by editing how the code maps color patterns to letters or words.

## Troubleshooting
- **LEDs not lighting up?** Double-check wiring and LED polarity.
- **Keyboard not working?** Try a different keyboard, some modern ones may not be compatible.
- Ensure the USB Host Shield is fully seated on the Arduino and has power.
- Open the Serial Monitor to see debug info and help messages.

## Credits
Developed by **Yassine El Yacoubi** for the Pacific Science Center.
Inspired by real-world Wi-Fi communication, explained in a fun, hands-on way!

## Contact / Questions
For questions, ideas, or contributions, contact the project maintainer or the Science Engagement team at the Pacific Science Center.
