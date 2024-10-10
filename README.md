# Open Architecture Vat Photopolymerization (VPP) Bottom-Up System

This repository contains the Arduino code for an open-source, bottom-up vat photopolymerization (VPP) 3D printing system. The design is focused on flexibility and ease of customization for research and development purposes in additive manufacturing.

## Overview

The bottom-up VPP system is built on an open architecture, allowing users to modify and optimize the machine for various materials, curing profiles, and layer resolutions. This setup utilizes an Arduino microcontroller to control key components of the printer, including the z-axis motor and the light projector system, ensuring precise layer-by-layer photopolymerization.

## Features

- **Modular Design**: Easily integrate with different projectors and motors.
- **High Precision**: Supports precise control over curing and movement for high-resolution printing.
- **Customizable**: Open architecture allows for easy adaptation and optimization to fit different research and application needs.

## System Requirements

- **Microcontroller**: Arduino (compatible with Arduino Uno, Mega, etc.)
- **Motor Driver**: Compatible stepper motor driver DM320T
- **Actuator**: Linear actuator for the z-axis
- **Projector**: DLP projector
- **Power Supply**: 12V or 24V (depending on the motor and projector specifications)

## Wiring Diagram

Ensure you connect the components as follows:

1. Connect the stepper motor to the motor driver and then to the Arduino (check digital ports used are same as specified in the firmware). 
2. Connect the Arduino with a computer containing OpenVPP_software to control the light engine, via USB for powering and serial communication.
3. Power the stepper driver as per driver requirements.

## Installation and Setup

1. **Clone the Repository**:
   
   ```bash
   git clone https://github.com/DTUOpenAM/OpenVPP_Firmware.git
   cd OpenVPP_Software
   
3. **Configure Parameters**

- Modify the code to fit your specific setup, including motor steps, delays, motor connections, and driver connections.
- Ensure all settings are adjusted for your unique configuration.   

3. **Upload the Code**

- Open the Arduino IDE.
- Open the `Arduino_code.ino` file from this repository.
- Select the appropriate board and port from the **Tools** menu.
- Click **Upload** to upload the code to the Arduino.

## Contributing

Contributions are welcome! If you have improvements or new features to add, please fork the repository and submit a pull request. Be sure to document your changes and test them thoroughly.

