/**
 * @file Bumper_Switches.c
 * @brief Source code for the Bumper_Switches driver.
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

#include "../inc/Bumper_Switches.h"

void Bumper_Switches_Init(void(*task)(uint8_t))
{
    Bumper_Task = task;     // Store the task function for use during interrupt handling

    P4->SEL0 &= ~0xED;      // Clearing the corresponding bits in the SEL0 and SEL1 registers for GPIO
    P4->SEL1 &= ~0xED;

    P4->DIR &= ~0xED;       // Set the Corresponding bits as Input

    P4->REN |= 0xED;        // Enable pull-up resistors on the corresponding pins

    P4->OUT |= 0xED;        // Set the corresponding bits in the OUT register

    P4->IES |= 0xED;        // Configure the pins to use falling edge event triggers

    P4->IFG &= ~0xED;       // Clear any existing interrupt flags on the corresponding pins

    P4->IE |= 0xED;         // Enable interrupts on the corresponding pins

    // Set the priority level of the interrupts (IRQ 38) to 0 (section 2.4.3.20)
    NVIC->IP[9] = (NVIC->IP[9] & 0xFF0FFFFF);

    NVIC->ISER[1] = 0x00000040; // Enable Interrupt 38 in NVIC
}

uint8_t Bumper_Read(void)
{
    // Declare a local variable to store the input register value
    uint32_t bumper_state = ~P4->IN;

    // Use bitwise operations to extract the relevant bits representing the switch states.
    return (((bumper_state & 0xE0) >> 2) | ((bumper_state & 0x0C) >> 1) | (bumper_state & 0x01));
}

void PORT4_IRQHandler(void)
{
    // Clear the interrupt flags for the corresponding pins.
    P4->IFG &= ~0xED;

    // Execute the user-defined task
    (*Bumper_Task)(Bumper_Read());
}
