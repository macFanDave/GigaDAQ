/* ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  
5-Set_Realtime_Clock - Set the built-in real-time clock (RTC) on the Arduino GIGA R1 WiFi

This sketch only works with an Arduino GIGA R1 WiFi with an Arduino GIGA Display Shield.
GigaDAQ is the software to enable this hardware to be a stand-alone data acquisition system.

Create a user interface to set the RTC interactively.

Created on Feb 14, 2025
by David A. Trevas

Notes:
1. For more information on the time functions mktime() and localtime(),
see any reference of the time.h library that is part of the C standard.


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

GigaDAQ daq(PORTRAIT_USBDOWN);
uint32_t currTime, prevTimeUI, prevTimeTB;

const uint32_t UI_INTV = 100;
const uint32_t TB_INTV = 50;

//Colors using the 5-6-5 RGB format
const uint16_t LIGHT_PINK = 0xfbef;
const uint16_t PALE_GRAY = 0xe71c;

//Constants needed for the timeStatus bit logic operations
const uint16_t YEARS = 1 << 5;
const uint16_t MONTHS = 1 << 4;
const uint16_t DAYS = 1 << 3;
const uint16_t HOURS = 1 << 2;
const uint16_t MINUTES = 1 << 1;
const uint16_t SECONDS = 1 << 0;

uint16_t timeStatus = 0; //Binary 00111111 = Decimal 63
bool timeSet = false;

void setup() {
  daq.begin();

  daq.textbox[0] = Textbox("Time Window", 0, 0, 100, 10, BLACK, WHITE);  //Main display
  daq.textbox[0].setDisplayText("Enter year");

	//Column of six status indicators
	//RED and LIGHT_PINK indicate value is not set. 
	//GREEN indicates that a value is set.
	
  daq.textbox[1] = Textbox("Year status", 5, 25, 25, 5, RED, WHITE);
  daq.textbox[1].setDisplayText("YEAR");

  daq.textbox[2] = Textbox("Month status", 5, 30, 25, 5, LIGHT_PINK, PALE_GRAY);
  daq.textbox[2].setDisplayText("MONTH");

  daq.textbox[3] = Textbox("Day status", 5, 35, 25, 5, LIGHT_PINK, PALE_GRAY);
  daq.textbox[3].setDisplayText("DAY");

  daq.textbox[4] = Textbox("Hour status", 5, 40, 25, 5, LIGHT_PINK, PALE_GRAY);
  daq.textbox[4].setDisplayText("HOUR");

  daq.textbox[5] = Textbox("Minute status", 5, 45, 25, 5, LIGHT_PINK, PALE_GRAY);
  daq.textbox[5].setDisplayText("MINUTE");

  daq.textbox[6] = Textbox("Second status", 5, 50, 25, 5, LIGHT_PINK, PALE_GRAY);
  daq.textbox[6].setDisplayText("SECOND");

	//Slider is used to select numerical values
	
  daq.slider[0] = Slider("Setter", 3, 12, 94, 10, CYAN, BLUE);
  daq.slider[0].setMode(HORIZONTAL);
  daq.slider[0].setXlimits(2024.0, 2038.5);
  daq.slider[0].setPosition(2028.0, 1.0);
  daq.slider[0].setAction(sliderMove);

	//Button to enter the data to the time data structure
	
  daq.button[0] = Button("Enter", 50, 25, 40, 15, WHITE, GREEN);
  daq.button[0].setDisplayText("Enter");
  daq.button[0].setAction(enterButton);

	//Once all values are added to time data structure, the clock can be set
	
  daq.button[1] = Button("Set RTC", 30, 70, 40, 24, BLACK, PALE_GRAY);
  daq.button[1].setDisplayText("Set Time");
  daq.button[1].setAction(timeSetButton);
  
  daq.drawAll();

  prevTimeUI = prevTimeTB = millis();
}

void loop() {
  currTime = millis();

  if(currTime - prevTimeUI >= UI_INTV){
    daq.handleInputs();
    prevTimeUI = currTime;
  }

  if(currTime - prevTimeTB >= TB_INTV){
    if(timeStatus == 63 && timeSet == true){
      tm *tPtr;
      daq.tmStr = time(NULL);			//Print current time
      tPtr = localtime(&(daq.tmStr));
      daq.textbox[0].setDisplayText(String(asctime(tPtr)));
    }
    daq.updateDisplays();
    prevTimeTB = currTime;
  }
}

void sliderMove(void){
  int currField, sVal;

  currField = redTextbox(); //The RED component is active, not LIGHT_PINK or GREEN

  switch(currField){
    case YEARS:
      sVal = (int)(daq.slider[0].posX);
      daq.textbox[0].setDisplayText(String(sVal));
      break;
    case MONTHS:
      sVal = (int)(daq.slider[0].posX);
      daq.textbox[0].setDisplayText(String(sVal));
      break;
    case DAYS:
      sVal = (int)(daq.slider[0].posX);
      daq.textbox[0].setDisplayText(String(sVal));
      break;
    case HOURS:
      sVal = (int)(daq.slider[0].posX);
      daq.textbox[0].setDisplayText(String(sVal));
      break;
    case MINUTES:
      sVal = (int)(daq.slider[0].posX);
      daq.textbox[0].setDisplayText(String(sVal));
      break;
    case SECONDS:
      sVal = (int)(daq.slider[0].posX);
      daq.textbox[0].setDisplayText(String(sVal));
      break;
  }
}

void enterButton(void){
  int currField;

  currField = redTextbox();

	//Each button press records the value and sets focus to the next component
	//The prompt in the text box is updated
	
  switch(currField){
    case YEARS:
      daq.timeBD.tm_year = daq.textbox[0].dispText.toInt() - 1900;
      daq.textbox[1].fgColor = GREEN;
      daq.textbox[2].fgColor = RED; daq.textbox[2].bgColor = WHITE;
      timeStatus = timeStatus | YEARS;
      daq.textbox[0].setDisplayText("Enter months");
      daq.slider[0].setXlimits(1.0, 12.5);
      daq.slider[0].setPosition(5.0, 1.0);
      break;
    case MONTHS:
      daq.timeBD.tm_mon = daq.textbox[0].dispText.toInt() - 1;
      daq.textbox[2].fgColor = GREEN;
      daq.textbox[3].fgColor = RED; daq.textbox[3].bgColor = WHITE;
      timeStatus = timeStatus | MONTHS;
      daq.textbox[0].setDisplayText("Enter days");
      daq.slider[0].setXlimits(1.0, 31.5);
      daq.slider[0].setPosition(15.0, 1.0);
      break;
    case DAYS:
      daq.timeBD.tm_mday = daq.textbox[0].dispText.toInt();
      daq.textbox[3].fgColor = GREEN;
      daq.textbox[4].fgColor = RED; daq.textbox[4].bgColor = WHITE;
      timeStatus = timeStatus | DAYS;
      daq.textbox[0].setDisplayText("Enter hours");
      daq.slider[0].setXlimits(0.0, 23.5);
      daq.slider[0].setPosition(11.0, 1.0);
      break;
    case HOURS:
      daq.timeBD.tm_hour = daq.textbox[0].dispText.toInt();
      daq.textbox[4].fgColor = GREEN;
      daq.textbox[5].fgColor = RED; daq.textbox[5].bgColor = WHITE;
      timeStatus = timeStatus | HOURS;
      daq.textbox[0].setDisplayText("Enter minutes");
      daq.slider[0].setXlimits(0.0, 59.5);
      daq.slider[0].setPosition(30.0, 1.0);
      break;
    case MINUTES:
      daq.timeBD.tm_min = daq.textbox[0].dispText.toInt();
      daq.textbox[5].fgColor = GREEN;
      daq.textbox[6].fgColor = RED; daq.textbox[6].bgColor = WHITE;
      timeStatus = timeStatus | MINUTES;
      daq.textbox[0].setDisplayText("Enter seconds");
      daq.slider[0].setXlimits(0.0, 59.5);
      daq.slider[0].setPosition(30.0, 1.0);
      break;
    case SECONDS:
      daq.timeBD.tm_sec = daq.textbox[0].dispText.toInt();
      daq.textbox[6].fgColor = GREEN;
      timeStatus = timeStatus | SECONDS;
      daq.button[1].bgColor = WHITE;
      daq.textbox[0].setDisplayText("Press Set Time");
      break;
  }
  daq.drawSlider(0);
}
int redTextbox(void){
  int i = 1, retVal=0;
  bool matchFound = false;

  do{
    if(daq.textbox[i].fgColor == RED){
      matchFound = true;
      switch(i){
        case 1:
          retVal = YEARS;
          break;
        case 2:
          retVal = MONTHS;
          break;
        case 3:
          retVal = DAYS;
          break;
        case 4:
          retVal = HOURS;
          break;
        case 5:
          retVal = MINUTES;
          break;
        case 6:
          retVal = SECONDS;
          break;
      }
    }
    i++;
  }while(i < 7 && matchFound == false);

  return retVal;
}

void timeSetButton(void){

  if(timeStatus == 63 && daq.button[1].bgColor == WHITE){
    daq.tmStr = mktime(&(daq.timeBD));
    set_time(daq.tmStr);
    timeSet = true;
    daq.button[1].bgColor = PALE_GRAY;
    daq.drawButton(1);
  }
}
