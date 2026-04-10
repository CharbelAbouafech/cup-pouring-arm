# cup-pouring-arm

## Overview

Autonomous line-following robot using an MSP432. It reads reflectance sensors and navigates paths, turns, intersections, and dead ends using a finite state machine (FSM). Motor control is handled with PWM, and behavior can be modified via a switch input.

## Features

* Line following with reflectance sensors
* FSM-based navigation
* Handles turns, intersections, and dead ends
* PWM motor control
* Timer-driven sensor sampling
* Switch-controlled behavior
* LED status indicators

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

## Notes

* Uses delay-based motion (no feedback control)
* Tunable speeds and timing constants
* Designed for simple autonomous navigation tasks

## Future Improvements

* Add PID control
* Use encoders instead of delays
* Improve intersection handling
* Add obstacle detection
