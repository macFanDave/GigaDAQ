/* ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  
2-Sliders - A demonstration of the sliders on a GigaDAQ

This sketch only works with an Arduino GIGA R1 WiFi with an Arduino GIGA Display Shield.
GigaDAQ is the software to enable this hardware to be a stand-alone data acquisition system.

This example shows how to create sliders and associate them with actions that change a text display.

Created on Feb 9, 2025
by David A. Trevas

Notes:
1. Color is a 16-bit number. The first five bits are the red values (0-31), the next six bits are
the green values (0-63) and the final 5 bits are the blue (0-31). The formula to calculate the decimal number
for color is 2048*Red + 32*Green + 1*Blue

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

uint16_t variableColor, blueValue = 16, greenValue = 32; //Color in 5-6-5 format
const uint16_t mediumRed = 32768; //Red contribution to RGB color will be constant

uint32_t currentTime, previousTime; 
const uint32_t UI_INTERVAL = 150;	
									
void setup() {
  daq.begin();

  variableColor = mediumRed + greenValue*32 + blueValue*1;

  daq.textbox[0] = Textbox("Color display", 3, 2, 71, 30, BLACK, variableColor);
  daq.textbox[0].setDisplayText("G31 x B16");  //Start by displaying the initial value of the counter.

/*
Create the first slider (number 0) with the name "Green vert".
The slider will start at 75% of the screen width from the left edge and 2% of the screen height 
from the top edge. The button with be 24% of the screen width and 88% of the screen height.
Foreground color green, background white 
*/
	
  daq.slider[0] = Slider("Green vert", 75, 2, 24, 88, GREEN, WHITE);
  daq.slider[0].setMode(VERTICAL);
  daq.slider[0].setYlimits(0.0, 63.0);
  daq.slider[0].setPosition(1.0, (float)greenValue);
  daq.slider[0].setAction(greenSlide);  //Assign the slider of a button press to the function described below

  //Create the second slider (number 1) with the name "Blue horiz".
  daq.slider[1] = Slider("Blue horiz", 2, 90, 96, 10, BLUE, MAGENTA);
  daq.slider[1].setMode(HORIZONTAL);
  daq.slider[1].setXlimits(0.0, 31.0);
  daq.slider[1].setPosition((float)blueValue, 1.0);
  daq.slider[1].setAction(blueSlide);

  daq.drawAll();	//Don't leave setup() without this step!
  
  previousTime = millis();
}

void loop() {
  currentTime = millis(); 
  
  if(currentTime - previousTime >= UI_INTERVAL){  											 
    daq.handleInputs();
    previousTime = currentTime;  //Set the start time to now to restart the count. Don't forget this step!
  }
  daq.updateDisplays();
}

void greenSlide(void){
  char textBuf[32];

  greenValue = (uint16_t)(daq.slider[0].posY);
  variableColor = mediumRed + greenValue*32 + blueValue*1;
  daq.textbox[0].bgColor = variableColor;

  snprintf(textBuf, 31, "B%d x G%d", blueValue, greenValue);
  daq.textbox[0].setDisplayText(String(textBuf));
}

void blueSlide(void){
  char textBuf[32];

  blueValue = (uint16_t)(daq.slider[1].posX);
  variableColor = mediumRed + greenValue*32 + blueValue*1;
  daq.textbox[0].bgColor = variableColor;

  snprintf(textBuf, 31, "B%d x G%d", blueValue, greenValue);
  daq.textbox[0].setDisplayText(String(textBuf));
}
