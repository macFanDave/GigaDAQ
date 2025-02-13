# GigaDAQ
DAQ library for Arduino GIGA with Display Shield.

This is a method to create an interactive user interface (UI) on the Arduino GIGA Display Shield in an Arduino sketch and connect it with all of the resources available to the Arduino GIGA R1 WiFi. It is able to read sensors, drive motors, regulate power to devices, communicate wirelessly, send and receive data through USB-A and USB-C port, and more. Here, we will show you how to create the UI in the `setup()` function of an Arduino sketch and control the event loop in the `loop()` function.

## Table of Contents

1. [Introduction](#introduction)
2. [Percentage Concept](#percentage-concept)
3. [Controls](#controls)
    1. [Text Boxes](#text-boxes)
       * [Textbox Constructor](#textbox-constructor)
       * [Textbox setDisplayText](#textbox-setdisplaytext)
    2. [Buttons](#buttons)
        * [Button Constructor](#button-constructor)
        * [Button setAction](#button-setaction)
        * [Button setDisplayText](#button-setdisplaytext)
    3. [Sliders](#sliders)
        * [Slider Constructor](#slider-constructor)
        * [Slider setMode](#slider-setmode)
        * [Slider setAction](#slider-setaction)
        * [Slider setXlimits/setYlimits](#slider-setxlimits)
        * [Slider setPosition](#slider-setposition)
        * [Slider posX/posY](#slider-posx)
        
4. [User Interaction](#user-interaction)
     * [GigaDAQ handleInputs](#gigadaq-handle-inputs)
     * [Calling Functions at Intervals](#gigadaq-calling-functions-at-intervals)
     * [GigaDAQ updateDisplays](#gigadaq-update-displays)
5. [Data Logging](#data-logging)
 	* [Connecting to a Flash Drive](#connecting-to-a-flash-drive)
 	* [GigaDAQ startDataRecording](#gigadaq-startdatarecording)
 	* [Write Data to File](#write-data-to-file)
 	* [GigaDAQ endDataRecording](#gigadaq-enddatarecording)
 	* [Using a Button as a Toggle Switch](#button-as-toggle-switch)
6. [Future Enhancements](#future-enhancements)

***

# Introduction <a name="introduction"></a>

*Before setup() and loop() blocks, declare a GigaDAQ object with the display orientation as the parameter.*

~~~cpp
GigaDAQ daq(PORTRAIT_USBDOWN);
~~~
You are free to choose any valid name for the object. I will use *daq* in this guide and in the examples.

PORTRAIT_UBSDOWN is the default orientation. This view is a tall, narrow screen with the USB ports facing down. The other options are:

| Option | Description |
| ---  | --- |		        		                          
| LANDSCAPE_USBRIGHT  | Wide screen, USB ports on the right (typical)  |     
| PORTRAIT_USBUP | Tall, narrow screen, USB ports facing up (unusual) |
| LANDSCAPE_USBLEFT | Wide screen, USB ports on the left (unusual) |      

*Invoke the begin() function early in the setup() routine.*

```cpp
daq.begin();
```
***

# Percentage Concept  <a name="percentage-concept"></a>

In laying out a user interface, the designer needs to know whether it will be used in
Portrait or Landscape mode, or whether it will have to switch between the two. To ensure portability and handle future
changes in pixel counts, controls are placed and sized on a 100 x 100 grid. A left/right or width parameter is expressed as a percentage
of screen width (0 to 100%), and a down/up or height parameter is a percentage of screen height.

The standard order to specify a rectangle is:
1. x (leftmost position)
2. y (topmost position)
3. Width
4. Height

(0, 0) is the top left corner and y increases downward.

***

# Controls <a name="controls"></a>

A GigaDAQ object contains arrays of Button, Slider and Textbox objects that are all initialized with zero width and zero height.

Buttons and Sliders are both *input* controls and Textboxes are *output* controls. 

For most cases, you will lay out all of the controls and assign their actions in the `setup()` routine.

You can add a name, x and y coordinates, height and width, background and foreground colors individually, but it is more convenient and compact to use a constructor to do all of that in a single line. These will be demonstrated for each control type.

***

## Text Boxes <a name="text-boxes"></a>

Text Boxes are output controls that display the text in the **String** property dispText. The box with text is redrawn either when a `drawTextbox()` command is issued or when a difference between the dispText and the previous String, prevDispText, are not equal.

Open example sketch **0-Text\_Box\_Display** to see a sketch featuring a text box. The example also contains info about C-strings, stdio.h and time.h functions from standard C.

## Textbox() Constructor <a name="textbox-constructor"></a>

```cpp
daq.textbox[0] = Textbox("Time Display", 5, 20, 90, 15, BLACK, WHITE);
```
1. This creates a text box named "Time Display." (Note: Characters within quotation marks (") are **String** objects.)
- The upper left corner of the box is 5% of the screen width from the left edge of the screen, and 20% of the screen height from the top edge.
- The text box has a width of 90% of the screen width and 15% of the screen height.
-  The foreground (text) color is BLACK and the background color is WHITE.

##Textbox setDisplayText() <a name="textbox-setdisplaytext"></a>

```cpp
daq.textbox[0].setDisplayText("00:00:00");
```
Assigns a **String** value to the dispText property of a textbox object.

It is important to use this function to change the text field since it assigns the existing value to the previous display text property before it updates the current value. This is necessary to signal that the textbox needs to be redrawn.

Numbers and C-style strings need to be converted to **String** types using the `String()` function.

***

## Buttons <a name="buttons"></a>

Buttons are input controls that can respond to presses. 

In modern UIs, the action that actually triggers the related action to occur is when a finger on a button leaves the control. This is called a Button Up action and is the only one supported by GigaDAQ.

Open example sketch **1-Buttons** to see how to create and use buttons. You can see how to associate buttons with actions, where actions must be functions of the form `void function(void)`.

##Button() Constructor <a name="button-constructor"></a>

```cpp
daq.button[0] = Button("Down button", 5, 40, 40, 20, WHITE, RED);
```
1. This creates a button named "Down button."
2. The upper left corner of the button is 5% of the screen width from the left edge of the screen, and 40% of the screen height from the top edge.
3. The button has a width of 40% of the screen width and 20% of the screen height.
- The foreground (text) color is WHITE and the background color is RED.

##Button setAction() <a name="button-setaction"></a>

```cpp
daq.button[0].setAction(buttonAction);
```
*buttonAction* is a function of the form `void buttonAction(void)`. This will execute every time a finger leaves a button (a button up action).

If your buttons do very different things, it's better to write a separate function for each one. If they do similar things, you might like to write one function and use if-then-else logic to get the right action.

Here is the function called by both buttons in the sketch:

```cpp
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
```

##Button setDisplayText() <a name="button-setdisplaytext"></a>

```cpp
daq.button[0].setDisplayText("Decrease Value");
```
Assigns a **String** value to the dispText property of a button object. Since a button is an input object, changing the dispText does not trigger a redraw. This needs a `daq.drawAll()` or `daq.drawButton()` command.

***

## Sliders <a name="sliders"></a>

Sliders are input controls that respond to finger drags (or swipes). 

The associated action occurs when the previous event and current event happen in the same slider at two different positions.

Open example sketch **2-Sliders** to see the implementation of a horizontal and a vertical slider.

## Slider() Constructor <a name="slider-constructor"></a>

```cpp
daq.slider[0] = Slider("Green vert", 75, 2, 24, 80, GREEN, WHITE);
```
1. This creates a slider named "Green vert."
2. The upper left corner of the button is 75% of the screen width from the left edge of the screen, and 2% of the screen height from the top edge.
3. The slider has a width of 24% of the screen width and 80% of the screen height.
4. The foreground color is GREEN and the background color is WHITE.

## Slider setMode() <a name="slider-setmode"></a>

```cpp
daq.slider[0].setMode(VERTICAL);
```
Assigns a **SliderMode** value to the mode property of a slider object. The available modes are:


Mode | Description
----- | -------
HORIZONTAL | Wide, short slider where only the x-value changes (y stays at 1.0) 
VERTICAL | Tall, narrow slider where only the y-value changes (x stays at 1.0)
TRACKPAD | Rectangular area when x- and y-values are both useful.

##Slider setAction() <a name="slider-setaction"></a>

```cpp
daq.slider[0].setAction(greenSlide);
```
*greenSlide* is a function of the form `void greenSlide(void)`. This will execute every time a finger moves within this slider.

Here is the function called by this slider:

```cpp
void greenSlide(void){
  char textBuf[32];

  greenValue = (uint16_t)(daq.slider[0].posY);
  variableColor = mediumRed + greenValue*32 + blueValue*1;
  daq.textbox[0].bgColor = variableColor;

  snprintf(textBuf, 31, "B%d x G%d", blueValue, greenValue);
  daq.textbox[0].setDisplayText(String(textBuf));
}
```
> 🎵   **Note:** The code above uses some features in the C language that are useful on the Arduino GIGA. The `snprintf()`
> function requires a character (**char**) buffer and not a **String**. There are many sites and books that explain how
> things work in C. Learning about using format specifiers in the stdio.h library can be very helpful in composing complex output
> strings in a single line of code.
>
> Putting a data type in parentheses before a variable is used for *type-casting*.
> In the line:
> 
> ```cpp
> greenValue = (uint16_t)(daq.slider[0].posY);
> ```
> I ask that the floating-point (**float**) value *posY* be converted to an unsigned 16-bit integer (**uint16_t**) before
> being assigned to *greenValue*. Modern systems often handle these conversions automatically, but some of us still like to make
> our intentions explicit.

##Slider setXlimits()/setYlimits() <a name="slider-setxlimits"></a>

By default, sliders scale from a minimum value of 0.0 to a maximum of 1.0. It is important to note that the limits of slider values are floating-point (**float**) numbers and not integers.
You should set the X limits for horizontal sliders and Y limits for vertical sliders. For trackpads, you should set both.

```cpp
daq.slider[0].setYlimits(0.0, 63.0);
```
For a vertical slider, the value at the bottom is 0.0 and the value at the top is 63.0;

- Slider setPosition <a name="slider-setposition"></a>

```cpp
daq.slider[0].setPosition(1.0, (float)greenValue);
```
For a vertical slider, the x-position should be 1.0 and the y-position should be the desired starting point.
For a horizontal slider, the x-position should be the desired starting point and the y-position should be 1.0.
For a trackpad, set both starting values.

##Slider posX/posY <a name="slider-posx"></a>

The current x or y position of the slider is found in the `posX` or `posY` properties.  

```cpp
greenValue = (uint16_t)(daq.slider[0].posY);
```
***

# User Interaction <a name="user-interaction"></a>

Once the UI is created in the `setup()` routine, it is time to go into the `loop()` to jump into the "event loop"

Once the `loop` is entered, it repeats until the power is cut or a new sketch is uploaded. Arduinos can cycle their loops thousands of times a second and calling functions every time can generate excessive data or even spurious results. One must exercise judgment as to how much information is appropriate.

##GigaDAQ handleInputs()<a name="gigadaq-handle-inputs"></a>

```cpp
daq.handleInputs();
```

This function polls (requests data) from the touch screen and records the event for that moment in time. I have found that making these requests between 5 and 10 times a second (that is, an interval between 100 and 200 msec) yields good results. You may want to experiment with other intervals. Specific instructions for setting the interval are given in the next section.

> 🐊 **WARNING!** Use only one finger on the touch screen. The screen is equipped to accept multiple fingers, so your results will be unpredictable if you apply more than one finger.
> 

This function interprets events and triggers input device actions. If the current event is a NOTHING and the previous one is a BUTTON, then that Button's action, a button up, is called. If the current and previous events are both in the same Slider, a Slider move event is initiated.

##Calling Functions at Intervals<a name="calling-functions-at-intervals"></a>

One way to control the flow of data is to use the built-in timer to indicate when to call a function. For most applications, the `millis()` function is optimal. It reports the number of milliseconds elapsed since the sketch started as an unsigned long integer (**uint32_t**), a number that ranges from 0 to 4,294,967,295 (It "rolls over" after about 49 days.)

The procedure involves checking the *currentTime* at the top of the `loop()` and finding the difference with the *previousTime* the function was called. Once that difference equals or exceeds a *UI_INTERVAL*, call the function.

**Here's the critical step. DO NOT FORGET IT!** Set the *previousTime* equal to the *currentTime* to begin the counting all over again.

This is one way to implement this technique.

Before the `setup()` routine, add:

```cpp
uint32_t currentTime, previousTime;
const uint32_t UI_INTERVAL = 200; //milliseconds, 5 readings per second
```
Near or at the bottom of the `setup()` routine, insert:

```cpp
previousTime = millis();
```

Finally, insert this near the top of the `loop()`:

```cpp
currentTime = millis();

if(currentTime - previousTime >= UI_INTERVAL){
	//Execute desired function(s)
	
	previousTime = currentTime;
}
```
> 🎵 **NOTE:** If your interval is long and you would like to call that function as soon as the `loop()` starts, you can set *previousTime* in the `setup()` to something like 4,295,000,000 to get a first call to the function immediately.
> `previousTime = 4295000000`

##GigaDAQ updateDisplays()<a name="gigadaq-update-displays"></a>

```cpp
daq.updateDisplays();
```
This function checks if the output controls (text boxes) have changed and redraws the ones that have. You can let that run every time in the `loop()` or schedule it to run at its own interval.

The underlying logic is that if the *dispText* and *prevDispText* of an output controls are different, redraw that control.

***

#Data Logging<a name="data-logging"></a>

An essential feature of a data acquisition system is the ability to store data to a file for later processing. The Arduino GIGA R1 WiFi has a USB-A port which can be used with a thumb drive to record data.

Open example sketch **3-Trackpad\_with\_Datalogging** to see how to log data to a flash drive with an example using a Slider in trackpad mode. Also, this sketch demonstrates using a Button as a toggle switch.

##Connecting to a Flash Drive<a name="connecting-to-a-flash-drive"></a>


Before the `setup()` routine, add this code:

```cpp
#include <Arduino_USBHostMbed5.h> 
#include <DigitalOut.h>           
#include <FATFileSystem.h>  		

USBHostMSD msd;                 
mbed::FATFileSystem usb("usb");  
```
At the top of the  `setup()` routine, put in these lines:

```cpp
int i=0, err=0;             
bool msdConnected = false; 
```
 Further down in `setup()`, add at least the following code:
 
```cpp
pinMode(PA_15, OUTPUT);         
digitalWrite(PA_15, HIGH);       
do{                                    
    msdConnected = msd.connect();        
    delay(1000);                         
    i++;                                
}while(msdConnected == false && i < 10); 
err = usb.mount(&msd); 
```
##GigaDAQ startDataRecording()<a name="gigadaq-startdatarecording"></a>

```cpp
daq.startDataRecording("TrackpadDemo.csv");
```
This command attempts to open a file for writing text. If this attempt fails, the file pointer, *daq.fp* , will be NULL. Before performing any other operations (fprintf(), fclose(), etc.), check that *daq.fp* is NOT NULL. Trying to use these on a NULL pointer often causes crashes.

The file name is a **String** and will create a file at the top level of the directory structure on the flash drive. If you wish to file it in an existing directory (folder), enter the whole path **without** a leading slash (/).

The data files are created in *append* mode, meaning that the file will be created if it doesn't exist, but if it does exist, data will be appended to the existing file and not overwrite it.

##Write Data to File<a name="write-data-to-file"></a>

In the stdio.h library, there are several functions to write data to a file, like fputs() and fwrite(). Perhaps the simplest for writing formatted text to a file is fprintf().

```cpp
if(daq.fp != NULL){
	fprintf(daq.fp, "%.3f, %.1f, %.1f\n", (float)millis()/1000.0, xValue, yValue); 
}
```
The first argument of the function is the file pointer, *daq.fp*. Do not use this function if *daq.fp* is NULL, so you see from the code that this is checked.

The second argument is the format string, which contains format codes or specifiers. There are many references to show you how to create the format string that suits your needs. The rules of format strings are the same for all the functions that end in ...printf, (including printf() itself.)

The number of percent signs (%) in the format string determines how many arguments go in the third position and after.
 
##GigaDAQ endDataRecording()<a name="gigadaq-enddatarecording"></a>

```cpp
daq.endDataRecording();
```
This step is required to make your data file useful. Without doing this, it is likely that your flash drive will contain arbitrary junk.

##Using a Button as a Toggle Switch<a name="button-as-toggle-switch"></a>

Instead of programming one button to start data recording and another one to end it, we can program a single button to toggle between the two actions.

We want to "Record data" and do its opposite, "Stop recording." Look at this code:

```cpp
void dataButton(void){
  if(daq.button[0].dispText.equals("Record data")){
    daq.startDataRecording("TrackpadDemo2.csv");
    daq.button[0].fgColor = YELLOW;
    daq.button[0].bgColor = RED;
    daq.button[0].setDisplayText("Stop recording");
    daq.drawButton(0);
  }
  else if(daq.button[0].dispText.equals("Stop recording")){
    daq.endDataRecording();				
    daq.button[0].fgColor = WHITE;
    daq.button[0].bgColor = BLUE;
    daq.button[0].setDisplayText("Record data");
    daq.drawButton(0);
  }
}
```
The action taken depends on the value of the *dispText* String. Once that action occurs, the *dispText* String is set to the opposite value.

Since buttons are input controls, the redraw `daq.drawButton()` function must be called explicity.

***

#🔮Future Enhancements<a name="future-enhancements"></a>

I have released the GigaDAQ library in a simple, but useful form.

Here are some ideas I plan to implement in updates (in no particular order)

- **Flash Drive Use** I tried to make the flash drive interaction very simple, but I did not succeed in being able to implement the code in this library. There's a dozen or so lines of code that you'll have to copy-and-paste into your sketches for now.
- **Output Control: Graph** I'd like to put a miniature version of the Serial Plotter into the GigaDAQ.
- **Output Control: Gauge** A control that looks like an analog gauge would be a nice enhancement. So much information is conveyed at a glance by a needle on a dial that cannot be comprehended by digits alone.
- **Dual Core Use** One great thing about the Arduino GIGA R1 WiFi is that it actually uses two cores. It seems that a good option is to use the more powerful M7 core for demanding tasks while putting the user interface (UI) functionality on the M4 core.
- **Wireless Data Transfer** While writing data to a thumb drive will often be convenient, there may be times when it is not. In these cases, transmitting data wirelessly via WiFi and/or Bluetooth would be necessary. From my previous experience with the Nano 33 IoT, I know there's some work to be done before it can be useful.
- **Advanced ADC** When making measurements, analog-to-digital conversion is often at the center of the discussion. With the GIGA, Advanced ADC has been included and I would like to explore ways to make it easy to use within GigaDAQ

  
  

