
#include "cpu.h"
#include "hal_common_includes.h" 
#include "pwm_hal.h"
#include "motor_hal.h"
#include "motion.h"
#include "encoder.h"
#include <stdio.h>


#define HIGH      3200U
#define LOW       0U
#define MEDIUM    0U

extern float g_inches_f;


void move_forward()
{
  configure_channel(1, LOW);
  configure_channel(2, HIGH);
  configure_channel(3, LOW);
  configure_channel(4, HIGH);
}

void move_back()
{
  configure_channel(1, HIGH);
  configure_channel(2, LOW);
  configure_channel(3, HIGH);
  configure_channel(4, LOW);
}

void move_back_left()
{
  configure_channel(1, LOW);
  configure_channel(2, LOW);
  configure_channel(3, HIGH);
  configure_channel(4, LOW);
}

void turn_f_left()
{
  configure_channel(1, LOW);
  configure_channel(2, HIGH);
  configure_channel(3, LOW);
  configure_channel(4, MEDIUM);
}

void turn_f_right()
{
  configure_channel(1, LOW);
  configure_channel(2, MEDIUM);
  configure_channel(3, LOW);
  configure_channel(4, HIGH);
}

void brake()
{
  configure_channel(1, LOW);
  configure_channel(2, LOW);
  configure_channel(3, LOW);
  configure_channel(4, LOW);
}

void move_robot(uint8_t next_dir)
{
	switch(next_dir)
	{
		case LEFT:
      brake();
      cpu_sw_delay(10U);
      encoders_enable();
      turn_f_left();
      
      while(g_rightEncoderCount < 33)
      {
          printf("\n\r");
      }

      encoders_disable();
      brake();
      //cpu_sw_delay(20U);

			break;
		case RIGHT:
      brake();
      cpu_sw_delay(10U);

      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
      encoders_enable();

      move_back_left();
      while(g_leftEncoderCount < 2){
        printf("\n\r");
      }

      encoders_reset();
      brake();
      move_forward();
      while(g_leftEncoderCount < 22 && g_rightEncoderCount < 22){
        printf("\n\r");
      }

      encoders_reset();
      brake();
      cpu_sw_delay(40U);
      turn_f_right();

      while(g_leftEncoderCount < 36){
        printf("\n\r");
      }

      encoders_disable();

     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
      brake();
      //cpu_sw_delay(20U);

			break;
		case FORWARD:
      //brake();
      //cpu_sw_delay(10U);
			move_forward();
      cpu_sw_delay(10U);

			break;
		case BACK:
			move_back();
			break;
    case RECENTER_RIGHT:
      brake();
      cpu_sw_delay(10U);
      turn_f_right();
      cpu_sw_delay(07U);
      break;

    case RECENTER_LEFT:
      brake();
      cpu_sw_delay(10U);
      turn_f_left();
      cpu_sw_delay(07U);
      break;

     case PULL_IN_PARK:
        brake();
        cpu_sw_delay(10U);

        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
        encoders_enable();

        move_forward();
        while(g_leftEncoderCount < 25 && g_rightEncoderCount < 25){
          printf("\n\r");
        }

        encoders_reset();

        brake();
        cpu_sw_delay(40U);
        turn_f_right();

        while(g_leftEncoderCount < 36){
          printf("\n\r");
        }

        encoders_reset();

        move_forward();
        while(g_leftEncoderCount < 35 && g_rightEncoderCount < 35){
          printf("\n\r");
        }
        encoders_disable();

      // HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);


       brake();
       while(1){

        //Red and BLUE LED will be on to show we parked
         HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
       }
       //next_dir = STOP;
        //cpu_sw_delay(20U);

      break;

		case STOP:
			brake();
			break;

	}
}	