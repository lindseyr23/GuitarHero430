# Guitar Hero 430




### Rice University ELEC 327 Final Project Spring 24

## Project Description 
This project aims to 


--- 

### Team Members
- Natalia Mendiola | nm58@rice.edu
- Lindesy Russ     | ltr1@rice.edu
---

### Materials

* 2x TI LaunchPad Kit with MSP430 (MSP-EXP430G2ET)
* SK9822 LED Strips
* SunFounder I2C LCD1602 Module
* Solderless Breadboard
* 2x 4.4k Resistors
* Jumper wires

### Schematic
<p align="center">
  <img src="./Schematic_guitarhero.png" alt="Schematic">
</p>

---

## Repository Structure

#### Note:
Consider adding STL files or additional notes here.

### Board_1

* **Game Logic:**
  - [main.c](./Experimental_Work/game_logic/main.c): File handling the game logic.
  - [rgb_interface_user.c](./Experimental_Work/game_logic/rgb_interface_user.c): File handling the RGB interface for user interaction.
  - [rgb_interface_user.h](./Experimental_Work/game_logic/rgb_interface_user.h): Header file for RGB interface.

* **Serial Communication:**
  - [serial_comm.c](./Experimental_Work/game_logic/serial_comm.c): File handling serial communication.
  - [serial_comm.h](./Experimental_Work/game_logic/serial_comm.h): Header file for serial communication.

### Board 2

* **Game Display Logic:**
  - [main.c](./Experimental_Work/board_2/main.c): File handling the game logic.
  - [rgb_interface.c](./Experimental_Work/board_2/rgb_interface.c): File handling the RGB interface.
  - [rgb_interface.h](./Experimental_Work/board_2/rgb_interface.h): Header file for RGB interface.

* **Audio:**
  - [PlaySong.c](./Experimental_Work/board_2/PlaySong.c): File handling audio playback.
  - [PlaySong.h](./Experimental_Work/board_2/PlaySong.h): Header file for audio playback.

### Experimental_Work 
<i>Please ignore this folder. This is code that is currently in progress. </i>

---

## References

1. 
