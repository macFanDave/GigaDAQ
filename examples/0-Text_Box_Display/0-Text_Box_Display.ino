/* ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  
0-Text_Box_Display - A demonstration of the basic text box display on a GigaDAQ

This sketch only works with an Arduino GIGA R1 WiFi with an Arduino GIGA Display Shield.
GigaDAQ is the software to enable this hardware to be a stand-alone data acquisition system.

This example shows how to create a text box display and change the values shown.

Created on Feb 9, 2025
by David A. Trevas

Notes:
1. Unless your real-time clock (RTC) always has power, this code will generate an arbitrary
time. Setting the clock will be addressed in a later example.

MIT License

Copyright (c) 2025 David A. Trevas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~ */

#include <GigaDAQ.h>

//One of the following two lines must be commented out and the other un-commented.

GigaDAQ daq(PORTRAIT_USBDOWN);  //Tall, narrow appearance (iPhone-like)
//GigaDAQ daq(LANDSCAPE_USBRIGHT);  //Wide, short appearance (TV-like)

tm *timePointer;   //Required for time display. From the standard C time.h library

void setup() {
  daq.begin();

	//Create the first text box (number 0) with the name "Time display".
	//The text box will start at 5% of the screen width from the left edge and 20% of the screen height 
	//from the top edge. The box with be 90% of the screen width and 15% of the screen height.
	//The text color will be black and the background white.
	
  daq.textbox[0] = Textbox("Time display", 5, 20, 90, 15, BLACK, WHITE);
  
  	//The initial text shown in the box will be 00:00:00 until something replaces it
  	
  daq.textbox[0].setDisplayText("00:00:00");

  timePointer = &(daq.timeBD); //timePointer is the address of the daq.timeBD structure
  								//It is required for the localtime() function later.

  daq.drawAll(); //Don't leave setup() without this step!
}

void loop() {
  char timeString[63]; //A C-style string buffer

  daq.tmStr = time(NULL);				//See any C language reference. This is part of time.h
  timePointer = localtime(&daq.tmStr);	//Same

	/* Long Note!
	
	If you haven't worked with C before, the following line of code is a lot to unpack.
	
	One of the fundamental functions of the Standard I/O library (stdio.h) is printf();
	printf() enables you to write your data to the screen in exactly the format you want. To do this, you
	should consult a reference of printf() to learn about the format specifiers. This will allow you to form
	strings to you exact needs in one line of code rather than building them up with Strings over many lines.
	
	Instead of showing a string to the screen, you can assign it to another string for later use using sprintf(),
	or writing it to a file using fprintf().
	
	One problem with sprintf() is that if your string is bigger than your buffer, you create a catastrophe called
	a buffer overflow. This crashes programs and allows hackers to run wild. A safer altenative is to use snprintf().
	It has an extra argument (the second one) that lets you limit the number of characters to transfer.
	
	Notice that there are three percent signs in the string that is the third parameter of the snprintf() function.
	These represent where the fourth, fifth and sixth parameters are displayed. The "d" indicates that the data is an
	integer, and the 2 means two digits must be displayed. The leading 0 means that for numbers 0-9, a leading zero
	must be added (00-09).
	
	After looking up the tm structure in time.h, you will realized that tm_hour, tm_min, and tm_sec represent the
	hour, minute and second values of time.
	
	*/
  snprintf(timeString, 62, "%02d:%02d:%02d", timePointer->tm_hour, timePointer->tm_min, timePointer->tm_sec);
  daq.textbox[0].setDisplayText(String(timeString));

  daq.updateDisplays(); //When the value in time string changes, the text box is redrawn
}
