Arduino Bluetooth QuickBot 
==================================

![Arduino QuickBot](/photos/20150504_233456.jpg?raw=true "Arduino QuickBot")

This is the QuickBot implementation on the Arduino platform.

Instead of BeagleBone Black i use Arduino UNO.  
Instead of WiFi module i use Bluetooth module HC-06.

It uses the same commands as original QuickBot, but instead of TCP/IP connection  
it uses serial connection.

Robot is based on SparkFun RedBot Kit  
    https://www.sparkfun.com/products/12697.  
Mainboard is SparkFun RedBot Mainboard (Arduino UNO compatible)  
    https://www.sparkfun.com/products/12097.

Android Bluetooth Joystick application to handle QuickBot via Bluetooth module:  
    https://github.com/altexdim/android-bluetooth-joystick

QuickBot originated from coursera course "Control of Mobile Robots":  
    https://www.coursera.org/course/conrob  
Original QuickBot site and repository:  
    http://o-botics.org/robots/quickbot/mooc/v2/

Photos and video
----------------------------------
Photos are in the folder /photos of this repository.

Video of the robot moving around while i'm controlling it with  
Android Bluetooth Joystick application:  
    https://drive.google.com/file/d/0B4F2-uZsSusnc2JXcFBnS0lNSzg

Credits and links
----------------------------------
* CMake project for Arduino (original project):  
    https://github.com/queezythegreat/arduino-cmake  
    Mozilla Public License, v. 2.0 http://mozilla.org/MPL/2.0/

* CMake project for Arduino 1.6+ (fork):  
    https://github.com/altexdim/arduino-cmake

* Coursera course "Control of Mobile Robots":  
    https://www.coursera.org/course/conrob

* Original QuickBot site:  
    http://o-botics.org/robots/quickbot/mooc/v2/

* QuickBot repository:  
    https://github.com/o-botics

* Android Bluetooth Joystick application:  
    https://github.com/altexdim/android-bluetooth-joystick

Commands
----------------------------------

* **$CHECK***  
*OUTPUT*: Hello from QuickBot
Check (ping)  

* **$RESET***  
*OUTPUT*: Encoder values reset to [0, 0]
Reset counter

* **$IRVAL?***  
*OUTPUT*: [100, 150, 200, 250, 300]
Get IR sensor values

* **$ENVAL?***  
*OUTPUT*: [-5, 10]
Get encoders values

* **$ENVEL?***  
*OUTPUT*: [-15, 20]
Get encoders velocities

* **$PWM=0,0***  
*OUTPUT*: None
Stops motors

* **$PWM=123,234***  
*OUTPUT*: None
Set pwm values to motors drivers

* **$PWM?***  
*OUTPUT*: [155, 180]
Get last set pwm values

* **$DEBUG=1***  
*OUTPUT*: Debug set to 1
Enable debug mode  
If enabled, prints debug info every second to the communication connection

* **$DEBUG=0***  
*OUTPUT*: Debug set to 0
Disable debug mode

* **$PERF?***  
*OUTPUT*: Performance = 1850
Get performance counter value.  
This value means how many loops the program runs in one second.
