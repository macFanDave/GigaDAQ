/* ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  
1-Buttons - A demonstration of the buttons on a GigaDAQ

This sketch only works with an Arduino GIGA R1 WiFi with an Arduino GIGA Display Shield.
GigaDAQ is the software to enable this hardware to be a stand-alone data acquisition system.

This example shows how to create buttons and associate them with actions that change a text display.

Created on Feb 9, 2025
by David A. Trevas

Notes:
1. 

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

//GigaDAQ daq(PORTRAIT_USBDOWN);  //Tall, narrow appearance (iPhone-like)
GigaDAQ daq(LANDSCAPE_USBRIGHT);  //Wide, short appearance (TV-like)

int counter = 100;  //An initial value you will alter via the buttons.

uint32_t currentTime, previousTime; //Used to regulate how often functions get called.
const uint32_t UI_INTERVAL = 150;	//User interface interactions are often sufficient at 5-10 times per second
									//100-200 milliseconds is optimal for UI operations.

void setup() {
  daq.begin();

  daq.textbox[0] = Textbox("Time display", 5, 20, 90, 15, BLACK, WHITE);
  daq.textbox[0].setDisplayText(String(counter));  //Start by displaying the initial value of the counter.

	//Create the first button (number 0) with the name "Down button".
	//The text box will start at 5% of the screen width from the left edge and 40% of the screen height 
	//from the top edge. The button with be 40% of the screen width and 20% of the screen height.
	//The text color will be white and the background red.
	
  daq.button[0] = Button("Down button", 5, 40, 40, 20, WHITE, RED);
  daq.button[0].setDisplayText("Decrease Value");
  daq.button[0].setAction(buttonAction);  //Assign the action of a button press to the function described below

  daq.button[1] = Button("Up button", 55, 40, 40, 20, BLACK, GREEN);
  daq.button[1].setDisplayText("Increase Value");
  daq.button[1].setAction(buttonAction);

  daq.drawAll();	//Don't leave setup() without this step!
  
  previousTime = millis(); //Set the start time to use for finding a time interval.
}

void loop() {
  currentTime = millis();  //Get the time now.
  
  if(currentTime - previousTime >= UI_INTERVAL){  //If the difference between now and the start exceeds the given
  												 //interval, enter the block, otherwise keep looping
    daq.handleInputs();
    previousTime = currentTime;  //Set the start time to now to restart the count. Don't forget this step!
  }
  daq.updateDisplays(); //If the counter value has changed, redraw the text box
}

/* It would have been possible (and maybe easier) to write separate functions for up and down.
In this case, we use the fact that a button up is when the currentEvent is a NOTHING and the previousEvent
was linked to a BUTTON with a name.*/

void buttonAction(void){
  if(daq.previousEvent.name.equals("Down button")){
    counter--;
    daq.textbox[0].setDisplayText(String(counter));
  }
  else if(daq.previousEvent.name.equals("Up button")){
    counter++;
    daq.textbox[0].setDisplayText(String(counter));
  }
}
