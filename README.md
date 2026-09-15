# 🎮 Gaming Hub – STM32 Embedded Gaming Platform

Gaming Hub is an STM32-based embedded gaming platform that combines multiple interactive games with a TFT display, LED matrix, physical controls, infrared remote control, and audio output.

The project demonstrates practical embedded-systems concepts through a modular hardware and software architecture.

## 🎯 Project Overview

Gaming Hub provides a collection of four games controlled through physical buttons or an IR remote:

- 🧮 Math Quiz
- 🧠 Memory Match
- 🐍 Snake
- 🎵 Music Maker

The system uses an ST7735S TFT display as the main graphical interface while an 8×8 LED matrix provides additional visual feedback.

Audio output is generated through a resistor-ladder DAC connected to an audio jack.

## 🎮 Games

### 🧮 Math Quiz

A mathematical challenge where the player answers generated questions using the available controls.

Difficulty levels:

- Easy
- Medium
- Hard

### 🧠 Memory Match

A memory-based game where the player must remember and match displayed patterns.

The difficulty level affects the timing of the game.

### 🐍 Snake

A classic Snake game implemented on the 8×8 LED matrix.

The snake movement speed changes according to the selected difficulty.

### 🎵 Music Maker

A simple interactive music system that allows the player to select and play musical notes.

The system supports two octaves:

- DO4
- RE4
- MI4
- FA4
- SOL4
- LA4
- SI4
- DO5
- RE5
- MI5
- FA5
- SOL5
- LA5
- SI5

## ⚙️ Hardware

| Component | Description |
|---|---|
| STM32 | Main microcontroller |
| ST7735S TFT | Main graphical display |
| 8×8 LED Matrix | Game and status visualization |
| 74HC595 ×2 | Shift registers for LED matrix control |
| IR Receiver | Wireless remote control input |
| IR Remote | Remote navigation and game control |
| Push Buttons | Physical user controls |
| Buzzer | Sound indication |
| Resistor Ladder DAC | Audio signal generation |
| Audio Jack | External audio output |

## 🔌 Pin Configuration

### User Controls

| Function | STM32 Pin |
|---|---|
| UP | PB0 |
| DOWN | PB1 |
| SELECT | PB3 |
| IR Receiver | PB7 |
| Buzzer | PB6 |

### 74HC595 Shift Registers

| Signal | STM32 Pin |
|---|---|
| DATA | PA0 |
| CLOCK | PA1 |
| LATCH | PA2 |

### Audio

| Function | STM32 Pin |
|---|---|
| Audio DAC Output | PC15 |

## 🎛️ Controls

### Physical Buttons

| Button | Function |
|---|---|
| UP | Move selection up |
| DOWN | Move selection down |
| SELECT | Select / Enter |

### IR Remote

| Remote Button | Function |
|---|---|
| PREV | UP |
| NEXT | DOWN |
| SELECT | SELECT |

The IR receiver uses NEC protocol decoding.

## 🖥️ Display Modes

Gaming Hub supports two display modes:

### BOTH

Uses both:

- ST7735S TFT
- 8×8 LED matrix

### TFT ONLY

Uses the TFT display while disabling the LED matrix visualization.

## 🔊 Audio System

The audio system is implemented using a software-generated waveform and a resistor-ladder DAC.

The system uses:

- 8 kHz sample rate
- Timer-based audio generation
- Phase accumulator
- Multiple musical note frequencies
- External resistor ladder
- Audio jack output

The project also includes a Sound ON/OFF setting.

## 🎚️ Difficulty Levels

The system provides three difficulty levels:

- Easy
- Medium
- Hard

Difficulty affects game behavior such as:

- Memory game timing
- Snake movement speed
- Math Quiz difficulty

## ⚙️ Settings

The Settings menu provides:

1. Sound
2. Difficulty
3. Display
4. High Scores
5. Reset Scores
6. Back

## 🏗️ Software Architecture

The project follows a modular embedded-driver architecture.

Main modules include:

- GPIO
- RCC
- NVIC
- EXTI
- TIM
- SysTick
- SPI
- ST7735S
- LED Matrix
- IR
- Audio
- DAC

The application logic is implemented in `main.c`.

## 📁 Project Structure

```text
Gaming_Hub/
│
├── include/
│   ├── GPIO_interface.h
│   ├── RCC_interface.h
│   ├── NVIC_interface.h
│   ├── EXTI_interface.h
│   ├── TIM_interface.h
│   ├── SysTick_interface.h
│   ├── SPI_interface.h
│   ├── ST7735S_interface.h
│   ├── LED_MATRIX_interface.h
│   ├── IR_interface.h
│   ├── AUDIO_interface.h
│   └── ...
│
├── src/
│   ├── main.c
│   ├── GPIO_program.c
│   ├── RCC_program.c
│   ├── NVIC_program.c
│   ├── EXTI_program.c
│   ├── TIM_program.c
│   ├── SysTick_program.c
│   ├── SPI_program.c
│   ├── ST7735S_program.c
│   ├── LED_MATRIX_program.c
│   ├── IR_program.c
│   ├── AUDIO_program.c
│   └── ...
│
├── system/
├── ldscripts/
├── .project
├── .cproject
└── README.md

## 🛠️ Development Environment

The project was developed using:

- STM32 ARM Microcontroller
- Eclipse-based embedded development environment
- C Programming Language
- ARM GCC / embedded toolchain
- Git & GitHub

## 🚀 Build and Run

1. Clone the repository.
2. Import the project into the Eclipse embedded development environment.
3. Build the project.
4. Connect the STM32 hardware.
5. Program the microcontroller.
6. Connect the TFT, LED matrix, controls, IR receiver, and audio circuit.
7. Run Gaming Hub.

## 🧩 Embedded Concepts Demonstrated

This project demonstrates practical implementation of:

- GPIO configuration and control
- Timers
- External interrupts
- NVIC interrupt management
- SPI communication
- TFT display control
- Shift-register interfacing
- LED matrix multiplexing
- IR remote decoding
- NEC infrared protocol
- Software audio generation
- DAC signal generation
- Game-state management
- Menu navigation
- Embedded modular software architecture

## 🔮 Future Improvements

Possible future improvements include:

- Additional games
- More advanced graphics
- Improved audio effects
- Persistent high-score storage
- Additional wireless controllers
- Improved game animations
- More advanced user interface

## 📌 Project Status

**Current Status: Working Prototype**

The core Gaming Hub system, games, controls, display system, IR remote control, LED matrix, and audio functionality have been implemented and tested on the STM32 platform.

## 👨‍💻 Team

- **Mohammed Wael Nabil**
- **Youssef Sherif Abd Elgawwad**
- **Ahmed Khaled Abdelaziz**