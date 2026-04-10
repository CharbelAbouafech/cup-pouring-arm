/**
 * @file Bumper_Switches.h
 * @brief Header file for the Bumper_Switches driver.
 *
 * This file contains the function definitions for the Bumper_Switches driver.
 * It will be used to as a protective measure for our robot, in scenarios where
 * there is an object blocking our robot.
 *
 * The following pins are used when the Bumper Switches are connected to the TI MSP432 LaunchPad:
 *  - BUMP_0 - P4.0
 *  - BUMP_1 - P4.2
 *  - BUMP_2 - P4.3
 *  - Right bump switch - GND
 *  - BUMP_3 - P4.5
 *  - BUMP_4 - P4.6
 *  - BUMP_5 -n P4.7
 *  - Left bumper switch - GND
 *
 * @author Javier Narvaez, Charbel Abou Afech
 *
 */

#ifndef INC_BUMPER_SWITCHES_H_
#define INC_BUMPER_SWITCHES_H_

#include <stdint.h>
#include "msp.h"

/**
 * @brief User-defined task function for handling Bumper Switch interrupt events.
 *
 * @return None
 */
void (*Bumper_Task)(uint8_t bumper_switch_state);

/**
 * @brief Initialize the Bumper Switches and set up interrupt handling.
 *
 * @param task A pointer to the user-defined function that will be called on a falling edge event.
 *
 * @return None
 */
void Bumper_Switches_Init(void(*task)(uint8_t));

/**
 * @brief Read the current state of the 6 Bumper Switches which have negative logic behavior.
 *
 * @return uint8_t The 6-bit positive logic result representing the current state of the switches (0 to 63).
 */
uint8_t Bumper_Read(void);

#endif /* INC_BUMPER_SWITCHES_H_ */
