/* ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  
4-I2C_device_with_Datalogging - Using an I2C device with datalogging to a flash drive on a GigaDAQ

This sketch only works with an Arduino GIGA R1 WiFi with an Arduino GIGA Display Shield.
GigaDAQ is the software to enable this hardware to be a stand-alone data acquisition system.

This example shows how to use an I2C device (in this case, a BMP280 sensor
that measures temperature and pressure) and record the data to a file.
Using a button as a toggle switch will also be demonstrated.

BMP280          GIGA R1
VCC ---(red)----- 3.3V
GND --(black) --- GND
SDA - (yellow) -- SDA (20)
SCL - (orange) -- SCL (21)

Created on Feb 9, 2025
by David A. Trevas

Notes:
1. To enable file writing, include all of the lines marked with FLASH in the comment.
2. Currently, a flash drive has to be formatted as FAT32 and using the MBR. 
See Arduino documentation for any updates.
3. This sketch contains separate interval trackers for input controls and outputs.
Most of the cycles emphasize the data collection

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

uint32_t currentTime, previousTimeUI, previousTimeDisp; 
const uint32_t UI_INTERVAL = 200; //Check buttons and sliders 5 times per second
const uint32_t DISPLAY_INTV = 100; //Update textboxes as needed 10 times a second

#include <BMP280-SOLDERED.h>  //Library required for BMP280 sensor
Soldered_BMP280 bmp280;					//Create a BMP280 object				

bool dataRecording = false;

void setup() {
  int i=0, err=0;             //FLASH
  bool msdConnected = false;  //FLASH

  daq.begin();

  pinMode(PA_15, OUTPUT);                 //FLASH
  digitalWrite(PA_15, HIGH);              //FLASH
  do{                                     //FLASH
    msdConnected = msd.connect();         //FLASH
    delay(1000);                          //FLASH
    i++;                                  //FLASH
  }while(msdConnected == false && i < 10);  //FLASH
  err = usb.mount(&msd);                    //FLASH
  
  daq.textbox[0] = Textbox("Time", 1, 1, 98, 12, BLACK, WHITE);
  daq.textbox[0].setDisplayText(String((float)millis()/1000.0,3)); 

  daq.textbox[1] = Textbox("Temperature", 1, 14, 98, 12, WHITE, GREEN);
  daq.textbox[1].setDisplayText("deg C"); 

  daq.textbox[2] = Textbox("Pressure", 1, 27, 98, 12, CYAN, BLACK);
  daq.textbox[2].setDisplayText("hPa"); 

  daq.button[0] = Button("Recording", 20, 65, 60, 20, WHITE, BLUE);
  daq.button[0].setDisplayText("Record data");
  daq.button[0].setAction(dataButton);
  
  daq.drawAll();	//Don't leave setup() without this step!
  
  if (!bmp280.begin()){
    while (1){
      delay(10);
    }
  }
  bmp280.setPresOversampling(OVERSAMPLING_X16);  //See BMP280-SOLDERED.h documentation
  bmp280.setIIRFilter(IIR_FILTER_16);
  bmp280.setTimeStandby(TIME_STANDBY_1000MS);
  bmp280.startNormalConversion();

  dataRecording = false;
  previousTimeUI = previousTimeDisp = millis(); 
}

void loop() {
  float temperature, pressure;

  currentTime = millis(); 
  
  if(currentTime - previousTimeUI >= UI_INTERVAL){   	 //Check for button presses 											
    daq.handleInputs();                                 //Interpret inputs and react if needed
    previousTimeUI = currentTime;  
  }

  if (bmp280.getTempPres(temperature, pressure)){     //Data collection
    if(daq.fp != NULL && dataRecording == true){
      fprintf(daq.fp, "%.3f, %.2f, %.2f\n", (float)millis()/1000., temperature, 100.0*pressure);
    }
    daq.textbox[0].setDisplayText(String((float)millis()/1000.0,3));
    daq.textbox[1].setDisplayText(String(temperature,2));
    daq.textbox[2].setDisplayText(String(100.0*pressure,2));
  }

  if(currentTime - previousTimeDisp >= DISPLAY_INTV){ //Check for changes in text box dispText
    daq.updateDisplays(); 
    previousTimeDisp = currentTime;
  }
}

void dataButton(void){
	/* Toggle behavior is accomplished by alternating the dispText property */
	
  if(daq.button[0].dispText.equals("Record data")){
    daq.startDataRecording("I2C_test.csv");			//FLASH
    daq.button[0].fgColor = YELLOW;
    daq.button[0].bgColor = RED;
    daq.button[0].setDisplayText("Stop recording");
    dataRecording = true;
    daq.drawButton(0);
  }
  else if(daq.button[0].dispText.equals("Stop recording")){
    daq.endDataRecording();								//FLASH
    daq.button[0].fgColor = WHITE;
    daq.button[0].bgColor = BLUE;
    daq.button[0].setDisplayText("Record data");
    dataRecording = false;
    daq.drawButton(0);
  }
}
