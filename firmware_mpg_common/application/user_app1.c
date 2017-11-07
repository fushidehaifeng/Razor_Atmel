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
extern u32 G_u32AntApiCurrentMessageTimeStamp;                    /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;                /* From ant_api.c */

extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;              /* The state machine function pointer */

static AntAssignChannelInfoType UserApp1_sChannelInfo_MASTER,UserApp1_sChannelInfo_SLAVER; /* ANT setup parameters */

static bool bStatus = FALSE;
static bool bRestart = FALSE;
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
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(RED);
  
  PWMAudioOff(BUZZER1);
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR,"Hide and Go Seek!");
  LCDMessage(LINE2_START_ADDR,"Press B1 to switch");
  
  if(1)
  {
    UserApp1_StateMachine = UserApp1SM_AntChoose;
  }
  else
  {
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

static void UserApp1SM_AntChoose()
{
  static bool bOk=FALSE;
  
  if( WasButtonPressed(BUTTON1) )
  {
    LCDCommand(LCD_CLEAR_CMD);
    ButtonAcknowledge(BUTTON1);
    bStatus=!bStatus;
    bOk=TRUE;
  }
  
  /**use bStatus to distinguish slave and hide**/
  /**use bOk     to know if BUTTON1 is Pressed**/
  if( bStatus && bOk )
  {
    LCDMessage(LINE1_START_ADDR,"YOU ARE HIDE!");
    LCDMessage(LINE2_START_ADDR,"Press BO to start");
    
    UserApp1_sChannelInfo_MASTER.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP_MASTER;
    UserApp1_sChannelInfo_MASTER.AntChannel          = ANT_CHANNEL_USERAPP_MASTER;
    UserApp1_sChannelInfo_MASTER.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
    UserApp1_sChannelInfo_MASTER.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;

    UserApp1_sChannelInfo_MASTER.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
    UserApp1_sChannelInfo_MASTER.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
    UserApp1_sChannelInfo_MASTER.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
    UserApp1_sChannelInfo_MASTER.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
    UserApp1_sChannelInfo_MASTER.AntFrequency        = ANT_FREQUENCY_USERAPP;
    UserApp1_sChannelInfo_MASTER.AntTxPower          = ANT_TX_POWER_USERAPP;

    UserApp1_sChannelInfo_MASTER.AntNetwork = ANT_NETWORK_DEFAULT;
  
    for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
    {
      UserApp1_sChannelInfo_MASTER.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
    }
    
    bOk=FALSE;
  }
  else if( bOk )
  {
    LCDMessage(LINE1_START_ADDR,"YOU ARE FIND!");
    LCDMessage(LINE2_START_ADDR,"Press BO to start");
    
    UserApp1_sChannelInfo_SLAVER.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP_SLAVE;
    UserApp1_sChannelInfo_SLAVER.AntChannel          = ANT_CHANNEL_USERAPP_SLAVER;
    UserApp1_sChannelInfo_SLAVER.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
    UserApp1_sChannelInfo_SLAVER.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;

    UserApp1_sChannelInfo_SLAVER.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
    UserApp1_sChannelInfo_SLAVER.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
    UserApp1_sChannelInfo_SLAVER.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
    UserApp1_sChannelInfo_SLAVER.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
    UserApp1_sChannelInfo_SLAVER.AntFrequency        = ANT_FREQUENCY_USERAPP;
    UserApp1_sChannelInfo_SLAVER.AntTxPower          = ANT_TX_POWER_USERAPP;

    UserApp1_sChannelInfo_SLAVER.AntNetwork = ANT_NETWORK_DEFAULT;
  
    for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
    {
      UserApp1_sChannelInfo_SLAVER.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
    }

    bOk=FALSE;
  }
  
  /* Attempt to queue the ANT channel setup */
  if( WasButtonPressed(BUTTON0) )
  {
    ButtonAcknowledge(BUTTON0);
    
    /**either of the status is configured**/
    if(  (bStatus && AntAssignChannel(&UserApp1_sChannelInfo_MASTER) ) || ( !bStatus && AntAssignChannel(&UserApp1_sChannelInfo_SLAVER) ) )
    {
      UserApp1_StateMachine = UserApp1SM_AntChannelAssign;
    }
  }
}

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT channel assignment */
static void UserApp1SM_AntChannelAssign()
{
    /* Channel assignment is successful, so open channel and
    proceed to Idle state */
    
    if(bStatus &&  AntRadioStatusChannel(ANT_CHANNEL_USERAPP_MASTER) == ANT_CONFIGURED)
    {
      AntOpenChannelNumber(ANT_CHANNEL_USERAPP_MASTER);
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR,"Hide!");
      UserApp1_StateMachine = UserApp1SM_Idle_Master;
    }
    
    else if(!bStatus && AntRadioStatusChannel(ANT_CHANNEL_USERAPP_SLAVER) == ANT_CONFIGURED)
    {
      AntOpenChannelNumber(ANT_CHANNEL_USERAPP_SLAVER);
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR,"Seeker");
      UserApp1_StateMachine = UserApp1SM_Idle_Slave;
      PWMAudioOn(BUZZER1);
    }
}
 /* end UserApp1SM_AntChannelAssign */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle_Master(void)
{
  static u8 au8TestMessage[] = {6, 6, 6, 2, 0xA5, 0, 0, 0};
  static u8 au8Restart[30]="PRESS B0 TO RESTART!";
  
  if( AntReadAppMessageBuffer() )
  {
     /* New message from ANT task: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      /**if get message which begin with "F",means already found**/
      if(G_au8AntApiCurrentMessageBytes[0] == 'F')
      {
        LCDCommand(LCD_CLEAR_CMD);
        LCDMessage(LINE1_START_ADDR,"YOU FOUND ME!");
        LCDMessage(LINE2_START_ADDR,&au8Restart[0]);
        bRestart=TRUE;
      }
    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      /**Keep Broadcasting**/
      AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP_MASTER, au8TestMessage);
    }
    
    /**if found and press BUTTON0 at the same time ,go back to UserApp1Initialize**/
    if( bRestart && WasButtonPressed(BUTTON0) )
    {
      ButtonAcknowledge(BUTTON0);
      bRestart = FALSE;
      AntCloseChannelNumber(ANT_CHANNEL_USERAPP_MASTER);
      AntUnassignChannelNumber(ANT_CHANNEL_USERAPP_MASTER);
      UserApp1_StateMachine = UserApp1Initialize;
    }
  } /* end AntReadData() */
} /* end UserApp1SM_Idle_Master() */


static void UserApp1SM_Idle_Slave(void)
{
    static u32 u32Time = 0;
    static u8 au8Feedback[]="FIND YOU!";
    static s8 SignalStrength = 0;
    static s8 CurrentSignal = 0;
    static u8 au8Restart[30]="PRESS B0 TO RESTART!";
    
    /**COUNT 10S**/
    if(u32Time <= 10000)
    {
      u32Time++;
    }
    
    switch(u32Time)
    {
      case 1    :   LCDMessage(LINE2_START_ADDR,"10 s");  break;
      case 1000 :   LCDMessage(LINE2_START_ADDR,"9  s");  break;
      case 2000 :   LCDMessage(LINE2_START_ADDR,"8  s");  break;
      case 3000 :   LCDMessage(LINE2_START_ADDR,"7  s");  break;
      case 4000 :   LCDMessage(LINE2_START_ADDR,"6  s");  break;
      case 5000 :   LCDMessage(LINE2_START_ADDR,"5  s");  break;
      case 6000 :   LCDMessage(LINE2_START_ADDR,"4  s");  break;
      case 7000 :   LCDMessage(LINE2_START_ADDR,"3  s");  break;
      case 8000 :   LCDMessage(LINE2_START_ADDR,"2  s");  break;
      case 9000 :   LCDMessage(LINE2_START_ADDR,"1  s");  break;
      case 10000:   LCDMessage(LINE2_START_ADDR,"HERE I COME!");  break;
      default   :   break;
    }
    
    if( AntReadAppMessageBuffer() )
    {
     /* New data message: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      SignalStrength=-1*G_sAntApiCurrentMessageExtData.s8RSSI;
      
      /**use RSSI to control the LED and BUZZER1**/
      /**when time count finished**/
      if(CurrentSignal != (SignalStrength-40)/5 && u32Time >= 10000)
      {
           CurrentSignal = (SignalStrength-40)/5;
           
          switch(CurrentSignal)
          {
          case 1:
            LedOn(WHITE);
            LedOn(PURPLE);
            LedOn(BLUE);
            LedOn(CYAN);
            LedOn(GREEN);
            LedOn(YELLOW);
            LedOn(ORANGE);
            LedOn(RED);
            /**in case 1:turn on all LEDs and send a special message to the HIDE**/
            PWMAudioSetFrequency(BUZZER1,400);
            AntQueueAcknowledgedMessage(ANT_CHANNEL_USERAPP_SLAVER,au8Feedback);
            LCDCommand(LCD_CLEAR_CMD);
            LCDMessage(LINE1_START_ADDR,"I FOUND YOU!");
            LCDMessage(LINE2_START_ADDR,&au8Restart[0]);
            bRestart=TRUE;
            break;
          case 2:
            LedOn(WHITE);
            LedOn(PURPLE);
            LedOn(BLUE);
            LedOn(CYAN);
            LedOn(GREEN);
            LedOn(YELLOW);
            LedOn(ORANGE);
            LedOff(RED);
            PWMAudioSetFrequency(BUZZER1,350);
            break;
          case 3:
            LedOn(WHITE);
            LedOn(PURPLE);
            LedOn(BLUE);
            LedOn(CYAN);
            LedOn(GREEN);
            LedOn(YELLOW);
            LedOff(ORANGE);
            LedOff(RED);
            PWMAudioSetFrequency(BUZZER1,300);
            break;
          case 4:        
            LedOn(WHITE);
            LedOn(PURPLE);
            LedOn(BLUE);
            LedOn(CYAN);
            LedOn(GREEN);
            LedOff(YELLOW);
            LedOff(ORANGE);
            LedOff(RED);
            PWMAudioSetFrequency(BUZZER1,250);
            break;
          case 5:
            LedOn(WHITE);
            LedOn(PURPLE);
            LedOn(BLUE);
            LedOn(CYAN);
            LedOff(GREEN);
            LedOff(YELLOW);
            LedOff(ORANGE);
            LedOff(RED);
            PWMAudioSetFrequency(BUZZER1,200);
            break;
          case 6:
            LedOn(WHITE);
            LedOn(PURPLE);
            LedOn(BLUE);
            LedOff(CYAN);
            LedOff(GREEN);
            LedOff(YELLOW);
            LedOff(ORANGE);
            LedOff(RED);
            PWMAudioSetFrequency(BUZZER1,150);
            break;
          case 7:
            LedOn(WHITE);
            LedOn(PURPLE);
            LedOff(BLUE);
            LedOff(CYAN);
            LedOff(GREEN);
            LedOff(YELLOW);
            LedOff(ORANGE);
            LedOff(RED);
            PWMAudioSetFrequency(BUZZER1,100);
            break;
          case 8:
            LedOn(WHITE);
            LedOff(PURPLE);
            LedOff(BLUE);
            LedOff(CYAN);
            LedOff(GREEN);
            LedOff(YELLOW);
            LedOff(ORANGE);
            LedOff(RED);
            PWMAudioSetFrequency(BUZZER1,0);
            break;
          default:
            break;
          }/*end of switch*/
      }/*end of if*/

      /**Press BUTTON3 to close the BUZZER1**/
      if( WasButtonPressed(BUTTON3) )
      {
        ButtonAcknowledge(BUTTON3);
        PWMAudioOff(BUZZER1);
      }
      
      /**Press BUTTON2 to open the BUZZER1**/
      if( WasButtonPressed(BUTTON2) )
      {
        ButtonAcknowledge(BUTTON2);
        PWMAudioOn(BUZZER1);
      }
      
      /**if found and press BUTTON0 at the same time ,go back to UserApp1Initialize**/
      if( bRestart && WasButtonPressed(BUTTON0) )
      {
        ButtonAcknowledge(BUTTON0);
        u32Time=0;
        bRestart = FALSE;
        AntCloseChannelNumber(ANT_CHANNEL_USERAPP_SLAVER);
        AntUnassignChannelNumber(ANT_CHANNEL_USERAPP_MASTER);
        UserApp1_StateMachine = UserApp1Initialize;
      }
    } /* end if(G_eAntApiCurrentMessageClass == ANT_DATA) */
} /* end UserApp1SM_Idle_Slave() */
}


/* Handle an error (for now, do nothing) */
static void UserApp1SM_Error(void)          
{
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
