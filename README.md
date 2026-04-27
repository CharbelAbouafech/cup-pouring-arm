# Cup Pouring Arm

## Overview

Autonomous line-following robot using an MSP432. It reads reflectance sensors and navigates paths, turns, intersections, and dead ends using a finite state machine (FSM). Motor control is handled with PWM, and behavior can be modified via a switch input.

## Features

* Line following with reflectance sensors
* FSM-based navigation
* Handles turns, intersections, and dead ends
* PWM motor control
* Timer sampling
* Switch-controlled behavior

## Key Concepts

### Line Detection

Sensor values are encoded as bit patterns to classify positions such as:

* Center, slight deviations
* Sharp left/right
* T-intersection
* Dead end

### FSM States

* `FOLLOW_LINE`: Default tracking
* `CHECK_INTERSECTION`: Confirms turns/intersections
* `TURN_LEFT_STATE` / `TURN_RIGHT_STATE`: Executes turns
* `TURN_AROUND`: Reverses direction
* `DEAD_END`: Stops

## Special Behavior

At T-intersections:

* Switch = 1 → triggers one-time PWM action
* Switch = 2 → turn right
* Otherwise → turn around

## Timing

* Sensor read every ~10 ms via Timer_A1 interrupt
* FSM updates immediately after each read
* Delays used for turning and stabilization

## Main Flow

1. Timer interrupt triggers sensor read
2. Sensor + switch input processed
3. FSM selects movement
4. Motors + LEDs updated

## Initialization

`main()` sets up:

* 48 MHz clock
* LEDs
* Reflectance sensors
* Timers (A1 interrupt, A2 PWM)
* PMOD switch
* Motors


## Pin Connections

| Component                          | Signal / Description                     | MSP432 Pin        |
|-----------------------------------|------------------------------------------|-------------------|
| SWT1                              | Switch 1                                 | P10.0             |
| SWT2                              | Switch 2                                 | P10.1             |
| SWT3                              | Switch 3                                 | P10.2             |
| SWT4                              | Switch 4                                 | P10.3             |
| Pin 5 PMOD Switch                            | Ground                                   | GND               |
| Pin 6 PMOD Switch                            | Power                                    | VCC (3.3V)        |
| Servo 1                           | PWM Input                   | P5.6 (PM_TA2.1)   |
| Servo 1                           | Power                          | 5V External Power Source               |
| Servo 1                           | Ground                       | GND               |
| Servo 2                           | PWM Input                   | P5.7 (PM_TA2.2)   |
| Servo 2                           | Power                          | 5V External Power Source               |
| Servo 2                           | Ground                      | GND               |
| Reflectance Sensor Output 0       | Channel 0                                | P7.0              |
| Reflectance Sensor Output 1       | Channel 1                                | P7.1              |
| Reflectance Sensor Output 2       | Channel 2                                | P7.2              |
| Reflectance Sensor Output 3       | Channel 3                                | P7.3              |
| Reflectance Sensor Output 4       | Channel 4                                | P7.4              |
| Reflectance Sensor Output 5       | Channel 5                                | P7.5              |
| Reflectance Sensor Output 6       | Channel 6                                | P7.6              |
| Reflectance Sensor Output 7       | Channel 7                                | P7.7              |
| Reflectance Sensor               | Control Even                             | P5.3              |
| Reflectance Sensor               | Control Odd                              | P9.2              |
| Reflectance Sensor               | Power                                    | 5V                |
| Reflectance Sensor               | Ground                                   | GND               |