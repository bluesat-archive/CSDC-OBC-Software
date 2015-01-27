CSDC-OBC-Software
=================

FreeRTOS Kernel, initially ported to the SAM3X series.

Contents
--------

* [Quickstart Guide](#quickstart-guide-for-sam3x-ek-demo-application)
* [Code Structure Guide](#code-structure-guide)
  * [Folder Structure](#folder-structure)
  * [Naming Conventions](#naming-conventions)
* [Applications](#applications)
* [Drivers](#drivers-note-1)
* [Creating a new application](#creating-a-new-application)
* [References](#references)



Quickstart Guide for SAM3X-EK Demo Application
------------------

The project's original source can be found [here](http://www.freertos.org/Atmel_SAM3_SAM3X-EK_SAM3S-EK2_RTOS_Demo.html).

The following will help you set up the development environment necessary to Flash the FreeRTOS SAM3X-EK Demo onto any board with a ATSAM3X chip.  

1. Download the Atmel IDE from [here](http://www.atmel.com/microsite/atmel_studio6/default.aspx). Atmel Studio 6 is built around Visual Studio 10 and works exactly the same way. It's well made, fully featured IDE.

2. Open CORTEX_ATSAM3X_Atmel_Studio_demo/RTOSDemo.atsln to open the project. The source files should be viewable within Atmel Studio

3. Do what you need to do via the solution explorer and navigating between files

4. Hit F7, or go Build->Build Solution in order to build the project. The project should successfully compile with no errors. If not, try clean and Build again,

5. (TBD) Burn the program onto the dev board using the SAM ICE

Code Structure Guide
----------------------------

The [OldSat CSC software](https://github.com/bluesat/csc-software/tree/master/BLUEsat-CSC) serves as a good guide to the structure of code we want to see.

### Folder Structure
```
> src
-> Applications
--> Application_1
---> *c and *h files
--> Applicaiton_2
---> *c and *h files
-> Drivers (see note 1)
--> Atmel Drivers
---> (see note 1)
--> Driver_1
---> *c and *h files
--> Driver_2
---> *c and *h files
-> FreeRTOS
--> Copy structure exactly from exitsing FreeRTOS code
```

### Naming Conventions

* BLUEsat applications are appended with '_app'.
* BLUEsat drivers are appended with '_drv'.
* BLUEsat is spelt BLUEsat and not Bluesat.

Applications
-------------
These are essentially the PortTask functions that will execute in the application layer. They will do most of the high level tasks for the CubeSat. This level is quite abstract and example applications could be 

* TelemetryReporter
* PowerManager
* AttitudeManager

Drivers (note 1)
---------------------
Drivers will be split into two categories: 

1. Low level drivers written by Atmel/Other Chip manufacturers.
2. High(er) level drivers written by us for BLUEsat boards

The low level drivers should keep the same folder/code structure as provided by atmel (you may not even need to include them if you're staying in Atmel Studio)

The BLUEsat drivers will be things written by us used to control the boards and low-level things not currently handled easily by the Atmel drivers. Examples include

* DebugWrite (through UART)
* TransmitStringToEarth (*char outputBuff)
* ReadCommandFromEarth (*char inputBuff)
* SetAttitude(int angle1, int angle2, int angle3) 

To Do 
-----------
_(Last updated Jan 24th 2015)_

1. Debug Terminal through UART - DebugWrite(char *outputBuffer);
2. Comms drivers for CAN
  * CAN_Read(int channel_no, char\* inputBuffer)
  * CAN_write(int channel, char\* outputBuffer)  (*see [this code](https://github.com/UTAT-SpaceSystems/CDH-OBC_PhaseI/blob/master/Code/src/housekeep_test.c)*)
3. _
4. _
5. _

Creating a new application
----
_Refer to BLUEsat_Blink as an example._
#### Notes

* Applications are saved in the folder ../src/Applications/"Application Name"/
* All application *.c and *.h files should be saved in the relevant application folder
* The application needs to include a function to run xTaskCreate, portTASK_FUNCTION, and portTASK_FUNCTION_PROTO
* Add '#include <"Application Name"/\*.h> into main_full under "BLUESAT APPLICATIONS"
* Start the application in main_full()
* _**Important**_: 'vTaskNameTask' must be exactly the same in all cases below.
* _**Important**_: 'vStartTaskNameTasks' must be exactly the same in main_full() and the application *.c and *.h files.

#### In the Application \*.c and \*.h files

##### *portTASK_FUNCTION_PROTO*
```c
static portTASK_FUNCTION_PROTO( vTaskNameTask, pvParameters );
```
Change:

* 'vTaskNameTask' to the name of the application, retain the 'v' at the front and 'Task' at the end. 

##### *Starting the Application*

```c
void vStartTaskNameTasks( UBaseType_t uxPriority )
{
	short sTask;
	
	for( sTask = 0; sTask < intgNUMBER_OF_TASKS; sTask++ )
	{
		xTaskCreate( vTaskNameTask, "TaskName", intgSTACK_SIZE, ( void * ) &( xTaskCheck[ sTask ] ), uxPriority, ( TaskHandle_t * ) NULL );
	}
}
```
Change:

* 'vStartTaskNameTasks' to the name of the application with the 'vStart' at the front and 'Tasks' at the end.
* 'vTaskNameTask' to the name of the application, retain the 'v' at the front and 'Task' at the end. 
* 'TaskName' to the name of the application.

##### *portTASK_FUNCTION*

_This is just an overview of the structure; missing error handling._

```c
static portTASK_FUNCTION( vTaskNAmeTask, pvParameters )
{
	for( ;; )
	{		
		// Application code goes inside here
	}
}
```

Change:

* 'vTaskNameTask' to the name of the application, retain the 'v' at the front and 'Task' at the end. 

#### In main_full()

```c
/* BLUESAT APPLICATIONS - INCLUDES */
#include <TaskName/TaskName_app.h>
```
```c
void main_full( void )
{
	...
	/* BLUESAT APPLICATIONS - START*/
	vStartTaskNameTasks( tskIDLE_PRIORITY );
	...
}
```
Change:

* 'TaskName' to the name of the application.
* 'vStartTaskNameTasks' to the name of the application with the 'vStart' at the front and 'Tasks' at the end.

References
----------
* [Arduino Due Overview](http://arduino.cc/en/Main/arduinoBoardDue)
* [Ardiuno Due Pin Mapping](http://arduino.cc/en/Hacking/PinMappingSAM3X)
* [Arduino Due Schematic](http://arduino.cc/en/uploads/Main/arduino-Due-schematic.pdf)
* [Atmel ATSAM3X8E Overview](http://www.atmel.com/devices/SAM3X8E.aspx?tab=overview)
* [Atmel Application Note - UART](http://www.atmel.com/Images/Atmel-42299-Universal-Asynchronous-Receiver-Transceiver-UART_ApplicationNote_AT07896.pdf)
  * This application note describes how to use the ASF driver for interfacing to the UART module on on SAM.
* [Atmel Application Note - Getting Started with FreeRTOS on Atmel](http://www.atmel.com/Images/Atmel-42382-Getting-Started-with-FreeRTOS-on-Atmel-SAM-Flash-MCUs_ApplicationNote_AT04056.pdf)
  * This application note illustrates the basic functionality of the FreeRTOS Real Time Operating System and shows how to use it on SAM microcontroller.
* [SAM3X/SAM3A Series Summary](http://www.atmel.com/Images/doc11057s.pdf)
  * PIO Register Mapping: Page 654
  * USART Register Mapping: Page 835
* [SAM3X/SAM3A Series Complete](http://www.atmel.com/Images/doc11057.pdf)