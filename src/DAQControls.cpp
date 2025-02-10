/**

@file

@section intro_sec Introduction

This is where the specific controls for the GigaDAQ project are detailed. This software is specifically for use with the Arduino GIGA R1 WiFi with the Arduino GIGA Display Shield, and allows you to make a standalone data-acquisition system (DAQ) that is interactive and can record data to a flash drive.

Each control has different instructions for how it is drawn and how it reacts to user input. 

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

#include "DAQControls.h"


Button::Button(){
    type = BUTTON;
    name = "";
    w = 0;
    h = 0;
    this->buttonUp = nullptr;
}
Button::Button(String nm, unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint16_t c1,  uint16_t c2){
    name = nm;
    type = BUTTON;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    fgColor = c1;
    bgColor = c2;
    this->buttonUp = nullptr;
}
void Button::setAction(void (*du)()){
	this->buttonUp = du;
}
void Button::release(){
	if(buttonUp != nullptr){
		(*buttonUp)();
	}
}

Slider::Slider(){
    type = SLIDER;
    name = "";
    w = 0;
    h = 0;
    mode = HORIZONTAL;
    minX = 0.0;
    maxX = 1.0;
    minY = 0.0;
    maxY = 1.0;
    posX = 0.5;
    posY = 0.5;
    this->slide = nullptr;
}
Slider::Slider(String nm, unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint16_t c1,  uint16_t c2){
    name = nm;
    type = SLIDER;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    fgColor = c1;
    bgColor = c2;
    mode = HORIZONTAL;
    minX = 0.0;
    maxX = 1.0;
    minY = 0.0;
    maxY = 1.0;
    posX = 0.5;
    posY = 0.5;
    this->slide = nullptr;
}
void Slider::setXlimits(float minimumX, float maximumX){
    minX = minimumX;
    maxX = maximumX;
}
void Slider::setYlimits(float minimumY, float maximumY){
    minY = minimumY;
    maxY = maximumY;
}
void Slider::setPosition(float px, float py){
	posX = px;
	posY = py;
}
void Slider::setMode(SliderMode md){
	mode = md;
}
void Slider::setAction(void(*sf)()){
	this->slide = sf;
}
void Slider::sliderMotion(){
	if(slide != nullptr){
		(*slide)();
	}
}
    
Textbox::Textbox(){
    type = TEXTBOX;
    name = "";
    w = 0;
    h = 0;
}
Textbox::Textbox(String nm, unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint16_t c1,  uint16_t c2){
    name = nm;
    type = TEXTBOX;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    fgColor = c1;
    bgColor = c2;
}
