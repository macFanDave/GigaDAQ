/**

@file

This is where the specific controls for the GigaDAQ project are detailed. Each one has different instructions for how they are drawn and how they react to user input. This software is specifically for use with the Arduino GIGA R1 WiFi with the Arduino GIGA Display Shield, and allows you to make a standalone data-acquisition system (DAQ) that is interactive and can record data to a flash drive. 

Written by David A. Trevas

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

#ifndef _DAQ_CONTROLS_INCLUDE_
#define _DAQ_CONTROLS_INCLUDE_

#include <stdio.h>
#include "Control.h"


/** Slider modes */
enum SliderMode {
	HORIZONTAL,		/**< Horizontal slider, only x-value */
	VERTICAL,		/**< Vertical slider, only y-value */
	TRACKPAD		/**< Two-dimensional trackpad, x- and y-values available */
};

/**
@brief Button class can be used for buttons and toggle switches.
*/
class Button : public Control {
public:
    void (*buttonUp)(void); ///< Function to execute once finger leaves button
    /** Default constructor of a Button object. Initializes with safe values */
    Button();
    /**
    Constructor with user-defined values
    
    @param nm String that is a unique identifier
    @param x Left position of control as a percentage of screen width
    @param y Top position of control as a percentage of screen height
    @param w Width of control as a percentage of screen width
    @param h Height of control as a percentage of screen height
    @param c1 Foreground color (text color) in 5-6-5 format
    @param c2 Background color in 5-6-5 format
    */
    Button(String nm, unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint16_t c1,  uint16_t c2);
    /**
    @brief Sets the UI action which happens when a button is released.
    
    @param du A function pointer where the function must be of the form void f(void)
    */
    void setAction(void (*du)());
    /**
    @brief Execute the action set as the buttonUp action.
    */
    void release();
};

/**
@brief Slider class can be used for sliders and 2D trackpads.
*/
class Slider : public Control {
public:
	SliderMode mode; ///< Use a one-dimensional x- or y-slider or a two-D trackpad
    float minX;		///< Scaled value at left edge of slider (float)
    float maxX;		///< Scaled value at right edge of slider (float)
    float minY;		///< Scaled value at bottom edge of slider (float)
    float maxY;		///< Scaled value at top edge of slider (float)
    float posX;		///< Scaled x-value at current touch point (float)
    float posY;		///< Scaled y-value at current touch point (float)
    void (*slide)(); ///< Action taken after two consecutive events take place within a slider
    /** Default constructor of a Slider object. Initializes with safe values */
    Slider();
    /**
    Constructor with user-defined values
    
    @param nm String that is a unique identifier
    @param x Left position of control as a percentage of screen width
    @param y Top position of control as a percentage of screen height
    @param w Width of control as a percentage of screen width
    @param h Height of control as a percentage of screen height
    @param c1 Foreground color (text color) in 5-6-5 format
    @param c2 Background color in 5-6-5 format
    */
    Slider(String nm, unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint16_t c1,  uint16_t c2);
    /**
    @brief Sets the minimum and maximum values of the slider in the horizontal direction.
    
    @param minimumX floating-point value of left edge of slider.
    @param maximumX floating-point value of right edge of slider.
    */
    void setXlimits(float minimumX, float maximumX);
    /**
    @brief Sets the minimum and maximum values of the slider in the vertical direction.
    
    @param minimumY floating-point value of bottom edge of slider.
    @param maximumY floating-point value of top edge of slider.
    */
    void setYlimits(float minimumY, float maximumY);
    /**
    @brief Sets the current values of the display point of the slider.
    
    @param px floating-point value of x-point of slider.
    @param py floating-point value of y-point of slider.
    */
    void setPosition(float px, float py);
    /**
    @brief Choose to use a one-dimensional x- or y-slider or a two-D trackpad.
    
    @param md SliderMode reflecting your choice of usage.
    */ 
    void setMode(SliderMode md);
    /**
    @brief Sets the UI action which happens when finger is dragged within a slider.
    
    @param du A function pointer where the function must be of the form void f(void)
    */
    void setAction(void(*sf)());
    /**
    @brief Execute the action set as the slide action.
    */
    void sliderMotion();
};

/**
@brief Textbox class can be used to display strings of text. Output only.
*/
class Textbox : public Control {
public: 
	/** Default constructor of a Textbox object. Initializes with safe values */
    Textbox();
    /**
    Constructor with user-defined values
    
    @param nm String that is a unique identifier
    @param x Left position of control as a percentage of screen width
    @param y Top position of control as a percentage of screen height
    @param w Width of control as a percentage of screen width
    @param h Height of control as a percentage of screen height
    @param c1 Foreground color (text color) in 5-6-5 format
    @param c2 Background color in 5-6-5 format
    */
    Textbox(String nm, unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint16_t c1,  uint16_t c2);
};
#endif /* _DAQ_CONTROLS_INCLUDE_ */
