Arduino Bluetooth QuickBot 
==================================

![Arduino QuickBot](/photos/20150504_233456.jpg?raw=true "Arduino QuickBot")

This is QuickBot implementation on the Arduino platform.

Instead of BeagleBone Black i use Arduino UNO.  
Instead of WiFi module i use Bluetooth module HC-06.

It uses the same commands as original QuickBot, but instead of TCP/IP connection  
it uses serial connection.

Disclaimer: this is proof of concept alfa release,  
source code is ugly, it will be refactored if i will  
find enough free time for that. But it totaly works.

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
