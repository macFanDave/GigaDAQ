/**

@file

@mainpage GigaDAQ - Data-acquisition system using Arduino GIGA R1 WiFi with Display Shield

@section intro_sec Introduction

This documents the main object of the GigaDAQ project. This software is specifically for use with the Arduino GIGA R1 WiFi with the Arduino GIGA Display Shield, and allows you to make a standalone data-acquisition system (DAQ) that is interactive and can record data to a flash drive. 

@note One design decision that is subject to change is that a GigaDAQ object contains static arrays of Button, Slider and Textbox objects. With unused objects being small, the complexity of enabling dynamic allocation doesn't seem to justify the effort. This may change in future versions.

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

#include "GigaDAQ.h"
      
GigaDAQ::GigaDAQ(){
    screenW = GIGA_DS_WIDTH;
    screenH = GIGA_DS_HEIGHT;
    rotation = PORTRAIT_USBDOWN;
    
}
GigaDAQ::GigaDAQ(DisplayOrientation rotation){
    this->rotation = rotation;
    if(rotation == PORTRAIT_USBDOWN || rotation == PORTRAIT_USBUP){
        screenW = GIGA_DS_WIDTH;
        screenH = GIGA_DS_HEIGHT;
    }
    else{
        screenW = GIGA_DS_HEIGHT;
        screenH = GIGA_DS_WIDTH;
    }
}

void GigaDAQ::begin(void){
	tm *timePtr;
	
	graph.begin();
	touch.begin();
	graph.setRotation(rotation);
	
	tmStr = time(NULL);				//From the standard C time.h library
	timePtr = localtime(&tmStr);
	
	//The year is the number of years since 1900. If the return value is 2025-2030, a "reasonable" value is assumed.
	if(timePtr->tm_year >= 125 && timePtr->tm_year <= 130){
		timeBD = *timePtr;
	}
	else{ //Set clock to an arbitrary value if the RTC is not set. Here, we choose 13:26:47 on July 22, 2025
		timeBD.tm_year = 125;
		timeBD.tm_mon = 6;
		timeBD.tm_mday = 22;
		timeBD.tm_hour = 13;
		timeBD.tm_min = 26;
		timeBD.tm_sec = 47;
		set_time(mktime(&timeBD));
		tmStr = mktime(&timeBD);
	}
}
//
// The method behind drawing all of the controls is to draw to a buffer in memory called the "canvas" first and when
// the canvas is completed, transfer it to the screen as a bitmap in a single function.
//
void GigaDAQ::drawButton(int num){
	int cw, ch, cx, cy;
	int fontNo;
	uint16_t bboxw, bboxh;
	MonoBoundingBox mbb;
	
	cw = button[num].w * screenW / 100; //Convert percentages to pixels
	ch = button[num].h * screenH / 100;
	
	if(cw > 0 && ch > 0){					//Only attempt this if the button has non-zero width and height
		GFXcanvas16 canvas(cw, ch);
		cx = button[num].x * screenW / 100;
		cy = button[num].y * screenH / 100;
		
		canvas.fillScreen(0x0000);
		canvas.fillRect(0, 0, cw, ch, button[num].bgColor);
		canvas.setTextWrap(false);
		canvas.setTextColor(button[num].fgColor);
		mbb = maxFont(button[num].dispText, cw, ch); //Find the largest font that will fit in the button
		
		switch(mbb.fontSize){
			case MONO9PT:
				canvas.setFont(&FreeMonoBold9pt7b);
				break;
			case MONO12PT:
				canvas.setFont(&FreeMonoBold12pt7b);
				break;
			case MONO18PT:
				canvas.setFont(&FreeMonoBold18pt7b);
				break;
			case MONO24PT:
				canvas.setFont(&FreeMonoBold24pt7b);
				break;
			default:
				canvas.setFont(&FreeMonoBold9pt7b);
				break;
		}
		canvas.setCursor((cw-mbb.w)/2, ch - (ch-mbb.h)/2);  //Center the text within the button
		canvas.print(button[num].dispText);
		graph.drawRGBBitmap(cx, cy, canvas.getBuffer(), cw, ch);
		button[num].prevDispText = button[num].dispText;
	}
}
void GigaDAQ::drawSlider(int num){  //see drawButton() method for ideas that are similar
	int cw, ch, cx, cy, smx, smy;
	float fracx, fracy;
	
	cw = slider[num].w * screenW / 100;
	ch = slider[num].h * screenH / 100;
	
	if(cw > 0 && ch > 0){
		GFXcanvas16 canvas(cw, ch);
		cx = slider[num].x * screenW / 100;
		cy = slider[num].y * screenH / 100;
		
		canvas.fillScreen(slider[num].bgColor);
		canvas.drawRect(0, 0, cw, ch, slider[num].fgColor);
		
		if(slider[num].mode == VERTICAL){
			fracx = 1.0;
		}
		else{
			fracx = (slider[num].posX - slider[num].minX)/(slider[num].maxX - slider[num].minX);
		}
		smx = (int)(fracx * cw);
		
		if(slider[num].mode == HORIZONTAL){
			fracy = 1.0;
		}
		else{
			fracy = (slider[num].posY - slider[num].minY)/(slider[num].maxY - slider[num].minY);
		}
		smy = (int)(fracy * ch);
		
		canvas.fillRect(0, ch-smy, smx, smy, slider[num].fgColor);
		
		
		graph.drawRGBBitmap(cx, cy, canvas.getBuffer(), cw, ch);
	}
}
void GigaDAQ::drawTextbox(int num){ //see drawButton() method for ideas that are similar
	int cw, ch, cx, cy, fontNo;
	uint16_t bboxw, bboxh;
	MonoBoundingBox mbb;
	
	cw = textbox[num].w * screenW / 100;
	ch = textbox[num].h * screenH / 100;
	
	if(cw > 0 && ch > 0){
		GFXcanvas16 canvas(cw, ch);
		cx = textbox[num].x * screenW / 100;
		cy = textbox[num].y * screenH / 100;
		
		canvas.fillScreen(0x0000);
		canvas.fillRect(0, 0, cw, ch, textbox[num].bgColor);
		canvas.setTextWrap(false);
		canvas.setTextColor(textbox[num].fgColor);
		
		mbb = maxFont(textbox[num].dispText, cw, ch);
		
		switch(mbb.fontSize){
			case MONO9PT:
				canvas.setFont(&FreeMonoBold9pt7b);
				break;
			case MONO12PT:
				canvas.setFont(&FreeMonoBold12pt7b);
				break;
			case MONO18PT:
				canvas.setFont(&FreeMonoBold18pt7b);
				break;
			case MONO24PT:
				canvas.setFont(&FreeMonoBold24pt7b);
				break;
			default:
				canvas.setFont(&FreeMonoBold9pt7b);
				break;
		}
		canvas.setCursor((cw-mbb.w)/2, ch - (ch-mbb.h)/2);
		
		canvas.print(textbox[num].dispText);
		graph.drawRGBBitmap(cx, cy, canvas.getBuffer(), cw, ch);
		textbox[num].prevDispText = textbox[num].dispText;
	}
}
void GigaDAQ::drawAll(){
    int i;
    
    graph.fillScreen(0x0000);
    
    for(i = 0; i < NUM_BUTTONS; i++){
        if(button[i].w > 0 && button[i].h > 0){
        	drawButton(i);
        }
    }
    
    for(i = 0; i < NUM_SLIDERS; i++){
        if(slider[i].w > 0 && slider[i].h > 0){
            drawSlider(i);
        }
    }
    
    for(i = 0; i < NUM_TEXTBOXES; i++){
        if(textbox[i].w > 0 && textbox[i].h > 0){
            drawTextbox(i);
        }
    }
}
void GigaDAQ::locate(int touchX, int touchY){
    unsigned px=0, py=0, cx, cy, cw, ch;
    float fracx, fracy, slidx, slidy;
    
    int i;
    bool matchFound = false;
    
    switch(rotation){				//Convert pixels to percentages. Touch screen values do not vary with rotation.
        case PORTRAIT_USBDOWN:
            px = touchX * 100 / screenW;
            py = touchY * 100 / screenH;
            break;
        case LANDSCAPE_USBRIGHT:
            px = touchY * 100 / screenW;
            py = (screenH - touchX) * 100 / screenH;
            break;
        case PORTRAIT_USBUP:
            px = (screenW - touchX) * 100 / screenW;
            py = (screenH - touchY) * 100 / screenH;
            break;
        case LANDSCAPE_USBLEFT:
            px = (screenW - touchY) * 100 / screenW;
            py = touchX * 100 / screenH;
            break;
    }
    
    //Once the touch point is translated to percentages, find out which control, if any, contains the point.
    i = 0;
    
    do{	//Cycle though buttons to see if touch point is within a button.
        cw = button[i].w;
        ch = button[i].h;
        if(cw > 0 && ch > 0){
            cx = button[i].x;
            cy = button[i].y;
            if(cx < px && px <= cx+cw && cy < py && py <= cy+ch){
                matchFound = true;
                currentEvent.type = BUTTON;
                currentEvent.name = button[i].name;
                currentEvent.x = px;
                currentEvent.y = py;
                currentEvent.t = millis();
            }
        }
        i++;
    }while(i < NUM_BUTTONS && matchFound == false);
    
    if(matchFound == false){//Cycle though sliders to see if touch point is within a slider.
        i = 0;
        do{
            cw = slider[i].w;
            ch = slider[i].h;
            if(cw > 0 && ch > 0){
                cx = slider[i].x;
                cy = slider[i].y;
                if(cx < px && px <= cx+cw && cy < py && py <= cy+ch){
                    matchFound = true;
                    currentEvent.type = SLIDER;
                    currentEvent.name = slider[i].name;
                    currentEvent.x = px;
                    currentEvent.y = py;
                    currentEvent.t = millis();
                    fracx = (float)(px - cx)/(float)cw;
                    slidx = slider[i].minX + fracx*(slider[i].maxX - slider[i].minX);
                    fracy = (float)((cy+ch)-py)/(float)ch;
                    slidy = slider[i].minY + fracy*(slider[i].maxY - slider[i].minY);
                    slider[i].posX = slidx;
                    slider[i].posY = slidy;
                    drawSlider(i);
                }
            }
            i++;
        }while(i < NUM_SLIDERS && matchFound == false);
    }
    //No need to cycle though text boxes because they are output controls with no touch actions.
    
    //If finger is anywhere other than on a button or slider (the input controls), register a null event
    
    if(matchFound == false){
        currentEvent.type = NOTHING;
        currentEvent.name = "";
        currentEvent.x = 0;
        currentEvent.y = 0;
        currentEvent.t = 0;
    }
    return;
}
int GigaDAQ::arrayPosition(ControlType type, String name){
    int i, val=-1;
    bool matchFound = false;
    
    if(type == BUTTON){
        i = 0;
        do{
            if(button[i].name.equals(name)){
                matchFound = true;
                val = i;
            }
            i++;
        } while(i < NUM_BUTTONS && matchFound == false);
    }
    else if(type == SLIDER){
        i = 0;
        do{
            if(slider[i].name.equals(name)){
                matchFound = true;
                val = i;
            }
            i++;
        } while(i < NUM_SLIDERS && matchFound == false);
    }
    else if(type == TEXTBOX){
        i = 0;
        do{
            if(textbox[i].name.equals(name)){
                matchFound = true;
                val = i;
            }
            i++;
        } while(i < NUM_TEXTBOXES && matchFound == false);
    }
    
    return val;
}
void GigaDAQ::takeAction(void){
    int num;
    float fracx, fracy, slidx, slidy;
    
    //Button action when finger lifts from button
    if(previousEvent.type == BUTTON && currentEvent.type == NOTHING){
        num = arrayPosition(BUTTON, previousEvent.name);
        if(num >= 0){
            button[num].release();
        }
    }
    
    //Slider responds if finger stays in slider
    
    if(previousEvent.type == SLIDER && currentEvent.type == SLIDER && previousEvent.name == currentEvent.name){
        num = arrayPosition(SLIDER, currentEvent.name);
        if(num >= 0){
            slider[num].sliderMotion();
        }
    }
    
    //Other actions will go here
}
void GigaDAQ::handleInputs(void){
	uint8_t contacts;
	GDTpoint_t points[5];
	int tpx, tpy;
	
	contacts = touch.getTouchPoints(points);
	
	if(contacts > 0){  //If multiple fingers are used, only the first one is considered. 
						//Do not use more than one finger.
		tpx = points[0].x;
		tpy = points[0].y;
	}
	else{
		tpx = 0;
		tpy = 0;
	}
	locate(tpx, tpy);
	takeAction();
	previousEvent = currentEvent;
	
	
}
void GigaDAQ::updateDisplays(void){
	int i;
	for(i=0; i<NUM_TEXTBOXES; i++){
		if(textbox[i].w > 0 && textbox[i].h > 0 && textbox[i].dispText.equals(textbox[i].prevDispText) == false){
			drawTextbox(i);
			textbox[i].prevDispText = textbox[i].dispText;
		}
	}
}

void GigaDAQ::startDataRecording(String fileName){
	int i;
	const char *strConv;
  	char fBuf[256];
  	
  	strConv = fileName.c_str();
  	snprintf(fBuf, 255, "/usb/%s", strConv);
  	fp = fopen(fBuf, "at"); 	
}
void GigaDAQ::endDataRecording(){
	if(fp) fclose(fp);

}