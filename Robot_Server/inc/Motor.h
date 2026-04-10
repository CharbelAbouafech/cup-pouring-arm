/**
 * @file Motor.h
 * @brief Motor driver header file.
 *
 * Contains functions to control DC motors using PWM.
 * Includes setup, movement, speed control, and stop functions.
 *
 * @author Javier Narvaez, Charbel Abou Afech
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include <stdint.h>
#include "msp.h"
#include "../inc/Timer_A0_PWM.h"

/**
 * @brief Initialize the motors.
 *
 * Sets up GPIO pins and Timer A0 for motor control.
 *
 * @return None
 */
void Motor_Init();

/**
 * @brief Move motors forward.
 *
 * @param left_duty_cycle Left motor speed (0-99%).
 * @param right_duty_cycle Right motor speed (0-99%).
 *
 * @return None
 */
void Motor_Forward(uint16_t left_duty_cycle, uint16_t right_duty_cycle);

/**
 * @brief Move motors backward.
 *
 * @param left_duty_cycle Left motor speed (0-99%).
 * @param right_duty_cycle Right motor speed (0-99%).
 *
 * @return None
 */
void Motor_Backward(uint16_t left_duty_cycle, uint16_t right_duty_cycle);

/**
 * @brief Turn left.
 *
 * @param left_duty_cycle Left motor speed (0-99%).
 * @param right_duty_cycle Right motor speed (0-99%).
 *
 * @return None
 */
void Motor_Left(uint16_t left_duty_cycle, uint16_t right_duty_cycle);

/**
 * @brief Turn right.
 *
 * @param left_duty_cycle Left motor speed (0-99%).
 * @param right_duty_cycle Right motor speed (0-99%).
 *
 * @return None
 */
void Motor_Right(uint16_t left_duty_cycle, uint16_t right_duty_cycle);

/**
 * @brief Stop the motors.
 *
 * Sets speed to 0%.
 *
 * @return None
 */
void Motor_Stop();

#endif /* INC_MOTOR_H_ */
