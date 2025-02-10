/**

@file

This documents the main object of the GigaDAQ project. This software is specifically for use with the Arduino GIGA R1 WiFi with the Arduino GIGA Display Shield, and allows you to make a standalone data-acquisition system (DAQ) that is interactive and can record data to a flash drive. 

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

#ifndef _GIGADAQ_INCLUDE_
#define _GIGADAQ_INCLUDE_

#include <stdio.h>
#include "DAQControls.h"

const int NUM_BUTTONS = 20;		///< Maximum number of buttons in a GigaDAQ object
const int NUM_SLIDERS =	10;		///< Maximum number of sliders in a GigaDAQ object
const int NUM_TEXTBOXES = 15;	///< Maximum number of text boxes in a GigaDAQ object

/** Orientation of Arduino GIGA Display Shield (DS) */
enum DisplayOrientation {
	PORTRAIT_USBDOWN = 0,	/**< DS held with USB ports facing down. W = 480, H = 800 pixels (typical use) */
	LANDSCAPE_USBRIGHT = 1, /**< DS held with USB ports facing right. W = 800, H = 480 pixels (typical use) */
	PORTRAIT_USBUP = 2,		/**< DS held with USB ports facing up. W = 480, H = 800 pixels (unusual use) */
	LANDSCAPE_USBLEFT =	3	/**< DS held with USB ports facing left. W = 800, H = 480 pixels (unusual use) */
};

const unsigned int GIGA_DS_WIDTH = 480;		///< In default rotation, screen width in pixels
const unsigned int GIGA_DS_HEIGHT = 800;	///< In default rotation, screen height in pixels

class GigaDAQ {
public:
    unsigned int screenW;		///< Screen width in pixels
    unsigned int screenH;		///< Screen height in pixels
    DisplayOrientation rotation;	///< Orientation for display and touch calculations
    Button button[NUM_BUTTONS];		///< Array of Button objects
    Slider slider[NUM_SLIDERS];		///< Array of Slider objects
    Textbox textbox[NUM_TEXTBOXES];	///< Array of Textbox objects
    Event currentEvent;			///< Most recent touch event
    Event previousEvent;		///< Touch event prior to current one
    GigaDisplay_GFX graph;		///< Object for screen drawing functions
	Arduino_GigaDisplayTouch touch; ///< Object for touch screen functions
	
	FILE *fp;					///< File pointer for data-logging operations
    /** Constructor for object. Initializes object with safe values */
    GigaDAQ();
    /** Constructor with user-selected orientation
    
    @param rotation Orientation of Display Shield when sketch runs.
    */
    GigaDAQ(DisplayOrientation rotation);
    /**
    @brief Starts graph and touch objects and checks real-time clock. If real-time clock (RTC) has a reasonable value, that is accepted. Otherwise, an arbitrary value is inserted to give data files a reasonable timestamp.
    */
    void begin(void);
    /**
    @brief Draws all objects (where the width and height are both greater than zero) regardless if they are current or stale.
    */
    void drawAll();
    /**
    @brief Forces the drawing of a button at the given array position.
    
    @param num Array position of button to be drawn. Must be an integer between 0 and NUM_BUTTONS-1.
    */
    void drawButton(int num);
    /**
    @brief Forces the drawing of a slider at the given array position.
    
    @param num Array position of slider to be drawn. Must be an integer between 0 and NUM_SLIDERS-1.
    */
    void drawSlider(int num);
    /**
    @brief Forces the drawing of a text box at the given array position.
    
    @param num Array position of text box to be drawn. Must be an integer between 0 and NUM_TEXTBOXES-1.
    */
    void drawTextbox(int num);
    /**
    @brief Creates an event based on where a touch point is. Determines if touch point is in an input control.
    
    @param touchX x-pixel of touch Event
    @param touchY y-pixel of touch Event
    @note Internal use only.
    */
    void locate(int touchX, int touchY);
    
    /**
    @brief Find array index of control given its type and name
    
    @param type Control type: button, slider, textbox, etc.
    @param name Unique name assigned to control
    
    @returns Array index between 0 and NUM_... - 1, on success
    @returns -1 on failure
    */
    int arrayPosition(ControlType type, String name);
    /**
    @brief Interprets action based on current and previous events.
    If the previous event is in a button and the current is in nothing, a button up action is triggered.
    If both events are in one slider, a slide motion action takes place.
    */
    void takeAction(void);
    /**
    @brief Polls the touch screen for a touch point, creates an event and takes action if needed. It is recommended that this function be called at intervals between 100 and 200 milliseconds. Use judgment if going outside this range.
    */
    void handleInputs(void);
    /**
    @brief Redraw text boxes where the display text and previous display text are different
    */
    void updateDisplays(void);
    /**
    @brief Attempts to open a file with the given name on the flash drive.
    
    A flash drive (thumb drive) must be successfully connected and mounted for this function to succeed. If this function succeeds, the file pointer, fp, will be non-NULL. Before using any of the C file writing functions like fprintf, fputs, fwrite, etc., test to make sure that fp is non-NULL and skip if it is NULL.
    
    @param fileName Desired name of file to record to. The string "/usb/" will be placed before the given name to ensure it records to the drive.
    
    @note The file is opened in append mode. If the file does not exist, it is created. If the file does exist, it is opened and data is added to the end of the file, that is, it does not overwrite the old data.
    
    @note If you want to place the file anywhere other than the top level of the flash drive directory structure, you will have to write the path explicitly and it will only work if the folders exist. Folders that don't exist will not be automatically created.
    */
    void startDataRecording(String fileName);
    /**
    @brief Closes the data file.
    
    Failure to close the file properly will result in a loss of data.
    */
    void endDataRecording();

	tm timeBD;		///< Time structure containing elements of "broken-down" time
	time_t tmStr;	///< Integer representation of time. Use localtime() to interpret value.
};
#endif /* _GIGADAQ_INCLUDE_ */
