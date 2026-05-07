# Cup Pouring Arm

## Overview

Autonomous line-following robot with a servo-controlled claw arm, built on the MSP432. The robot follows a reflectance-sensor track, navigates intersections via a finite state machine (FSM), and executes a cup-pickup sequence at designated T-intersection waypoints. Motor control uses PWM, and the PMOD slide switches gate operation and configure path behavior.

## Features

* Line following with 8-channel reflectance sensor
* FSM-based navigation (turns, intersections, dead ends)
* Servo-controlled claw arm (open / close / pickup sequence)
* Switch-gated operation — robot idles when all switches are OFF
* Switch-configurable straight-through behavior at turns
* PWM motor and servo control via Timer A2
* 10 ms periodic sensor sampling via Timer A1 interrupt
* RGB LED status indicators per FSM state

## Key Concepts

### Line Detection

Sensor values are encoded as 8-bit patterns to classify position:

| Pattern (binary) | Position |
|-----------------|----------|
| `00011000`      | Center |
| `00011100`      | Slight left of center |
| `00001110`      | Off left |
| `00111000`      | Slight right of center |
| `01110000`      | Off right |
| `00000001`      | Far left |
| `10000000`      | Far right |
| `11111000`      | Left turn |
| `00011111`      | Right turn |
| `11111111`      | T-intersection |
| `00000000`      | Dead end |

### FSM States

* `FOLLOW_LINE`: Default line tracking; corrects deviations, detects special positions
* `CHECK_INTERSECTION`: Creeps forward and re-reads sensor to confirm turn/intersection type
* `TURN_LEFT_STATE`: Executes a left turn sequence then returns to CHECK_INTERSECTION
* `TURN_RIGHT_STATE`: Executes a right turn sequence then returns to CHECK_INTERSECTION
* `TURN_AROUND`: Spins 180° (left pivot ~900 ms) then returns to CHECK_INTERSECTION

Dead-end handling is done inline in `FOLLOW_LINE`: the robot stops with LED1 red when the sensor reads all zeros.

### Claw / Arm Control

Two servos are driven by Timer A2 PWM on P5.6 (CCR1) and P5.7 (CCR2):

| Function | Servo 2 Duty Cycle | Result |
|---|---|---|
| `claw_open()` | 1500 | Claw fully open |
| `claw_close()` | 2800 | Claw fully closed |
| `pickup()` | open → 500 ms → close → 500 ms | Pickup sequence |

Servo 1 (P5.6) is held at duty cycle 2100 every interrupt cycle.

### Switch Behavior (PMOD SWT, P10.0–P10.3)

`switch_status` is the 4-bit value read from P10 each interrupt.

| switch_status | Behavior |
|---|---|
| `0x00` (all OFF) | Robot idles: claw opens, motors stop, `point` counter resets |
| `0x01` (SWT1) or `0x04` (SWT3) | At **left turns**: go straight instead of turning |
| `0x01` (SWT1) | At **right turns**: go straight instead of turning |
| Any other non-zero value | Turn left or right as detected |

At a confirmed **T-intersection**:
* `point == 1` → execute `pickup()` then turn around
* Otherwise → execute `pickup()` and stop

`point` increments by 1 each time the robot commits to a left or right turn, tracking waypoints along the route.

## Timing

* Sensor read every ~10 ms via Timer A1 interrupt (CCR0 = 12000, 1 MHz effective)
* Timer A2 PWM period = 20 ms (60000 counts, SMCLK/2 = 6 MHz, up-down mode)
* FSM updates immediately after each sensor read

## Main Flow

1. Timer A1 interrupt fires every 1 ms; sensor triggered every 10th tick
2. On tick 10+1: sensor data latched, switch status read, claw and FSM updated
3. If switch_status == 0: claw opens, motors stop, counter resets
4. Otherwise: claw closes, FSM processes sensor data and drives motors/LEDs

## Initialization (`main`)

| Step | Call |
|---|---|
| 48 MHz clock | `Clock_Init48MHz()` |
| LEDs (onboard + chassis) | `LED1_Init()`, `LED2_Init()`, `Chassis_Board_LEDs_Init()` |
| Reflectance sensor | `Reflectance_Sensor_Init()` |
| SysTick interrupt | `SysTick_Interrupt_Init(...)` |
| Timer A1 interrupt (1 ms) | `Timer_A1_Interrupt_Init(&Timer_A1_Periodic_Task, 12000)` |
| Timer A2 PWM (20 ms) | `Timer_A2_PWM_Init(60000, 0, 0)` |
| PMOD switches | `PMOD_SWT_Init()` |
| Motors | `Motor_Init()` |

## Pin Connections

| Component | Signal / Description | MSP432 Pin |
|---|---|---|
| SWT1 | Switch 1 | P10.0 |
| SWT2 | Switch 2 | P10.1 |
| SWT3 | Switch 3 | P10.2 |
| SWT4 | Switch 4 | P10.3 |
| PMOD Pin 5 | Ground | GND |
| PMOD Pin 6 | Power | VCC (3.3V) |
| Servo 1 (arm) | PWM Input | P5.6 (PM_TA2.1) |
| Servo 1 (arm) | Power | 5V External |
| Servo 1 (arm) | Ground | GND |
| Servo 2 (claw) | PWM Input | P5.7 (PM_TA2.2) |
| Servo 2 (claw) | Power | 5V External |
| Servo 2 (claw) | Ground | GND |
| Reflectance Sensor Ch 0–7 | Data output | P7.0–P7.7 |
| Reflectance Sensor | Control Even | P5.3 |
| Reflectance Sensor | Control Odd | P9.2 |
| Reflectance Sensor | Power | 5V |
| Reflectance Sensor | Ground | GND |
