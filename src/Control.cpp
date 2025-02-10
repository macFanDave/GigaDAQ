/**

@file

@section intro_sec Introduction

This contains some of the building blocks and utilities for the GigaDAQ project. This software is specifically for use with the Arduino GIGA R1 WiFi with the Arduino GIGA Display Shield, and allows you to make a standalone data-acquisition system (DAQ) that is interactive and can record data to a flash drive. 

@section dependencies Dependencies

Hardware:
Arduino GIGA R1 WiFi and Arduino GIGA Display Shield

Libraries:
Arduino_GigaDisplay_GFX.h and Arduino_GigaDisplayTouch.h

@section author Author

Written by David A. Trevas

@section license License

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

*/

#include "Control.h"

Control::Control(){}
void Control::setDisplayText(String txt){
    prevDispText = dispText; 	//Place existing dispText string into previous
    							//Difference between the two indicates change
    							//that requires a redraw.	
    dispText = txt;
}

Event::Event(){
    type = NOTHING;
    name = "";
    x = 0;
    y = 0;
    t = 0;
}

MonoBoundingBox maxFont(String dString, unsigned int containerWidth, unsigned int containerHeight){
	MonoBoundingBox temp;
	
	int i, w, h, numChar;
	bool matchFound = false;
	
	numChar = dString.length();
	
	i = 4;			//Start with the largest font first and try smaller when needed.
	
	do{
		switch(i){
			case MONO9PT:
				w = 11*numChar;
				h = 15;
				break;
			case MONO12PT:
				w = 14*numChar;
				h = 20;
				break;
			case MONO18PT:
				w = 21*numChar;
				h = 30;
				break;
			case MONO24PT:
				w = 28*numChar;
				h = 40;
				break;
			default:
				w = 11*numChar;
				h = 15;
				break;
		}
		//It was found that a small pad between the string and container is required.
		//1.5 pixels per side seems to work.
		
		if(w < containerWidth - 3 && h < containerHeight - 3){
		
			switch(i){
				case 1:
					temp.fontSize = MONO9PT;
					break;
				case 2:
					temp.fontSize = MONO12PT;
					break;
				case 3:
					temp.fontSize = MONO18PT;
					break;
				case 4:
					temp.fontSize = MONO24PT;
					break;
			}
			temp.w = w;
			temp.h = h;
			matchFound = true;
		}
		i--;
	} while(i >= 1 && matchFound == false);
	
	//Even if no match is found when all sizes are attempted, the smallest size is returned
	//This will result in text getting cut off. At this point, the user ought to use a bigger 
	//container or send less text to be displayed.
	if(matchFound == false){
		temp.fontSize = MONO9PT;
		temp.w = 11*numChar;
		temp.h = 15;
	}
	
	return temp;	
}