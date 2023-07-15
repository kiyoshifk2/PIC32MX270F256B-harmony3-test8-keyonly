/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "system/time/sys_time.h"

void ut_printf(const char *fmt, ...);

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

/* Usage ID to Key map table */

const char KeyTable_lower[256]=
{
                          0,		// 0x00
                          0,
                          0,
                          0,
                          'a',
                          'b',
                          'c',
                          'd',
                          'e',		// 0x08
                          'f',
                          'g',
                          'h',
                          'i',
                          'j',
                          'k',
                          'l',
                          'm',		// 0x10
                          'n',
                          'o',
                          'p',
                          'q',
                          'r',
                          's',
                          't',
                          'u',		// 0x18
                          'v',
                          'w',
                          'x',
                          'y',
                          'z',
                          '1',
                          '2',
                          '3',		// 0x20
                          '4',
                          '5',
                          '6',
                          '7',
                          '8',
                          '9',
                          '0',
                          '\n',		// enter		// 0x28
                          0x1b,		// escape
                          '\b',		// backspace
                          '\t',		// tab
                          ' ',		// space
                          '-',
                          '^',
                          '@',
                          '[',						// 0x30
                          0,
                          ']',
                          ';',
                          ':',
                          0,
                          ',',
                          '.',
                          '/',						// 0x38
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,		// 0x40
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,		// 0x48
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,		// 0x50
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,		// 0x58
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,		// 0x60
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,		// 0x68
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,		// 0x70
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,		// 0x78
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,		// 0x80
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          '\\',
                          0,		// 0x88
                          '\\',
                          0,
                          0,
                          0,
                          0,
                          0,
                          '\\',
                          0,		// 0x90
};

const char KeyTable_upper[256]=
{
                          0,		// 0x00
                          0,
                          0,
                          0,
                          'A',
                          'B',
                          'C',
                          'D',
                          'E',		// 0x08
                          'F',
                          'G',
                          'H',
                          'I',
                          'J',
                          'K',
                          'L',
                          'M',		// 0x10
                          'N',
                          'O',
                          'P',
                          'Q',
                          'R',
                          'S',
                          'T',
                          'U',		// 0x18
                          'V',
                          'W',
                          'X',
                          'Y',
                          'Z',
                          '!',
                          '\"',
                          '#',		// 0x20
                          '$',
                          '%',
                          '&',
                          '\'',
                          '(',
                          ')',
                          0,
                          '\n',		// enter		// 0x28
                          0x1b,		// escape
                          '\b',		// backspace
                          '\t',		// tab
                          ' ',		// space
                          '=',
                          '~',
                          '`',
                          '{',						// 0x30
                          0,
                          '}',
                          '+',
                          '*',
                          0,
                          '<',
                          '>',
                          '?',						// 0x38
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,		// 0x40
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,		// 0x48
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,		// 0x50
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,		// 0x58
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,		// 0x60
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,		// 0x68
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,		// 0x70
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,		// 0x78
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,		// 0x80
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          '_',
                          0,		// 0x88
                          '|',
                          0,
                          0,
                          0,
                          0,
                          0,
                          '\\',
                          0,		// 0x90
};

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

char APP_ch;
//char old_ch;
USB_HID_KEYBOARD_KEYPAD old_keyCode;
// *****************************************************************************
// *****************************************************************************
// Section: Application Local Routines
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************
 * USB HOST Layer Events - Host Event Handler
 *******************************************************/

USB_HOST_EVENT_RESPONSE APP_USBHostEventHandler (USB_HOST_EVENT event, void * eventData, uintptr_t context)
{
	ut_printf("APP_USBHostEventHandler start\n");//AAAAA
    switch(event)
    {
        case USB_HOST_EVENT_DEVICE_UNSUPPORTED:
            break;
        default:
            break;
    }
    return USB_HOST_EVENT_RESPONSE_NONE;
}

/*******************************************************
 * USB HOST HID Layer Events - Application Event Handler
 *******************************************************/

void APP_USBHostHIDKeyboardEventHandler(USB_HOST_HID_KEYBOARD_HANDLE handle, 
        USB_HOST_HID_KEYBOARD_EVENT event, void * pData)
{   
//	ut_printf("APP_USBHostHIDKeyboardEventHandler start\n");//AAAAA
    switch ( event)
    {
        case USB_HOST_HID_KEYBOARD_EVENT_ATTACH:
            appData.handle = handle;
            appData.state =  APP_STATE_DEVICE_ATTACHED;
            appData.nBytesWritten = 0;
            appData.stringReady = false;
            memset(&appData.string, 0, sizeof(appData.string));
            memset(&appData.lastData, 0, sizeof(appData.lastData));
            appData.stringSize = 0;
            appData.capsLockPressed = false;
            appData.scrollLockPressed = false;
            appData.numLockPressed = false;
            appData.outputReport = 0;
            break;

        case USB_HOST_HID_KEYBOARD_EVENT_DETACH:
            appData.handle = handle;
            appData.state = APP_STATE_DEVICE_DETACHED;
            appData.nBytesWritten = 0;
            appData.stringReady = false;
            appData.usartTaskState = APP_USART_STATE_CHECK_FOR_STRING_TO_SEND;
            memset(&appData.string, 0, sizeof(appData.string));
            memset(&appData.lastData, 0, sizeof(appData.lastData));
            appData.stringSize = 0;
            appData.capsLockPressed = false;
            appData.scrollLockPressed = false;
            appData.numLockPressed = false;
            appData.outputReport = 0;
            break;

        case USB_HOST_HID_KEYBOARD_EVENT_REPORT_RECEIVED:
            appData.handle = handle;
            appData.state = APP_STATE_READ_HID;
            /* Keyboard Data from device */
            memcpy(&appData.data, pData, sizeof(appData.data));
            break;

        default:
            break;
    }
    return;
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary local functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    memset(&appData, 0, sizeof(appData));
    appData.state = APP_STATE_INIT;
    appData.usartTaskState = APP_USART_STATE_DRIVER_OPEN;
}


void APP_USART_Tasks(void)
{
    if(appData.stringReady){ //AAAAA
        ut_printf((char*)appData.string);//AAAAA
        appData.stringReady = false;//AAAAA
    }
}



void APP_Driver ( void )
{
    char ch;
    
    appData.currentOffset = 0;
    
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
//        	ut_printf("APP_STATE_INIT\n");//AAAAA
            USB_HOST_EventHandlerSet(APP_USBHostEventHandler, 0);
            USB_HOST_HID_KEYBOARD_EventHandlerSet(APP_USBHostHIDKeyboardEventHandler);
            
			USB_HOST_BusEnable(0);
			appData.state = APP_STATE_WAIT_FOR_HOST_ENABLE;
            break;
			
		case APP_STATE_WAIT_FOR_HOST_ENABLE:
//			ut_printf("APP_STATE_WAIT_FOR_HOST_ENABLE\n");//AAAAA
            /* Check if the host operation has been enabled */
            if(USB_HOST_BusIsEnabled(0))
            {
                /* This means host operation is enabled. We can
                 * move on to the next state */
                appData.state = APP_STATE_HOST_ENABLE_DONE;
            }
            break;
        case APP_STATE_HOST_ENABLE_DONE:
            appData.stringSize = 64;
            memcpy(&appData.string[0], "\r\n***Connect Keyboard***\r\n",
                        sizeof("\r\n***Connect Keyboard***\r\n"));
            appData.stringReady = true;
            /* The test was successful. Lets idle. */
            appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
            break;

        case APP_STATE_WAIT_FOR_DEVICE_ATTACH:

            /* Wait for device attach. The state machine will move
             * to the next state when the attach event
             * is received.  */

            break;

        case APP_STATE_DEVICE_ATTACHED:
            
            /* Wait for device report */
            memcpy(&appData.string[0], "---Keyboard Connected---\r\n",
                    sizeof("---Keyboard Connected---\r\n"));
            appData.stringReady = true;
            appData.stringSize = 64;
            appData.state = APP_STATE_READ_HID;
            break;

        case APP_STATE_READ_HID:
//            for(count = 0; count < appData.data.nNonModifierKeysData; count++)
//            {
                if(appData.data.nonModifierKeysData[0].event == USB_HID_KEY_PRESSED)
                {
                    /* scan code to ascii charactor */
                    // if upper case
                    if(appData.data.modifierKeysData.leftShift || appData.data.modifierKeysData.rightShift)
                        ch = KeyTable_upper[(unsigned char)appData.data.nonModifierKeysData[0].keyCode];
                    // if lower case
                    else
    					ch = KeyTable_lower[(unsigned char)appData.data.nonModifierKeysData[0].keyCode];
					if(old_keyCode != appData.data.nonModifierKeysData[0].keyCode)
					{
//						ut_printf("0x%02x ", appData.data.modifierKeysData);
//						ut_printf("0x%02x ", appData.data.nonModifierKeysData[0].keyCode);
//						ut_printf("0x%02x %c\n", ch, ch);
                        if(APP_ch==0)
                            APP_ch = ch;
						old_keyCode = appData.data.nonModifierKeysData[0].keyCode;
					}
                }
                else{
                    old_keyCode = 0xff;
                }
//            }
            break;

        case APP_STATE_DEVICE_DETACHED:
            appData.state = APP_STATE_HOST_ENABLE_DONE;
            break;

        case APP_STATE_ERROR:

            /* The application comes here when the demo
             * has failed. Provide LED indication .*/

//            LED1_On();
            break;

        default:
            break;
    }

}

//
//      APP_Driver inputed data display
//
void APP_MAIN(void)
{
    char ch;
    
    if(APP_ch){                 // exist new key data
        ch = APP_ch;
        APP_ch = 0;
        ut_printf("%c", ch);
    }
}


void APP_Tasks ( void )
{
    APP_Driver();
    APP_USART_Tasks();
    APP_MAIN();
}

/*******************************************************************************
 End of File
 */
