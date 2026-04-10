/**
 * @file Reflectance_Sensor.h
 * @brief Reflectance sensor header file.
 *
 * Contains functions to use the 8-channel QTRX sensor with GPIO.
 *
 * Pin connections:
 *  - Sensor 0-7 -> P7.0 to P7.7
 *  - Ctrl Even  -> P5.3
 *  - Ctrl Odd   -> P9.2
 *  - VCC        -> 5V
 *  - GND        -> GND
 *
 * Basic read steps:
 *  1. Turn on IR LEDs
 *  2. Set pins high (output)
 *  3. Wait briefly
 *  4. Set pins as input
 *  5. Wait for signals to go low
 *  6. Turn off IR LEDs
 *
 * @author Javier Narvaez, Charbel Abou Afech
 */

#ifndef INC_REFLECTANCE_SENSOR_H_
#define INC_REFLECTANCE_SENSOR_H_

#include <stdint.h>
#include "msp.h"
#include "Clock.h"

/**
 * @brief Initialize the sensor.
 *
 * Sets up GPIO and IR LEDs.
 *
 * @return None
 */
void Reflectance_Sensor_Init();

/**
 * @brief Read all 8 sensors.
 *
 * Turns on LEDs, charges sensors, waits, then reads values.
 *
 * @param time Wait time in microseconds.
 *
 * @return 8-bit sensor data (1 = black, 0 = white)
 */
uint8_t Reflectance_Sensor_Read(uint32_t time);

/**
 * @brief Start a sensor read.
 *
 * Turns on LEDs and prepares pins.
 *
 * @return None
 */
void Reflectance_Sensor_Start();

/**
 * @brief Finish a sensor read.
 *
 * @return 8-bit sensor data (1 = black, 0 = white)
 */
uint8_t Reflectance_Sensor_End();

/**
 * @brief Calculate line position.
 *
 * Converts sensor data to position value.
 *
 * @param data 8-bit sensor data
 *
 * @return Position (in 0.1 mm)
 */
int32_t Reflectance_Sensor_Position(uint8_t data);

#endif /* INC_REFLECTANCE_SENSOR_H_ */
