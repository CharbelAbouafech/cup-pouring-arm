/**
 * @file Reflectance_Sensor.c
 * @brief Reflectance sensor source file.
 *
 * Uses GPIO to read the 8-channel QTRX sensor.
 *
 * @author Javier Narvaez, Charbel Abou Afech
 */

#include "../inc/Reflectance_Sensor.h"

/**
 * @brief Weights for each sensor.
 *
 * Used to calculate position.
 * Right sensors are positive, left are negative.
 */
static int32_t Weight[8] = {334, 238, 142, 48, -48, -142, -238, -334};

/**
 * @brief Bit masks for each sensor.
 *
 * Used to check each sensor bit.
 */
static int32_t Mask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

void Reflectance_Sensor_Init()
{
    // Set P5.3 as output (even IR LEDs)
    P5->SEL0 &= ~0x08;
    P5->SEL1 &= ~0x08;
    P5->DIR |= 0x08;

    // Turn off even IR LEDs
    P5->OUT &= ~0x08;

    // Set P9.2 as output (odd IR LEDs)
    P9->SEL0 &= ~0x04;
    P9->SEL1 &= ~0x04;
    P9->DIR |= 0x04;

    // Turn off odd IR LEDs
    P9->OUT &= ~0x04;

    // Set P7.0 - P7.7 as inputs
    P7->SEL0 &= ~0xFF;
    P7->SEL1 &= ~0xFF;
    P7->DIR &= ~0xFF;
}

uint8_t Reflectance_Sensor_Read(uint32_t time)
{
    // Turn on IR LEDs
    P5->OUT |= 0x08;
    P9->OUT |= 0x04;

    // Set P7.0 - P7.7 as outputs
    P7->DIR |= 0xFF;

    // Set pins high
    P7->OUT |= 0xFF;

    // Wait 10 us
    Clock_Delay1us(10);

    // Set pins as inputs
    P7->DIR &= ~0xFF;

    // Wait for decay
    Clock_Delay1us(time);

    // Read sensor values (1 = black, 0 = white)
    uint8_t reflectance_value = P7->IN;

    // Turn off IR LEDs
    P5->OUT &= ~0x08;
    P9->OUT &= ~0x04;

    // Return sensor data
    return reflectance_value;
}

void Reflectance_Sensor_Start()
{
    // Turn on IR LEDs
    P5->OUT |= 0x08;
    P9->OUT |= 0x04;

    // Set P7.0 - P7.7 as outputs
    P7->DIR |= 0xFF;

    // Set pins high
    P7->OUT |= 0xFF;

    // Wait 10 us
    Clock_Delay1us(10);

    // Set pins as inputs
    P7->DIR &= ~0xFF;
}

uint8_t Reflectance_Sensor_End()
{
    // Read sensor values (1 = black, 0 = white)
    uint8_t reflectance_value = P7->IN;

    // Turn off IR LEDs
    P5->OUT &= ~0x08;
    P9->OUT &= ~0x04;

    // Return sensor data
    return reflectance_value;
}

int32_t Reflectance_Sensor_Position(uint8_t data)
{
    int sum = 0;
    int count = 0;

    // If any sensor is active
    if (data)
    {
        // Check each sensor
        for(int i = 0; i < 8; i++)
        {
            // If this sensor is active
            if (data & Mask[i])
            {
                // Add its weight
                sum = sum + Weight[i];
                count++;
            }
        }

        // Return average position
        return sum/count;
    }
    else
    {
        // No sensors active, return default
        return Weight[0] + 1;
    }
}
