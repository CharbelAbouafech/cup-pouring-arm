/**
 * @file GPIO.c
 * @brief Source code for the GPIO driver.
 *
 * Includes function declarations for PMOD Switches & LEDs on chassis Board
 *
 * @author Javier Narvaez, Charbel Abou Afech
 */

#include "../inc/GPIO.h"

// Constant definitions for the built-in red LED
const uint8_t RED_LED_OFF           =   0x00;
const uint8_t RED_LED_ON            =   0x01;

// Constant definitions for the RGB LED colors
const uint8_t RGB_LED_OFF           =   0x00;
const uint8_t RGB_LED_RED           =   0x01;
const uint8_t RGB_LED_GREEN         =   0x02;
const uint8_t RGB_LED_YELLOW        =   0x03;
const uint8_t RGB_LED_BLUE          =   0x04;
const uint8_t RGB_LED_PINK          =   0x05;
const uint8_t RGB_LED_SKY_BLUE      =   0x06;
const uint8_t RGB_LED_WHITE         =   0x07;

// Constant definitions for the PMOD 8LD module
const uint8_t PMOD_8LD_ALL_OFF      =   0x00;
const uint8_t PMOD_8LD_ALL_ON       =   0xFF;
const uint8_t PMOD_8LD_0_3_ON       =   0x0F;
const uint8_t PMOD_8LD_4_7_ON       =   0xF0;

void LED1_Init()
{
    P1->SEL0 &= ~0x01;
    P1->SEL1 &= ~0x01;
    P1->DIR |= 0x01;
    P1->OUT &= ~0x01;
}

void LED1_Output(uint8_t led_value)
{
    P1->OUT = (P1->OUT & 0xFE) | led_value;
}

void LED1_Toggle()
{
    P1->OUT = P1->OUT ^ 0x01;
}

uint8_t LED1_Status()
{
    uint8_t LED1_Status = P1->OUT & 0x01;
    return LED1_Status;
}

void LED2_Init()
{
    P2->SEL0 &= ~0x07;
    P2->SEL1 &= ~0x07;
    P2->DS |= 0x07;
    P2->DIR |= 0x07;
    P2->OUT &= ~0x07;
}

void LED2_Output(uint8_t led_value)
{
    P2->OUT = (P2->OUT & 0xF8) | led_value;
}

void LED2_Toggle(uint8_t led_value)
{
    P2->OUT = P2->OUT ^ led_value;
}

uint8_t LED2_Status()
{
    uint8_t LED2_Status = P2->OUT & 0x07;
    return LED2_Status;
}

void Buttons_Init()
{
    P1->SEL0 &= ~0x12;
    P1->SEL1 &= ~0x12;
    P1->DIR |= ~0x12;
    P1->REN |= 0x12;
    P1->OUT |= 0x12;
}

uint8_t Get_Buttons_Status()
{
    uint8_t button_status = (P1->IN & 0x12);
    return button_status;
}

void PMOD_SWT_Init()
{
    P10->SEL0 &= ~0xF;
    P10->SEL1 &= ~0xF;
    P10->DIR &= ~0xF;
}

uint8_t Get_PMOD_SWT_Status()
{
    uint8_t switch_status = P10->IN & 0xF;
    return switch_status;
}

void Chassis_Board_LEDs_Init()
{
    P8->SEL0 &= ~0xE1;
    P8->SEL1 &= ~0xE1;
    P8->DIR |= 0xE1;
    P8->OUT &= ~0xE1;
}
