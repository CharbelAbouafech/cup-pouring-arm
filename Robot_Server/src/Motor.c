/**
 * @file Motor.c
 * @brief Motor driver source file.
 *
 * Contains functions to control DC motors using PWM.
 *
 * @author Javier Narvaez, Charbel Abou Afech
 */

#include "../inc/Motor.h"

void Motor_Init()
{
    // Set P5.4 and P5.5 as GPIO outputs
    P5->SEL0 &= ~0x30;
    P5->SEL1 &= ~0x30;
    P5->DIR |= 0x30;

    // Set P5.4 and P5.5 to 0
    P5->OUT &= ~0x30;

    // Set P3.6 and P3.7 as GPIO outputs
    P3->SEL0 &= ~0xC0;
    P3->SEL1 &= ~0xC0;
    P3->DIR |= 0xC0;

    // Set P3.6 and P3.7 to 0
    P3->OUT &= ~0xC0;

    // Initialize Timer A0 for PWM
    Timer_A0_PWM_Init(TIMER_A0_PERIOD_CONSTANT, 0, 0);
}

void Motor_Forward(uint16_t left_duty_cycle, uint16_t right_duty_cycle)
{
    // Set motors to move forward
    P5->OUT &= ~0x30;

    // Set motor speeds
    Timer_A0_Update_Duty_Cycle_1(right_duty_cycle);
    Timer_A0_Update_Duty_Cycle_2(left_duty_cycle);

    // Turn motors on
    P3->OUT |= 0xC0;
}

void Motor_Backward(uint16_t left_duty_cycle, uint16_t right_duty_cycle)
{
    // Set motors to move backward
    P5->OUT |= 0x30;

    // Set motor speeds
    Timer_A0_Update_Duty_Cycle_1(right_duty_cycle);
    Timer_A0_Update_Duty_Cycle_2(left_duty_cycle);

    // Turn motors on
    P3->OUT |= 0xC0;
}

void Motor_Left(uint16_t left_duty_cycle, uint16_t right_duty_cycle)
{
    // Left motor backward
    P5->OUT |= 0x10;

    // Right motor forward
    P5->OUT &= ~0x20;

    // Set motor speeds
    Timer_A0_Update_Duty_Cycle_1(right_duty_cycle);
    Timer_A0_Update_Duty_Cycle_2(left_duty_cycle);

    // Turn motors on
    P3->OUT |= 0xC0;
}

void Motor_Right(uint16_t left_duty_cycle, uint16_t right_duty_cycle)
{
    // Left motor forward
    P5->OUT &= ~0x10;

    // Right motor backward
    P5->OUT |= 0x20;

    // Set motor speeds
    Timer_A0_Update_Duty_Cycle_1(right_duty_cycle);
    Timer_A0_Update_Duty_Cycle_2(left_duty_cycle);

    // Turn motors on
    P3->OUT |= 0xC0;
}

void Motor_Stop()
{
    // Turn motors off
    P3->OUT &= ~0xC0;
    P5->OUT &= ~0x30;

    // Set speed to 0
    Timer_A0_Update_Duty_Cycle_1(0);
    Timer_A0_Update_Duty_Cycle_2(0);
}
