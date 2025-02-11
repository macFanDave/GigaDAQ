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
5. [Data Logging](#data-logging)

## Introduction <a name="introduction"></a>

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

## Percentage Concept  <a name="percentage-concept"></a>

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

## Controls <a name="controls"></a>

A GigaDAQ object contains arrays of Button, Slider and Textbox objects that are all initialized with zero width and zero height.

Buttons and Sliders are both *input* controls and Textboxes are *output* controls. 

For most cases, you will lay out all of the controls and assign their actions in the ```setup()``` routine.

You can add a name, x and y coordinates, height and width, background and foreground colors individually, but it is more convenient and compact to use a constructor to do all of that in a single line. These will be demonstrated for each control type.

### Text Boxes <a name="text-boxes"></a>

Text Boxes are output controls that display the text in the **String** property dispText. The box with text is redrawn either when a ```drawTextbox()``` command is issued or when a difference between the dispText and the previous String, prevDispText, are not equal.

Open example sketch **0-Text_Box_Display** to see a sketch featuring a text box. The example also contains info about C-strings, stdio.h and time.h functions from standard C.

- Textbox Constructor <a name="textbox-constructor"></a>

```cpp
daq.textbox[0] = Textbox("Time Display", 5, 20, 90, 15, BLACK, WHITE);
```
This creates a text box named "Time Display." (Note: Characters within quotation marks (") are **String** objects.)

The upper left corner of the box is 5% of the screen width from the left edge of the screen, and 20% of the screen height from the top edge.

The text box has a width of 90% of the screen width and 15% of the screen height.

The foreground (text) color is BLACK and the background color is WHITE.

- Textbox setDisplayText() <a name="textbox-setdisplaytext"></a>

```cpp
daq.textbox[0].setDisplayText("00:00:00");
```
Assigns a **String** value to the dispText property of a textbox object.
It is important to use this function to change the text field since it assigns the existing value to the previous display text property before it updates the current value. This is necessary to signal that the textbox needs to be redrawn.

Numbers and C-style strings need to be converted to **String** types using the ```String()``` function.

### Buttons <a name="buttons"></a>

Buttons are input controls that can respond to presses. In modern UIs, the action that actually triggers the related action to occur is when a finger on a button leaves the control. This is called a Button Up action and is the only one supported by GigaDAQ.

Open example sketch **1-Buttons** to see how to create and use buttons. You can see how to associate buttons with actions, where actions must be functions of the form ```void function(void)```.

- Button Constructor <a name="button-constructor"></a>

```cpp
daq.button[0] = Button("Down button", 5, 40, 40, 20, WHITE, RED);
```
This creates a button named "Down button."

The upper left corner of the button is 5% of the screen width from the left edge of the screen, and 40% of the screen height from the top edge.

The button has a width of 40% of the screen width and 20% of the screen height.

The foreground (text) color is WHITE and the background color is RED.

- Button setAction <a name="button-setaction"></a>

```cpp
daq.button[0].setAction(buttonAction);
```
*buttonAction* is a function of the form ```void buttonAction(void)```. This will execute every time a finger leaves a button (a button up action).

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

- Button setDisplayText() <a name="button-setdisplaytext"></a>

```cpp
daq.button[0].setDisplayText("Decrease Value");
```
Assigns a **String** value to the dispText property of a button object. Since a button is an input object, changing the dispText does not trigger a redraw. This needs a ```daq.drawAll()``` or ```daq.drawButton()``` command.


### Sliders <a name="sliders"></a>

Sliders are input controls that respond to finger drags (or swipes). The associated action occurs when the previous event and current event happen in the same slider at two different positions.

Open example sketch **2-Sliders** to see the implementation of a horizontal and a vertical slider.

- Slider Constructor <a name="slider-constructor"></a>

```cpp
daq.slider[0] = Slider("Green vert", 75, 2, 24, 80, GREEN, WHITE);
```
This creates a slider named "Green vert."

The upper left corner of the button is 75% of the screen width from the left edge of the screen, and 2% of the screen height from the top edge.

The slider has a width of 24% of the screen width and 80% of the screen height.

The foreground color is GREEN and the background color is WHITE.

- Slider setMode() <a name="slider-setmode"></a>

```cpp
daq.slider[0].setMode(VERTICAL);
```
Assigns a **SliderMode** value to the mode property of a slider object. The available modes are:

HORIZONTAL    Wide, short slider where only the x-value changes (y stays at 1.0);
VERTICAL      Tall, narrow slider where only the y-value changes (x stays at 1.0);
TRACKPAD      Rectangular area when x- and y-values are both useful.

- Slider setAction <a name="slider-setaction"></a>

```cpp
daq.slider[0].setAction(greenSlide);
```
*greenSlide* is a function of the form ```void greenSlide(void)```. This will execute every time a finger moves within this slider.

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
> 🎵   **Note:** The code above uses some features in the C language that are useful on the Arduino GIGA. The ```snprintf()```
> function requires a character (**char**) buffer and not a **String**. There are many sites and books that explain how
> things work in C. Learning about using format specifiers in the stdio.h library can be very helpful in composing complex output
> strings in a single line of code.
>
> Putting a data type in parentheses before a variable is used for *type-casting*.
> In the line:
> ```cpp
> greenValue = (uint16_t)(daq.slider[0].posY);
> ```
> I ask that the floating-point (**float**) value *posY* be converted to an unsigned 16-bit integer (**uint16_t**) before
> being assigned to *greenValue*. Modern systems often handle these conversions automatically, but some of us still like to make
> our intentions explicit.

- Slider setXlimits/setYlimits <a name="slider-setxlimits"></a>

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

- Slider posX/posY <a name="slider-posx"></a>


## User Interaction <a name="user-interaction"></a>

Buttons and sliders respond to touch screen signals

## Data Logging <a name="data-logging"></a>

A properly-formatted thumb drive can store data.

