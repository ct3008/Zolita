# Zolita
Witness Zolita, the trash loving fortune-teller, in all her glory. Made for Creative Embedded Systems final project using a TTGO-T1 ESP32 microprocessor, keypad and motor.

Creative documentation can be found here: https://www.notion.so/module-1/Zolita-c3b49b4c2d54498c8e1f9f5bb306ea52

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


2) Attach the 8 longer male-female wires to pins {2, 15, 13, 12} to represent the 4 rows in the keypad and then pins {32, 33, 25, 26} for the 4 columns. This should be connected in order of left to right on the keypad's adapter if the keypad is placed upright (the keypad is above the adapter) and the keypad is facing upwards. Directly connect the male end of the wire to the legs on the ESP32 and then place the female part into the keypad's adapter. Flash the **zolita.ino** code to this ESP32.
3) On a breadboard, place a second ESP32 and connect pins {32, 26, 27, 25} which are attached to the ULN2003 stepper motor driver and connect them in the order of IN1, IN3, IN2, and IN4. Then connect PWR and GND on the driver to the 5V and GND pins on the ESP32. Place a piece of paper on the arm of the motor so that as it moves, it can mimic the dispensing of a fortune. Run the motor-good.ino code to make the motor run in tandem. Ideally we would have used ESPNOW to communicate between the two ESP32s, but we didn't have time.


Building Zolita's enclosure:

We replicate a traditional Zoltar box design for Zolita's enclosure. Any enclosure that can hold Zolita, her hardware, and her prized trash collection is acceptable. Here are the sample proportions of Zolita's enclosure: 

<img width="420" alt="Screen Shot 2024-05-11 at 1 26 12 AM" src="https://github.com/ct3008/Zolita/assets/155119122/51286354-319e-4145-a6b2-49819bcc3441">

We used three 1 x 3 foot panels of 0.25 inch-thick wood. We used a laser cutter to extract each piece of Zolita's enclosure with precision. We built her box using a hammer and nails. We left the roof loose (unattached to the body of the box) so it could be easily removed for the adjustment of hardware. Then, we laser cut her name and headshot on a square panel (panel 5) and attached this to the top of her box. We cut and sewed curtains and spray-painted her box purple. We 3D printed Zolita, Zolita's trash, and Zolita's trash can, and painted Zolita to match raccoon fur patterns. Finally, we added a wooden bench which we propped Zolita and her trash on top of.

<img width="795" alt="Screen Shot 2024-05-11 at 1 45 09 AM" src="https://github.com/ct3008/Zolita/assets/155119122/1fe3b227-ce23-49b0-a13c-55642961e615">


