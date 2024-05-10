# Zolita
Witness Zolita, the trash loving fortune-teller, in all her glory. Made for Creative Embedded Systems final project using a TTGO-T1 ESP32 microprocessor, keypad and motor.

Technical Documentation:

Required Materials:
1) 2 TTGO T1 ESP32 microprocessors
2) 1 4*4 Matrix Array Keypad
3) 1 Arduino Step Motor
4) 2 USB-C cables
5) 14+ male-female cables (8 of which should be relatively long for the keypad), 2 female-female cables
6) Plethora of tape, wood, fabric and nylon filament

Required Software:
1) Arduino IDE (import Keypad.h and ArdunioJSON.h)
2) Create ChatGPT API key
  3) Go to https://platform.openai.com/docs/overview
  4) New users get $5 of free computing credit
  5) Go to "API Keys" in the sidebar
  6) Create a new secret key

Overall tutorial: 
1) Connect ESP32 to wifi and make a variable to store your ChatGPT API secret key
<img width="595" alt="Screenshot 2024-05-09 at 10 59 17 PM" src="https://github.com/ct3008/Zolita/assets/109620408/5c66656e-90d9-46e0-a086-1d7132530bee">
2) Attach the 8 longer male-female wires to pins {2, 15, 13, 12} to represent the 4 rows in the keypad and then pins {32, 33, 25, 26} for the 4 columns. This should be connected in order of left to right on the keypad's adapter if the keypad is placed upright (the keypad is above the adapter) and the keypad is facing upwards. Directly connect the male end of the wire to the legs on the ESP32 and then place the female part into the keypad's adapter. Flash the zolita_combined.ino code to this ESP32.
3) On a breadboard, place a second ESP32 and connect pins {32, 26, 27, 25} which are attached to the ULN2003 stepper motor driver and connect them in the order of IN1, IN3, IN2, and IN4. Then connect PWR and GND on the driver to the 5V and GND pins on the ESP32. Place a piece of paper on the arm of the motor so that as it moves, it can mimic the dispensing of a fortune. 
