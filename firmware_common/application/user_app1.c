/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */
extern u8 G_au8DebugScanfBuffer[];                     /* From debug.c */
extern u8 G_u8DebugScanfCharCount;                     /* From debug.c */

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR+1,"0");
  LCDMessage(LINE1_START_ADDR+3,"0");
  LCDMessage(LINE1_START_ADDR+6,"0");
  LCDMessage(LINE1_START_ADDR+8,"0");
  LCDMessage(LINE1_START_ADDR+11,"0");
  LCDMessage(LINE1_START_ADDR+13,"0");
  LCDMessage(LINE1_START_ADDR+16,"0");
  LCDMessage(LINE1_START_ADDR+18,"0");
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(RED);
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  static u32 u32TimeCounter=0;
  static u8  u8TimeMessage[5];
  static u16 u16Counter=0;
  static u8 u8ColorType=7;
  static LedRateType DutyCycle=LED_PWM_0;
  static LedCommandType aeDemoList[]=  {
											{WHITE,0,TRUE,LED_PWM_100},
											{WHITE,0,FALSE,LED_PWM_0},
											{PURPLE,0,TRUE,LED_PWM_100},
											{PURPLE,0,FALSE,LED_PWM_0},
											{BLUE,0,TRUE,LED_PWM_100},
											{BLUE,0,FALSE,LED_PWM_0},
											{CYAN,0,TRUE,LED_PWM_100},
											{CYAN,0,FALSE,LED_PWM_0},
											{GREEN,3000,TRUE,LED_PWM_100},
											{GREEN,9000,FALSE,LED_PWM_0},
											{YELLOW,0,TRUE,LED_PWM_100},
											{YELLOW,0,FALSE,LED_PWM_0},
											{ORANGE,0,TRUE,LED_PWM_100},
											{ORANGE,0,FALSE,LED_PWM_0},
											{RED,1000,TRUE,LED_PWM_100},
											{RED,4000,FALSE,LED_PWM_0}
  										};
  
  static u8 au8LCDList[]={1,1,3,3,6,6,8,8,11,11,13,13,16,16,18,18};
  
  u32TimeCounter++;
  u16Counter++;
   
  if(u16Counter==100)//every 100ms change the LCD_light
  {
      u16Counter=0;
      switch(u8ColorType)
      {
		  case 1:
				LedPWM(LCD_RED,LED_PWM_100);
				LedPWM(LCD_GREEN,DutyCycle);
				LedPWM(LCD_BLUE,LED_PWM_0);
				DutyCycle++;
				break;
		  case 2:
				LedPWM(LCD_GREEN,LED_PWM_100);
				LedPWM(LCD_RED,DutyCycle);
				LedPWM(LCD_BLUE,LED_PWM_0);
				DutyCycle--;
				break;
		  case 3:
				LedPWM(LCD_RED,LED_PWM_0);
				LedPWM(LCD_GREEN,LED_PWM_100);
				LedPWM(LCD_BLUE,DutyCycle);
				DutyCycle++;
				break;
		  case 4:
				LedPWM(LCD_RED,LED_PWM_0);
				LedPWM(LCD_BLUE,LED_PWM_100);
				LedPWM(LCD_GREEN,DutyCycle);
				DutyCycle--;
				break;
		  case 5:
				LedPWM(LCD_GREEN,LED_PWM_0);
				LedPWM(LCD_BLUE,LED_PWM_100);
				LedPWM(LCD_RED,DutyCycle);
				DutyCycle++;
				break;
		  case 6:
				LedPWM(LCD_GREEN,LED_PWM_0);
				LedPWM(LCD_RED,LED_PWM_100);
				LedPWM(LCD_BLUE,DutyCycle);
				DutyCycle--;
				break;
		  case 7:
				u8ColorType=0;
				break;
		  default:
				break;
      }
      
      
      if((DutyCycle==LED_PWM_100)||(DutyCycle==LED_PWM_0))
      {
          u8ColorType++;
      }
  }

  if(!(u32TimeCounter%500))//Converts Numbers to characters
  {
	u8TimeMessage[0]=u32TimeCounter/10000;
	u8TimeMessage[1]=(u32TimeCounter%10000)/1000+'0';
	u8TimeMessage[2]=(u32TimeCounter%1000)/100+'0';
	u8TimeMessage[3]=(u32TimeCounter%100)/10+'0';
	u8TimeMessage[4]=u32TimeCounter%10+'0';
	
	if(u8TimeMessage[0])
	{
		u8TimeMessage[1]=9+'0';
		u8TimeMessage[2]=9+'0';
		u8TimeMessage[3]=9+'0';
		u8TimeMessage[4]=9+'0';
	}
	
	LCDMessage(LINE2_START_ADDR+8,&u8TimeMessage[1]);//show the timecounter on the LCD_LINE2
	LCDClearChars(LINE2_START_ADDR+12,16);
  }//end show timecounter

  for(u8 i=0;i<16;i++)//Change the state of the LED
  {
	
	if(u32TimeCounter == aeDemoList[i].u32Time)
	{
	  LedPWM(aeDemoList[i].eLED,aeDemoList[i].eCurrentRate);
	  
	  if(aeDemoList[i].bOn == TRUE)
	  {
	    LCDMessage(LINE1_START_ADDR+au8LCDList[i],"1");
	  }
	  else
	  {
		LCDMessage(LINE1_START_ADDR+au8LCDList[i],"0");
	  }
	}
  }
  
  if(u32TimeCounter == 2000)//End of cycle
  {
	u32TimeCounter=0;
  }
}



static void UserApp1SM_Error()
{

}/* end UserApp1SM_Error() */

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
