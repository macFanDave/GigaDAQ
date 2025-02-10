/* ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  
3-Trackpad_with_Datalogging - A demonstration of the trackpad and datalogging to a flash drive on a GigaDAQ

This sketch only works with an Arduino GIGA R1 WiFi with an Arduino GIGA Display Shield.
GigaDAQ is the software to enable this hardware to be a stand-alone data acquisition system.

This example shows how to create a trackpad and associate it with actions that are recorded to a file.
Using a button as a toggle switch will also be demonstrated.

Created on Feb 9, 2025
by David A. Trevas

Notes:
1. To enable file writing, include all of the lines marked with FLASH in the comment.
2. Currently, a flash drive has to be formatted as FAT32 and using the MBR. See Arduino documentation
for any updates.
3. I discovered that a 2D trackpad would crash if it is wider than 60%. For now, treat difficulties with
trackpads by reducing their size.

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
#include <Arduino_USBHostMbed5.h> 	//FLASH
#include <DigitalOut.h>           	//FLASH
#include <FATFileSystem.h>  		//FLASH

USBHostMSD msd;                   //FLASH
mbed::FATFileSystem usb("usb");   //FLASH

//One of the following two lines must be commented out and the other un-commented.

//GigaDAQ daq(PORTRAIT_USBDOWN);  //Tall, narrow appearance (iPhone-like)
GigaDAQ daq(LANDSCAPE_USBRIGHT);  //Wide, short appearance (TV-like)

uint32_t currentTime, previousTime; 
const uint32_t UI_INTERVAL = 150;
									

int numDatapoints = 0;
float xValue, yValue;

void setup() {
  int i=0, err=0;             //FLASH
  bool msdConnected = false;  //FLASH

  daq.begin();

  pinMode(PA_15, OUTPUT);           //For recording data on FLASH drive
  digitalWrite(PA_15, HIGH);        //For recording data on FLASH drive
  do{                                     //FLASH
    msdConnected = msd.connect();         //FLASH
    delay(1000);                          //FLASH
    i++;                                  //FLASH
  }while(msdConnected == false && i < 10);  //FLASH
  err = usb.mount(&msd);                    //FLASH
  
  daq.button[0] = Button("Recording", 20, 65, 60, 20, WHITE, BLUE);
  daq.button[0].setDisplayText("Record data");
  daq.button[0].setAction(dataButton);
  
  daq.textbox[0] = Textbox("Data count", 20, 90, 60, 8, BLACK, WHITE);
  daq.textbox[0].setDisplayText(String(numDatapoints)); 
	
  daq.slider[0] = Slider("Track Pad", 20, 1, 60, 60, YELLOW, GREEN);
  daq.slider[0].setMode(TRACKPAD);
  daq.slider[0].setXlimits(500.0, 750.0);
  daq.slider[0].setYlimits(-1500.0, 4000.0);
  daq.slider[0].setPosition(600.0, -500.0);
  daq.slider[0].setAction(trackSlide);  //Assign the slider motion to the function described below

  daq.drawAll();	//Don't leave setup() without this step!
  
  previousTime = millis(); 
}

void loop() {
  currentTime = millis(); 
  
  if(currentTime - previousTime >= UI_INTERVAL){   												
    daq.handleInputs();
    previousTime = currentTime;  
  }
  daq.updateDisplays(); 
}

void trackSlide(void){
  String outStr = "";

  xValue = daq.slider[0].posX;
  yValue = daq.slider[0].posY;
  numDatapoints++;
  outStr += String(numDatapoints);
  daq.textbox[0].setDisplayText(outStr);
  if(daq.fp != NULL){ //Ensure that the file pointer, fp, is not NULL. Don't try fprintf() with a NULL fp.
  	//fprintf() works similiarly to printf() and snprintf(). Any reference on stdio.h will explain its use.
  	
    fprintf(daq.fp, "%.3f, %.1f, %.1f\n", (float)millis()/1000.0, xValue, yValue);  //FLASH
  }
  
}

void dataButton(void){
	/* Toggle behavior is accomplished by alternating the dispText property */
	
  if(daq.button[0].dispText.equals("Record data")){
    daq.startDataRecording("TrackpadDemo2.csv");			//FLASH
    daq.button[0].fgColor = YELLOW;
    daq.button[0].bgColor = RED;
    daq.button[0].setDisplayText("Stop recording");
    daq.drawButton(0);
  }
  else if(daq.button[0].dispText.equals("Stop recording")){
    daq.endDataRecording();								//FLASH
    daq.button[0].fgColor = WHITE;
    daq.button[0].bgColor = BLUE;
    daq.button[0].setDisplayText("Record data");
    daq.drawButton(0);
  }
}
