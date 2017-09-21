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
extern bool bOk=TRUE;

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
  LedOff(RED);
  LedOff(WHITE);
  LedOff(YELLOW);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(BLUE);
  LedOff(PURPLE);
  LedOff(ORANGE);
	
  PWMAudioOff(BUZZER1);
  LCDCommand(LCD_CLEAR_CMD);

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

/*STATE 1*/
static void UserApp1SM_state1(void)
{
    /*enter if just one time*/
	if(bOk)
	{
		DebugPrintf("Entering state 1");
		DebugLineFeed();
		PWMAudioOff(BUZZER1);
		LCDMessage(LINE1_START_ADDR,"STATE 1");
		LedOff(RED);
		LedOn(WHITE);
		LedOff(YELLOW);
		LedOn(CYAN);
		LedOff(GREEN);
		LedOn(BLUE);
		LedOn(PURPLE);
		LedOff(ORANGE);
		LedPWM(LCD_GREEN,LED_PWM_0);
		LedPWM(LCD_BLUE,LED_PWM_100);
		LedPWM(LCD_RED,LED_PWM_100);
		bOk=FALSE;
	}

	
	/*if BUTTON2 pressed , switch to state 2 */	
  	if(WasButtonPressed(BUTTON2))
	{
		ButtonAcknowledge(BUTTON2);
		UserApp1_StateMachine=UserApp1SM_state2;	
		bOk=TRUE;
	}
}


/*STATE 2*/
static void UserApp1SM_state2(void)
{
  static u32 u32Time=0;
  
  u32Time++;
  
  /*enter if just one time*/
	if(bOk)
	{
		DebugPrintf("Entering state 2");
		DebugLineFeed();
		PWMAudioSetFrequency(BUZZER1,200);
		PWMAudioOn(BUZZER1);
		LCDMessage(LINE1_START_ADDR,"STATE 2");
		LedOff(RED);
		LedOff(WHITE);
		LedOff(YELLOW);
		LedOff(CYAN);
		LedOff(GREEN);
		LedOff(BLUE);
		LedOff(PURPLE);
		LedOff(ORANGE);
		LedBlink(GREEN,LED_1HZ);
		LedBlink(YELLOW,LED_2HZ);
		LedBlink(ORANGE,LED_4HZ);
		LedBlink(RED,LED_8HZ);
		LedPWM(LCD_BLUE,LED_PWM_0);
		LedPWM(LCD_GREEN,LED_PWM_25);
		LedPWM(LCD_RED,LED_PWM_100);
		bOk=FALSE;
	}
  
	/*BUZZER off at 100ms*/
	if(u32Time == 100)
	{
		PWMAudioOff(BUZZER1);
	}

	/*reset the time*/
	if(u32Time == 1000)
	{
		u32Time = 0;
		PWMAudioOn(BUZZER1);
	}

	/*if BUTTON1 pressed , switch to state 1 */
  	if(WasButtonPressed(BUTTON1))
	{
		ButtonAcknowledge(BUTTON1);
		UserApp1_StateMachine=UserApp1SM_state1;	
		bOk=TRUE;
	}
  
}

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
	static u8 u8FlagState=0;

	if(WasButtonPressed(BUTTON1))
	{
		ButtonAcknowledge(BUTTON1);
		u8FlagState=1;	
		bOk=TRUE;
	}

	if(WasButtonPressed(BUTTON2))
	{
		ButtonAcknowledge(BUTTON2);
		u8FlagState=2;
		bOk=TRUE;
	}

	/*if button1 is pressed ,then go to state1*/
	if(u8FlagState == 1)
	{
		UserApp1_StateMachine=UserApp1SM_state1;
	}

	/*if button2 is pressed ,then go to state2*/
	if(u8FlagState == 2)
	{
		UserApp1_StateMachine=UserApp1SM_state2;
	}
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
