# Cup Pouring Arm

A line-following robot with a servo claw, written in bare-metal C for the TI MSP432. It follows a taped line, handles turns and intersections, and grabs a cup at a T-intersection. Four PMOD switches turn it on and pick the route it takes.

**Authors:** Charbel Abou Afech, Javier Narvaez

**Demo video:** https://youtu.be/RD7pSS3bs8I

## How it runs

`main()` initializes the 48 MHz clock, the LEDs, the reflectance sensor, SysTick, the Timer_A1 periodic task, Timer_A2 PWM (both servos start at 0), the PMOD switches and the drive motors. Then it sits in an empty loop. All the work happens in the Timer_A1 task, which counts milliseconds:

- On every 10th tick it starts a reflectance reading.
- On the next tick it reads the result and the switches, sets servo 1 to 2100, and then:
  - **Any switch on:** closes the claw and runs the line-following state machine.
  - **All switches off:** opens the claw, stops the motors and resets the turn counter `point` to 0.

So the switches also work as the on/off control.

## Sensor patterns

The 8 sensor readings are packed into one byte. The names below follow the code (`FAR_LEFT` is bit 0, `FAR_RIGHT` is bit 7). This is what `FOLLOW_LINE` does for each pattern:

| Reading | Name | Action |
|---------|------|--------|
| `00011000` | CENTER | drive forward |
| `00011100` | SLIGHT_LEFT_CENTER | steer with `Motor_Right` |
| `00001110` | OFF_LEFT | `Motor_Right` 150 ms, then forward 300 ms |
| `00111000` | SLIGHT_RIGHT_CENTER | steer with `Motor_Left` |
| `01110000` | OFF_RIGHT | `Motor_Left` 150 ms, then forward 300 ms |
| `00000001` | FAR_LEFT | `Motor_Right` at 4500 for 300 ms, forward 300 ms, then `CHECK_INTERSECTION` |
| `10000000` | FAR_RIGHT | `Motor_Left` at 2500 for 500 ms, forward 500 ms, then `CHECK_INTERSECTION` |
| `11111000` | LEFT_TURN | `CHECK_INTERSECTION` |
| `00011111` | RIGHT_TURN | `CHECK_INTERSECTION` |
| `11111111` | T_INTERSECTION | `CHECK_INTERSECTION` |
| `00000000` | DEAD_END | stop the motors, red LED on |
| anything else | unknown | forward at half speed for 100 ms, stop, then `CHECK_INTERSECTION` |

## State machine

| State | What it does |
|-------|--------------|
| `FOLLOW_LINE` | Default. Follows the line using the table above. |
| `CHECK_INTERSECTION` | Creeps forward at half speed for 200 ms, stops, then decides what to do using the reading taken just before the creep. |
| `TURN_LEFT_STATE` | Forward 90 ms, turn left at 4500 for 490 ms, forward 500 ms, then back to `CHECK_INTERSECTION`. |
| `TURN_RIGHT_STATE` | The same sequence, turning right. |
| `TURN_AROUND` | Turn left at 4500 for 900 ms, then back to `CHECK_INTERSECTION`. |
| `DEAD_END_STATE` | Declared but never entered. A dead end is handled inside `FOLLOW_LINE`: the motors stop, and the robot resumes if it sees the line again. |

In `CHECK_INTERSECTION`:

- **CENTER or a slight correction:** back to `FOLLOW_LINE`.
- **T_INTERSECTION:** runs `pickup()`. If `point == 1` it then turns around. Otherwise it stops.
- **LEFT_TURN / RIGHT_TURN:** the PMOD switches decide whether to turn or drive straight (next table). Either way `point` goes up by 1.
- **Anything else:** back to `FOLLOW_LINE`.

`pickup()` opens the claw, waits 500 ms, closes it, and waits another 500 ms.

## Route selection (PMOD switches)

`Get_PMOD_SWT_Status()` returns the switch value. 0 means stopped. The code treats values 1 and 4 specially:

| Switch value | At a LEFT_TURN | At a RIGHT_TURN |
|--------------|----------------|-----------------|
| 1 | drive straight for 500 ms | drive straight for 500 ms |
| 4 | drive straight for 500 ms | turn right |
| any other non-zero value | turn left | turn right |

## Servos, speeds and LEDs

Timer_A2 PWM drives two servos, and the duty values below are in timer counts:

- **Servo 1** is held at 2100 by the control loop. **[fill in: what it moves]**
- **Servo 2 (claw):** open = 1500, closed = 2800.

Drive motors use `Motor_Forward`, `Motor_Left`, `Motor_Right` and `Motor_Stop`. Nominal speed is 2500 on both wheels, sharp turns use 4500, and creeping uses half speed (1250).

LED2 (RGB) shows what the robot is doing: green = following the line, yellow = correcting, pink = correcting the other way, white = FAR_LEFT, sky blue = FAR_RIGHT, blue = turning around or starting a pickup, red = unknown pattern or closing the claw. LED1 (red) comes on at a dead end or an unknown pattern.

## Hardware and pins

- TI MSP432 microcontroller with the 48 MHz clock
- Two-wheel drive base, driven through `Motor.h` in `inc/` **[fill in: motor pins]**
- 8-channel reflectance sensor array (5 V)
- Two servos, powered from an external 5 V supply
- PMOD switch module (4 switches, 3.3 V)
- All grounds go to the board's GND

| Signal | Pin |
|--------|-----|
| Reflectance sensor outputs 0 to 7 | P7.0 to P7.7 |
| Reflectance sensor control (even / odd) | P5.3 / P9.2 |
| Servo 1 (Timer_A2 CCR1) | P5.6 |
| Servo 2, claw (Timer_A2 CCR2) | P5.7 |
| PMOD switches 1 to 4 | P10.0 to P10.3 |

## Source layout

- `main.c`: the state machine, the Timer_A1 task and the claw routines
- `inc/`: drivers for the clock, GPIO, motors, reflectance sensor, SysTick, Timer_A1 and Timer_A2 **[fill in: which of these you wrote]**

## Known limitations

- The turn and recovery moves use fixed delays (up to 900 ms) inside the Timer_A1 task, so the robot isn't reading the sensor while it turns. These timings depend on battery level and wheel traction.
- `collision_detected` and `PWM_NOMINAL` are declared but not used.
- The route depends only on the switch value and the turn counter.

## Testing

We tested in stages on the robot:

1. **Sensor.** Checked the readings at different line positions and confirmed that intersections and dead ends show up as the expected patterns.
2. **State machine.** Ran each turn, recovery and turn-around case and watched the state transitions using the LED colors.
3. **PWM and servos.** Checked the claw open and close positions and the pickup motion.
4. **Full system.** Complete runs with different switch settings, repeated several times.

## Build and run

1. Open the project in **[fill in: Code Composer Studio / Keil uVision]**.
2. Build it and flash it to the MSP432.
3. Wire the sensor, servos, motors and PMOD switches as in the table above.
4. Put the robot on the line and turn on a switch to start. Turning all switches off stops it.
