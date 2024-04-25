# Guitar Hero 430
### Rice University ELEC 327 Final Project Spring '24

## Table of Contents
1. [Project Description](#project-description)
2. [Team Members](#team-members)
3. [Materials](#materials)
4. [File Descriptions](#file-descriptions)
5. [Setting up the Hardware](#setting-up-the-hardware)
   - [Schematic](#schematic)
   - [LED Game Display Setup Guide](#led-game-display-setup-guide)
   - [LED User Display Setup Guide](#led-user-display-setup-guide)
   - [3D-printed Strummer]()
   - [3d-printed LED game display]()
6. [References](#references)

## Project Description 
This project aims to ...

### Team Members
- Natalia Mendiola | nm58@rice.edu
- Lindsey Russ     | ltr1@rice.edu
  
---
### Materials

* 2x TI LaunchPad Kit with MSP430 (MSP-EXP430G2ET)
* 2x 330 Ω Resistors
* 1x SunFounder I2C LCD1602 Module
* SK9822 LED Strips
* Solderless Breadboard
* Jumper wires
* Enameled wire

### Required Software Packages
* Python 3.x.x
* Serial 3.x.x
* python-vlc x.x.x
* (add requirements.txt that pip installs things?)
----
## File Descriptions

### Board_1

* **Game Logic:**
  - [main.c](./board1/main.c): File with game logic handling user inputs.
 
* **User LED Display Logic:**
  - [rgb_interface_user.c](./board1/rgb_interface_user.c): File handling the RGB interface for user interaction.
  - [rgb_interface_user.h](./board1/rgb_interface_user.h): Header file for RGB interface.

* **Scoreboard LCD Display Logic:**
  - [i2c.c](./board1/i2c.c):
  - [i2c.h](./board1/i2c.h):
    
  - [lcd.c](./board1/lcd.c):
  - [lcd.h](./board1/lcd.h):

### Board 2

* **XXXX Logic:**
  - [main.c](./board2/main.c): File handling 

* **LED Game Display Logic:**
  - [rgb_interface.c](./board2/rgb_interface.c): File handling 
  - [rgb_interface.h](./board2/rgb_interface.h): Header file for

* **Audio:**
  - [receive_audio.py](./board2/receive_audio.py): 
  - [PlaySong.c](./board2/PlaySong.c): File handling 
  - [PlaySong.h](./board2/PlaySong.h): Header file for
  - [Rock Band 4  Fortunate Son  Creedence Clearwater Revival  Full Band HD.mp3](./board2/Rock Band 4  Fortunate Son  Creedence Clearwater Revival  Full Band HD.mp3): Song 1
  - [](): Song 2


### Experimental_Work 
<i>Please ignore this folder. This is code that is currently in progress. </i>

---
## Setting up the hardware

### Schematic
![Schematic](./images/Schematic_guitarhero.png)

### LED Game Display Setup Guide
<img src="./images/game_led_display.jpg" alt="LED Game Display" width="25%"/>

#### Components Needed:
- SK9822 LED strips
- Jumper wires (male-to-female)
- Enameled wire
- Heat shrink tubing
- Electrical Tape

#### Instructions:
1. **Prepare the LED Strip:**
   - Cut five 7-LED long strips from the SK9822 LED strip.
   
2. **Solder Jumper Wires:**
   - For only ONE LED strip, solder four jumper wires to one end:
     - G (Ground)
     - C (Clock)
     - D (Data)
     - 5V (Power)
   
3. **Attach Female Connectors:**
   - Solder female connectors to the other end of the jumper wires for this LED strip.

4. **Connect LED Strips:**
   - For the remaining four strips, connect each strip to the previous one using enameled wire:
     - Connect G to G, C to C, D to D, and 5V to 5V.
     - Hide the enameled wire connections behind the LED strips.
     - Daisy-chain the five LED strips together, ending with the jumper wires side.


5. **Secure Connections:**
   - Use electrical tape to secure the enameled wire connections behind the LED-facing side of the LED strips, forming a 5x7 LED rectangular shape.
   - Use heat shrink tubing to insulate and secure the jumper wire soldered connections.

### LED User Display Setup Guide
<img src="./images/user_led_display.jpg" alt="LED User Display" width="25%"/>


#### Components Needed:
- SK9822 LED strips
- Jumper wires (male-to-female)
- Heat shrink tubing

#### Instructions:
1. **Prepare the LED Strip:**
   - Cut a 5 LED long strip from the SK9822 LED strip.
   
2. **Solder Jumper Wires:**
   - Solder four jumper wires to one end of the LED strip:
     - One wire for G (Ground)
     - One wire for C (Clock)
     - One wire for D (Data)
     - One wire for 5V (Power)
   
3. **Attach Female Connectors:**
   - Solder female connectors to the other end of the jumper wires.
   
4. **Secure Connections:**
   - Use heat shrink tubing to insulate and secure the soldered connections.

#### Usage:
- Connect the female connectors to the corresponding male connectors on your control board or microcontroller.

#### Notes:
- Handle the soldering iron and heat shrink tubing carefully to prevent injuries or damage to components.

---

## References

1. **ADD GUITAR STRUM LINK HERE**
2. - Modified code by Dr.Joseph Young based on [I2C Display and MSP430](https://github.com/andrei-cb/I2C-Display-and-MSP430/tree/master) by Andrei-cb.
