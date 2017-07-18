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

static u8 UserApp_CursorPosition;

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
  UserApp_CursorPosition = LINE1_END_ADDR+1;
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
	static u8  u8Input[255];
	static u32 u32Count=0;
	static u8  u8Number=0;
	static u8  u8No=19;
	static u8  u8CharNumber=0;
	static bool bOk=FALSE;

	if(WasButtonPressed(BUTTON0))//comfirm to input the data
	{
		ButtonAcknowledge(BUTTON0);
		u8Number=DebugScanf(u8Input);
		bOk=TRUE;
		u32Count=0;
	}
	
	u32Count++;
	
	if(bOk)
	{
		if(u8Number<=40)
		{
		  LCDMessage(LINE1_START_ADDR,u8Input);
		  LCDMessage(LINE2_START_ADDR,&u8Input[20]);
		  bOk=FALSE;
		}
		else if(u32Count==400)//400ms
		{
		  u32Count=0;
		  
		  if(UserApp_CursorPosition>0)
		  {
			LCDMessage(--UserApp_CursorPosition,u8Input);
			LCDClearChars(LINE2_START_ADDR,20);
			
	    	if(u8No>0)
			{
			  LCDClearChars(LINE1_START_ADDR,--u8No);
			}
			
		  }
		  else
		  {
			LCDMessage(UserApp_CursorPosition,&u8Input[++u8CharNumber]);
			LCDClearChars(LINE2_START_ADDR,20);

			if((u8Number-u8CharNumber)<20)
			{
			  LCDClearChars(u8Number-u8CharNumber,1);
			}
		  }
		  
		  if(u8CharNumber==u8Number)//end
		  {
			UserApp_CursorPosition=LINE1_END_ADDR+1;
			u8CharNumber=0;
			u8No=19;
		  }
		  
	 	}
	}
}

static void UserApp1SM_Error()
{

}/* end UserApp1SM_Error() */

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
