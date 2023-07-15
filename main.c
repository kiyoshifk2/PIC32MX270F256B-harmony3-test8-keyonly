/*******************************************************************************
  MPLAB Harmony Project Main Source File

  Company:
    Microchip Technology Inc.
  
  File Name:
    main.c

  Summary:
    This file contains the "main" function for an MPLAB Harmony project.

  Description:
    This file contains the "main" function for an MPLAB Harmony project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state 
    machines of all MPLAB Harmony modules in the system and it calls the 
    "SYS_Tasks" function from within a system-wide "super" loop to maintain 
    their correct operation. These two functions are implemented in 
    configuration-specific files (usually "system_init.c" and "system_tasks.c")
    in a configuration-specific folder under the "src/system_config" folder 
    within this project's top-level folder.  An MPLAB Harmony project may have
    more than one configuration, each contained within it's own folder under
    the "system_config" folder.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

//Microchip licenses to you the right to use, modify, copy and distribute
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

#include <stdio.h>                      // Def vsprintf
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdarg.h>
//#include "config/default/system/system_module.h"   // SYS function prototypes
//#include "system/debug/sys_debug.h"
#include "definitions.h"

void spi_init();
void display(int x, int y, const char * str);
void disp_str(const char *str);
void ut_printf(const char *fmt, ...);


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
//
//		USB: Keyboard only
//
//		wait 200ms at 40MHz clock
//		volatile int i;
//		for(i=0; i<1000000; i++) ;
//
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all MPLAB Harmony modules, including application(s). */
    SYS_Initialize ( NULL );

    spi_init();
    
    disp_str("Hello World!!!\n");

    TRISBbits.TRISB4 = 0;           // LED output
    LATBbits.LATB4 = 1;

//    volatile int cnt;
//    for(;;){
//        LATBbits.LATB4 = 1;
//		for(cnt=0; cnt<2000000; cnt++)
//			;
//        LATBbits.LATB4 = 0;
//		for(cnt=0; cnt<2000000; cnt++)
//			;
//	}
    
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );

    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/



int master_level = 0;

void SYS_DEBUG_PRINT(int level, const char *fmt, ...)
{
    char buf[80];
	va_list ap;

	if(level>master_level)
		return;

	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);
	
    disp_str(buf);
}

void SYS_DEBUG_MESSAGE(int level, const char *fmt, ...)
{
	char buf[80];
	va_list ap;

	if(level>master_level)
		return;
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);
	
    disp_str(buf);
}

void SYS_DEBUG(int level, const char *fmt)
{
	if(level>master_level)
		return
	
    disp_str(fmt);
}
