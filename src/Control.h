/**

@file

This contains some of the building blocks and utilities for the GigaDAQ project. This software is specifically for use with the Arduino GIGA R1 WiFi with the Arduino GIGA Display Shield, and allows you to make a standalone data-acquisition system (DAQ) that is interactive and can record data to a flash drive. 

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

#ifndef _CONTROL_INCLUDE_
#define _CONTROL_INCLUDE_

#include <stdio.h>
#include "Arduino.h"
#include <Arduino_GigaDisplay_GFX.h>
#include <Arduino_GigaDisplayTouch.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

/** Pre-defined colors */
const uint16_t CYAN  = 0x07FF;	///< cyan color (16-bit unsigned integer, 5-6-5 format)
const uint16_t RED   = 0xf800;	///< red color (16-bit unsigned integer, 5-6-5 format)
const uint16_t BLUE  = 0x001F;	///< blue color (16-bit unsigned integer, 5-6-5 format)
const uint16_t GREEN = 0x07E0;	///< green color (16-bit unsigned integer, 5-6-5 format)
const uint16_t MAGENTA =0xF81F;	///< magenta color (16-bit unsigned integer, 5-6-5 format)
const uint16_t WHITE = 0xffff;	///< white "color" (16-bit unsigned integer, 5-6-5 format)
const uint16_t BLACK = 0x0000;	///< black "color" (16-bit unsigned integer, 5-6-5 format)
const uint16_t YELLOW= 0xFFE0;	///< yellow color (16-bit unsigned integer, 5-6-5 format)

/** Control type identifiers, 1-99: input controls, 100- : output controls. */
enum ControlType {
	NOTHING = 0,		/**< Not a control */
	BUTTON  = 1,		/**< Button, switch input */
	SLIDER  = 2,		/**< Slider, trackpad control */
	TEXTBOX = 100		/**< Text box output control */
};

/** Available sizes for GFX Monospace fonts */
enum FontSize {
	MONO9PT = 1,	/**< 9-pt monospace font (not very legible) */
	MONO12PT,		/**< 12-pt monospace font, small, but legible */
	MONO18PT,		/**< 18-pt monospace font, medium sized */
	MONO24PT		/**< 24-pt monospace font, large sized */
};

/**
@brief Base class for controls with common members for location, size, color, etc.
*/
class Control {
public:
    String name;		///< Unique identifier for a control 
    ControlType type;	///< Required for proper drawing and action instructions
    unsigned int x;    	///< Left position of control as a percentage of screen width
    unsigned int y;		///< Top position of control as a percentage of screen height
    unsigned int w;		///< Width of control as a percentage of screen width
    unsigned int h;		///< Height of control as a percentage of screen height
    String dispText;   	///< Text to be shown in control, if needed
    String prevDispText; ///< Previous text, useful for detecting changes
    uint16_t fgColor;	///< Foreground color (text color) in 5-6-5 format
    uint16_t bgColor;	///< Background color in 5-6-5 format
    /**
    Default constructor of a Control
    Initializes objects with safe values
    */
    Control();
    
    /**
    @brief Places new value into the dispText (display text) member.
    Places existing value in prevDispText before setting dispText to new value.
    
    @param 	txt
    		A String that becomes the value displayed by the control.
    */ 		
    void setDisplayText(String txt);
};

/**
@brief Class to interpret user interactions with Display Shield touch screen.
*/
class Event {
public:
    ControlType type; ///< Type of control being touched. Can be NOTHING
    String name;	  ///< Unique identifier for selected control
    unsigned int x;	  ///< x-position of touch point
    unsigned int y;	  ///< y-position of touch point
    uint32_t t;		  ///< Time stamp of when touch occurred
    /**
    Constructor for an Event object
    */
    Event();
};

/**
@brief Class to determine the size and position of a given string.
*/
class MonoBoundingBox {
public:
	FontSize fontSize;	///< Selected size of a Monospaced font (9, 12, 18, 24 pt)
	unsigned int w;		///< Width of bounding box containing string
	unsigned int h;		///< Height of bounding box containing string
};

/**
@brief Calculates the largest bounding box that can find the given string into the given container.

@param 	dString 
		The string to be displayed in the given container.
@param 	containerWidth
		Width of container, unsigned integer
@param 	containerHeight
		Width of container, unsigned integer
@returns MonoBoundingBox object that fits the largest font possible into the container.

@note To avoid the difficulties I had using the getTextBounds() function to calculate sizes at run-time, I established these functions for 4 sizes of normal Monospaced font (9, 12, 18 and 24). The height of a string on a single line was 15, 20, 30 and 40 pixels, and the width per character was 11, 14, 21 and 28 pixels.

*/		
MonoBoundingBox maxFont(String dString, unsigned int containerWidth, unsigned int containerHeight);

#endif /* _CONTROL_INCLUDE_ */
