CSDC-OBC-Software
====

Everything has moved to Atlasian services, the new git repo can be found here: https://bitbucket.org/bluesat/cubesat-obc-software


Quickstart Guide for SAM3X-EK Demo Application
------------------

The project's original source can be found [here](http://www.freertos.org/Atmel_SAM3_SAM3X-EK_SAM3S-EK2_RTOS_Demo.html).

The following will help you set up the development environment necessary to Flash the FreeRTOS SAM3X-EK Demo onto any board with a ATSAM3X chip.  

1. Download the Atmel IDE from [here](http://www.atmel.com/microsite/atmel_studio6/default.aspx). Atmel Studio 6 is built around Visual Studio 10 and works exactly the same way. It's well made, fully featured IDE.

2. Open CORTEX_ATSAM3X_Atmel_Studio_demo/RTOSDemo.atsln to open the project. The source files should be viewable within Atmel Studio

3. Do what you need to do via the solution explorer and navigating between files

4. Hit F7, or go Build->Build Solution in order to build the project. The project should successfully compile with no errors. If not, try clean and Build again,

5. (TBD) Burn the program onto the dev board using the SAM ICE

Applications
-------------
These are essentially the PortTask functions that will execute in the application layer. They will do most of the high level tasks for the CubeSat. This level is quite abstract and example applications could be 

* TelemetryReporter
* PowerManager
* AttitudeManager

Drivers
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

1. Debug Terminal through UART - DebugWrite(char \*outputBuffer);
2. Comms drivers for CAN
  * CAN_Read(int channel_no, char\* inputBuffer)
  * CAN_write(int channel, char\* outputBuffer)  (\*see [this code](https://github.com/UTAT-SpaceSystems/CDH-OBC_PhaseI/blob/master/Code/src/housekeep_test.c)*)
3. _
4. _
5. _

References
----------
* [Arduino Due Overview](http://arduino.cc/en/Main/arduinoBoardDue)
* [Ardiuno Due Pin Mapping](http://arduino.cc/en/Hacking/PinMappingSAM3X)
* [Arduino Due Schematic](http://arduino.cc/en/uploads/Main/arduino-Due-schematic.pdf)
* [Atmel ATSAM3X8E Overview](http://www.atmel.com/devices/SAM3X8E.aspx?tab=overview)
* [Atmel Application Note - Getting Started with FreeRTOS on Atmel](http://www.atmel.com/Images/Atmel-42382-Getting-Started-with-FreeRTOS-on-Atmel-SAM-Flash-MCUs_ApplicationNote_AT04056.pdf)
* [SAM3X/SAM3A Series Summary](http://www.atmel.com/Images/doc11057s.pdf)
* [SAM3X/SAM3A Series Complete](http://www.atmel.com/Images/doc11057.pdf)
