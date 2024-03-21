# Interfacing-to-Input-Output-Devices

# Overview
This project focuses on developing an embedded system using the Digilent Zybo Z7 development board, based on the AMD/Xilinx Zynq-7010 System-on-Chip (SoC). The system involves interfacing with various peripherals such as a 7-segment LED display, a keypad, pushbuttons, switches, LEDs, and an RGB LED.

# Learning Objectives
Gain experience designing a FreeRTOS application with tasks, delays, and queues.
Interface inputs from a keypad and pushbuttons.
Interface outputs to a 7-segment LED display and the console window in Xilinx Vivado SDK.
Implement a simple peripheral control system as an embedded system on the Zybo Z7.

# Hardware Requirements
Digilent Zybo Z7 development board.
Pmod peripherals: 2-digit 7-segment LED display, 16-button keypad module.
General-purpose pushbuttons and switches.
RGB LED and green LEDs.

# Software Requirements
Xilinx Vivado for hardware configuration.
Xilinx SDK for software development.
FreeRTOS for task management.

# Project Structure
Part 1: Interfacing with the 7-segment display and keypad.
Initialize SSD device and display keypad inputs on the SSD.
Implement sequential display of keypad inputs on both SSD digits.
Monitor and display system status changes.

Part 2: Interfacing with other peripherals.
Implement a peripheral control system using keypad inputs.
Control RGB LED settings and state.
Control green LEDs based on keypad commands.

# How to Run
Download the hardware configuration and software project files from the provided eClass site.
Open Vivado and export the hardware configuration to SDK.
Create a new SDK project and import the provided source files for each lab.
Compile and run the projects on the Zybo Z7 board.
