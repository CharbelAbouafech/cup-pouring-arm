# Cup Pouring Arm

# Authors
Javier Narvaez
Charbel Abou Afech

## Project Overview

The Cup Pouring Arm project is an autonomous robotic system built using the MSP432 microcontroller. The system combines reflectance sensor feedback, finite state machine (FSM) logic, PWM motor control, and servo actuation to navigate a predefined path and perform automated pouring actions.

The robot continuously reads reflectance sensor data to detect line position, intersections, and dead ends while adjusting movement in real time. Servo motors are controlled using PWM signals to perform arm movement and pouring actions. System behavior can also be modified through PMOD switch inputs, allowing different operational modes during demonstrations and testing.

### Main Features

- Autonomous line-following navigation
- FSM-based movement and decision making
- PWM-controlled servo operation
- Reflectance sensor processing
- Timer interrupt-driven sampling
- Switch-controlled operating modes
- Intersection and dead-end handling

---

## System Architecture

The system is organized into several functional modules working together under the MSP432 microcontroller.

### Control Flow

1. Timer interrupt triggers periodic sensor sampling
2. Reflectance sensor data is processed
3. FSM determines the robot state
4. Motor and servo PWM outputs are updated
5. LEDs and switch inputs provide system feedback and mode selection

### Functional Modules

#### Sensor Module

The reflectance sensor array detects line position and surface transitions. Sensor outputs are interpreted as bit patterns to classify movement conditions such as:

- Center alignment
- Slight left/right deviation
- Sharp turns
- T-intersections
- Dead ends

#### Finite State Machine (FSM)

The FSM controls navigation behavior and robot decisions.

##### FSM States

- `FOLLOW_LINE` — Default line tracking
- `CHECK_INTERSECTION` — Validates intersection conditions
- `TURN_LEFT_STATE` — Executes left turn
- `TURN_RIGHT_STATE` — Executes right turn
- `TURN_AROUND` — Reverses direction
- `DEAD_END` — Stops system operation

#### Motor and Servo Control

PWM signals generated using Timer_A2 control the servos responsible for robotic arm movement and pouring actions.

#### Timing and Interrupts

Timer_A1 generates interrupts approximately every 10 ms for periodic sensor sampling and FSM updates.

---

## Interfaces and Peripherals

The project integrates several peripherals connected to the MSP432 microcontroller.

### PMOD Switch Interface

| Component | Description | MSP432 Pin |
|---|---|---|
| SWT1 | Switch 1 | P10.0 |
| SWT2 | Switch 2 | P10.1 |
| SWT3 | Switch 3 | P10.2 |
| SWT4 | Switch 4 | P10.3 |
| PMOD Pin 5 | Ground | GND |
| PMOD Pin 6 | Power | VCC (3.3V) |

### Servo Motor Interface

| Component | Signal | MSP432 Pin |
|---|---|---|
| Servo 1 | PWM Input | P5.6 (PM_TA2.1) |
| Servo 1 | Power | External 5V |
| Servo 1 | Ground | GND |
| Servo 2 | PWM Input | P5.7 (PM_TA2.2) |
| Servo 2 | Power | External 5V |
| Servo 2 | Ground | GND |

### Reflectance Sensor Interface

| Component | Signal | MSP432 Pin |
|---|---|---|
| Reflectance Sensor Output 0 | Channel 0 | P7.0 |
| Reflectance Sensor Output 1 | Channel 1 | P7.1 |
| Reflectance Sensor Output 2 | Channel 2 | P7.2 |
| Reflectance Sensor Output 3 | Channel 3 | P7.3 |
| Reflectance Sensor Output 4 | Channel 4 | P7.4 |
| Reflectance Sensor Output 5 | Channel 5 | P7.5 |
| Reflectance Sensor Output 6 | Channel 6 | P7.6 |
| Reflectance Sensor Output 7 | Channel 7 | P7.7 |
| Reflectance Sensor | Control Even | P5.3 |
| Reflectance Sensor | Control Odd | P9.2 |
| Reflectance Sensor | Power | 5V |
| Reflectance Sensor | Ground | GND |

---

## Verification and Testing

The system was tested incrementally to validate sensor functionality, FSM behavior, servo control, and overall robotic operation.

### Sensor Testing

- Verified reflectance sensor readings across different line positions
- Confirmed proper detection of intersections and dead ends
- Tested timing consistency using periodic interrupts

### FSM Validation

- Confirmed correct state transitions for all navigation cases
- Tested turn execution and recovery behavior
- Verified dead-end and turnaround handling

### PWM and Servo Testing

- Validated PWM generation using Timer_A2
- Verified servo response and positioning accuracy
- Tested pouring arm actuation during operation

### Integrated System Testing

- Performed complete line-following navigation tests
- Evaluated intersection decision logic using PMOD switches
- Tested full autonomous operation under repeated trials

---

## Project Demonstration

### Demo Videos

- Full System Demonstration: *https://youtu.be/RD7pSS3bs8I*
- GitHub Link: *https://github.com/CharbelAbouafech/cup-pouring-arm*

---

## Conclusion

The Cup Pouring Arm project demonstrates the implementation of an embedded autonomous robotic system using the MSP432 microcontroller. By integrating reflectance sensors, FSM-based navigation, timer interrupts, and PWM-controlled servos. Future improvements may include more advanced path-planning algorithms, wireless communication, and additional sensor integration for enhanced autonomous operation.