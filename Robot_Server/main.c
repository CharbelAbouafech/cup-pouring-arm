#include <stdint.h>
#include "msp.h"
#include "inc/Clock.h"
#include "inc/CortexM.h"
#include "inc/GPIO.h"
#include "inc/Motor.h"
#include "inc/Timer_A1_Interrupt.h"
#include "inc/Timer_A2_PWM.h"
#include "inc/SysTick_Interrupt.h"
#include "inc/Reflectance_Sensor.h"



// Initialization
#define PWM_NOMINAL 2500
#define SPEEDL 2500
#define SPEEDR 2500

uint8_t collision_detected = 0;
static uint32_t Timer_A1_ms_elapsed = 0;

// Uses different switch values to determine the path
uint8_t switch_status = 0;

// Determines if action was taken
uint8_t point = 0;

typedef enum
{
    CENTER                  = 0b00011000,
    SLIGHT_LEFT_CENTER      = 0b00011100,
    OFF_LEFT                = 0b00001110,
    SLIGHT_RIGHT_CENTER     = 0b00111000,
    OFF_RIGHT               = 0b01110000,
    FAR_LEFT                = 0b00000001,
    FAR_RIGHT               = 0b10000000,
    LEFT_TURN               = 0b11111000,
    RIGHT_TURN              = 0b00011111,
    T_INTERSECTION          = 0b11111111,
    DEAD_END                = 0b00000000
} Line_Position;

typedef enum
{
    FOLLOW_LINE,
    CHECK_INTERSECTION,
    TURN_AROUND,
    TURN_LEFT_STATE,
    TURN_RIGHT_STATE,
    DEAD_END_STATE
} RobotState;

static RobotState currentState = FOLLOW_LINE;

void claw_open();
void claw_close();
void pickup();

void Detect_Line_Position(uint32_t reflectance_sensor_data)
{
    switch(currentState)
    {
        case FOLLOW_LINE:
        {
            LED1_Output(RED_LED_OFF);
            LED2_Output(RGB_LED_GREEN);
            switch(reflectance_sensor_data)
            {
                case CENTER:
                {
                    Motor_Forward(SPEEDL, SPEEDR);
                    break;
                }
                case SLIGHT_LEFT_CENTER:
                {
                    LED1_Output(RED_LED_OFF);
                    LED2_Output(RGB_LED_YELLOW);
                    Motor_Right(SPEEDL, SPEEDR);
                    break;
                }
                case OFF_LEFT:
                {
                    LED1_Output(RED_LED_OFF);
                    LED2_Output(RGB_LED_YELLOW);
                    Motor_Right(SPEEDL, SPEEDR);
                    Clock_Delay1ms(150);
                    Motor_Forward(SPEEDL, SPEEDR);
                    Clock_Delay1ms(300);
                    break;
                }
                case SLIGHT_RIGHT_CENTER:
                {
                    LED1_Output(RED_LED_OFF);
                    LED2_Output(RGB_LED_PINK);
                    Motor_Left(SPEEDL, SPEEDR);
                    break;
                }
                case OFF_RIGHT:
                {
                    LED1_Output(RED_LED_OFF);
                    LED2_Output(RGB_LED_YELLOW);
                    Motor_Left(SPEEDL, SPEEDR);
                    Clock_Delay1ms(150);
                    Motor_Forward(SPEEDL, SPEEDR);
                    Clock_Delay1ms(300);
                    break;
                }
                case FAR_LEFT:
                {
                    LED2_Output(RGB_LED_WHITE);
                    Motor_Right(4500, 4500);
                    Clock_Delay1ms(300);
                    Motor_Forward(SPEEDL, SPEEDR);
                    Clock_Delay1ms(300);
                    currentState = CHECK_INTERSECTION;
                    break;
                }
                case FAR_RIGHT:
                {
                    LED2_Output(RGB_LED_SKY_BLUE);
                    Motor_Left(2500, 2500);
                    Clock_Delay1ms(500);
                    Motor_Forward(SPEEDL, SPEEDR);
                    Clock_Delay1ms(500);
                    currentState = CHECK_INTERSECTION;
                    break;
                }
                case LEFT_TURN:
                {
                    currentState = CHECK_INTERSECTION;
                    break;
                }
                case RIGHT_TURN:
                {
                    currentState = CHECK_INTERSECTION;
                    break;
                }
                case T_INTERSECTION:
                {
//                    if (point >= 1)
//                    {
//                        Motor_Stop();
//                        pickup();
//                        point += 1;
//                    }
//                    else{
//                        currentState = TURN_AROUND;   // choose strategy
//                    }
//                    break;
                    currentState = CHECK_INTERSECTION;
                    break;
                }
                case DEAD_END:
                {
                    LED1_Output(RED_LED_ON);
                    LED2_Output(RGB_LED_OFF);
                    Motor_Stop();
                    break;
                }
                default:
                {
                    LED1_Output(RED_LED_ON);
                    LED2_Output(RGB_LED_RED);
                    int halfL = SPEEDL / 2;
                    int halfR = SPEEDR / 2;
                    Motor_Forward(halfL, halfR);
                    Clock_Delay1ms(100);
                    Motor_Stop();
                    currentState = CHECK_INTERSECTION;
                    break;
                }
            }
            break;
        }
        case TURN_AROUND:
        {
            LED1_Output(RED_LED_OFF);
            LED2_Output(RGB_LED_BLUE);
            Motor_Left(4500, 4500);
            Clock_Delay1ms(900);
            currentState = CHECK_INTERSECTION;
            break;
        }

        case CHECK_INTERSECTION:
        {
            int halfL = SPEEDL / 2;
            int halfR = SPEEDR / 2;
            Motor_Forward(halfL, halfR);
            Clock_Delay1ms(200);
            Motor_Stop();
            switch(reflectance_sensor_data)
            {
                case CENTER:
                {
                    currentState = FOLLOW_LINE;
                    break;
                }
                case SLIGHT_LEFT_CENTER:
                {
                    currentState = FOLLOW_LINE;
                    break;
                }
                case SLIGHT_RIGHT_CENTER:
                {
                    currentState = FOLLOW_LINE;
                    break;
                }
                case T_INTERSECTION:
                {
                    if (point == 1)
                    {
                        pickup();

                        currentState = TURN_AROUND;   // choose strategy
                    }
                    else{
                        //currentState = TURN_AROUND;   // choose strategy
                        pickup();
                        Motor_Stop();
                    }
                    break;
                }
                case LEFT_TURN:
                {
                    if (switch_status == 1 || switch_status == 4 )
                    {
                        Motor_Forward(SPEEDL, SPEEDR);
                        Clock_Delay1ms(500);
                        point += 1;
                    }
                    else
                    {
                        currentState = TURN_LEFT_STATE;
                        point += 1;
                    }
                    break;
                }
                case RIGHT_TURN:
                {
                    if (switch_status == 1)
                    {
                        Motor_Forward(SPEEDL, SPEEDR);
                        Clock_Delay1ms(500);
                        point += 1;
                    }
                    else
                    {
                        currentState = TURN_RIGHT_STATE;
                        point += 1;
                    }
                    break;
                }
                default:
                {
                    currentState = FOLLOW_LINE;
                    break;
                }
            }
            break;
        }

        case TURN_LEFT_STATE:
        {
            Motor_Forward(SPEEDL, SPEEDR);
            Clock_Delay1ms(90);
            Motor_Left(4500, 4500);
            Clock_Delay1ms(490);
            Motor_Forward(SPEEDL, SPEEDR);
            Clock_Delay1ms(500);
            currentState = CHECK_INTERSECTION;
            break;
        }

        case TURN_RIGHT_STATE:
        {
            Motor_Forward(SPEEDL, SPEEDR);
            Clock_Delay1ms(90);
            Motor_Right(4500, 4500);
            Clock_Delay1ms(490);
            Motor_Forward(SPEEDL, SPEEDR);
            Clock_Delay1ms(500);
            currentState = CHECK_INTERSECTION;
            break;
        }

        default:
        {
            currentState = FOLLOW_LINE;
            break;
        }
    }

}


void Timer_A1_Periodic_Task(void)
{
    Timer_A1_ms_elapsed++;

    if ((Timer_A1_ms_elapsed % 10) == 0)
    {
        Reflectance_Sensor_Start();
    }

    if ((Timer_A1_ms_elapsed % 10) == 1)
    {
        uint8_t Reflectance_Sensor_Data = Reflectance_Sensor_End();
        switch_status = Get_PMOD_SWT_Status();
        Timer_A2_Update_Duty_Cycle_1(2100);
        if(switch_status != 0){
            claw_close();
            Detect_Line_Position(Reflectance_Sensor_Data);
        }
        else{
            claw_open();
            Motor_Stop();
            point = 0;
        }
    }
}

int main(void){

    DisableInterrupts();

    Clock_Init48MHz();

    LED1_Init();
    LED2_Init();

    Chassis_Board_LEDs_Init();

    Reflectance_Sensor_Init();

    SysTick_Interrupt_Init(SYSTICK_INT_NUM_CLK_CYCLES, SYSTICK_INT_PRIORITY);
    Timer_A1_Interrupt_Init(&Timer_A1_Periodic_Task, TIMER_A1_INT_CCR0_VALUE);

    Timer_A2_PWM_Init(TIMER_A2_PERIOD_CONSTANT, 0, 0);

    PMOD_SWT_Init();

    Motor_Init();

    EnableInterrupts();

    while(1)
    {

    }
}

void claw_open(){
    Timer_A2_Update_Duty_Cycle_2(1500);
}
void claw_close(){
    Timer_A2_Update_Duty_Cycle_2(2800);
}

void pickup(){
    LED2_Output(RGB_LED_BLUE);
    claw_open();
    Clock_Delay1ms(500);
    LED2_Output(RGB_LED_RED);
    claw_close();
    Clock_Delay1ms(500);
}

