# Cup Pouring Arm

A line-following robot with a servo claw, written in bare-metal C for the TI MSP432. It follows a taped line using an 8-channel reflectance sensor, handles turns and intersections with a state machine, and runs a claw open/close routine at a T-intersection. The four PMOD switches start and stop the robot and choose which route it takes.

**Authors:** Charbel Abou Afech, Javier Narvaez

**Demo video:** https://youtu.be/RD7pSS3bs8I

## How it runs

`main()` sets up the 48 MHz clock, the LEDs, the reflectance sensor, SysTick, the Timer_A1 periodic task, Timer_A2 PWM (both servo outputs start at 0), the PMOD switches and the drive motors. Then it sits in an empty loop. All the work happens in the Timer_A1 task, which counts ticks:

- Every 10th tick it starts a reflectance reading (about every 10 ms).
- On the next tick it reads the result and the switches, and sets servo 1 to a fixed duty of 2100. Then:
  - **Any switch on:** it closes the claw and runs the line-following state machine.
  - **All switches off:** it opens the claw, stops the motors and resets the turn counter `point` to 0.

So the switches double as the on/off control.

## Reading the sensor

The 8 sensor readings come back as one byte. The code treats bit 7 as the left side and bit 0 as the right side. The pattern names describe where the robot is relative to the line, so `SLIGHT_LEFT_CENTER` means the robot has drifted a little left and has to steer right.

| Reading | Name | What `FOLLOW_LINE` does |
|---------|------|-------------------------|
| `00011000` | CENTER | drive forward |
| `00011100` | SLIGHT_LEFT_CENTER | steer right |
| `00001110` | OFF_LEFT | steer right for 150 ms, then forward 300 ms |
| `00111000` | SLIGHT_RIGHT_CENTER | steer left |
| `01110000` | OFF_RIGHT | steer left for 150 ms, then forward 300 ms |
| `00000001` | FAR_LEFT | hard right (speed 4500) for 300 ms, forward 300 ms, then `CHECK_INTERSECTION` |
| `10000000` | FAR_RIGHT | left (speed 2500) for 500 ms, forward 500 ms, then `CHECK_INTERSECTION` |
| `11111000` | LEFT_TURN | `CHECK_INTERSECTION` |
| `00011111` | RIGHT_TURN | `CHECK_INTERSECTION` |
| `11111111` | T_INTERSECTION | `CHECK_INTERSECTION` |
| `00000000` | DEAD_END | stop the motors |
| anything else | unknown pattern | forward at half speed for 100 ms, stop, then `CHECK_INTERSECTION` |

## State machine

| State | What it does |
|-------|--------------|
| `FOLLOW_LINE` | The default. Follows the line using the table above. |
| `CHECK_INTERSECTION` | Creeps forward at half speed for 200 ms, stops, then decides what to do from the sensor reading taken just before the creep. |
| `TURN_LEFT_STATE` | Forward 90 ms, left at 4500 for 490 ms, forward 500 ms, then back to `CHECK_INTERSECTION`. |
| `TURN_RIGHT_STATE` | The same sequence, turning right. |
| `TURN_AROUND` | Left at 4500 for 900 ms, then back to `CHECK_INTERSECTION`. |
| `DEAD_END_STATE` | Declared but never used. A dead end is handled inside `FOLLOW_LINE`, and the robot resumes once the sensor sees the line again. |

What `CHECK_INTERSECTION` does with the reading:

- **CENTER, SLIGHT_LEFT_CENTER or SLIGHT_RIGHT_CENTER:** back to `FOLLOW_LINE`.
- **T_INTERSECTION:** runs `pickup()`. If `point == 1` it then turns around. Otherwise it stops the motors and stays in `CHECK_INTERSECTION`, so the next pass creeps forward and checks again.
- **LEFT_TURN or RIGHT_TURN:** the switches decide whether to turn or drive straight (next section). Either way `point` goes up by 1.
- **Anything else:** back to `FOLLOW_LINE`.

`pickup()` opens the claw, waits 500 ms, closes it, and waits another 500 ms.

## Choosing a route

`Get_PMOD_SWT_Status()` returns the switch value. 0 means stopped. The code treats the values 1 and 4 specially:

| Switch value | At a LEFT_TURN | At a RIGHT_TURN |
|--------------|----------------|-----------------|
| 1 | drive straight for 500 ms | drive straight for 500 ms |
| 4 | drive straight for 500 ms | turn right |
| any other non-zero value | turn left | turn right |

## Servos, motors and LEDs

Timer_A2 PWM drives two servos. Duty values are in timer counts:

- **Servo 1** is held at 2100 by the control loop.
- **Servo 2 (claw)** is 1500 open and 2800 closed.

The drive motors use `Motor_Forward`, `Motor_Left`, `Motor_Right` and `Motor_Stop` with separate left and right speeds. Normal speed is 2500, hard turns use 4500, and creeping uses half speed (1250).

The RGB LED shows which case the robot is in: green = following the line, yellow = correcting, pink = correcting the other way, white = FAR_LEFT, sky blue = FAR_RIGHT, blue = turning around or starting a pickup, red = unknown pattern or the claw closing in a pickup. The red LED comes on at a dead end or an unknown pattern.

## Hardware and pins

- TI MSP432 microcontroller
- Two-wheel differential drive base
- 8-channel reflectance sensor array (5 V)
- Two servos, powered from an external 5 V supply
- PMOD switch module (4 switches, 3.3 V)
- All grounds go to the board's GND

| Signal | Pin |
|--------|-----|
| Reflectance sensor outputs 0 to 7 | P7.0 to P7.7 |
| Reflectance sensor control (even / odd) | P5.3 / P9.2 |
| Servo 1 (Timer_A2, channel 1) | P5.6 |
| Servo 2, claw (Timer_A2, channel 2) | P5.7 |
| PMOD switches 1 to 4 | P10.0 to P10.3 |

## Source layout

- `main.c`: the state machine, the Timer_A1 task and the claw routines
- `inc/`: the drivers for the clock, GPIO, motors, reflectance sensor, SysTick, Timer_A1 and Timer_A2

## Limitations

- Turns and recovery moves are fixed delays (up to 900 ms) inside the Timer_A1 task, so the robot doesn't read the sensor during them. The timings were tuned by hand and will change with battery level and wheel grip.
- The route depends only on the switch value and the `point` counter.

## Testing

We tested in stages on the robot: sensor readings first, then the state machine, then PWM and the claw, then complete runs with different switch settings. The RGB LED shows which case the robot thinks it's in, so you can see what it's reading while it drives.

## Build and run

1. Build the project for the MSP432 and flash it.
2. Wire the sensor, servos, motors and PMOD switches as in the pin table.
3. Put the robot on the line and turn on a switch. Turning all switches off stops it.
