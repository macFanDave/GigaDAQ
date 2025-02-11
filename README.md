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
    4. [Sliders](#sliders)
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

- Textbox setDisplayText() <a name="button-setdisplaytext"></a>

```cpp
daq.button[0].setDisplayText("Decrease Value");
```
Assigns a **String** value to the dispText property of a button object. Since a button is an input object, changing the dispText does not trigger a redraw. This needs a ```daq.drawAll()``` or ```daq.drawButton()``` command.


### Sliders <a name="sliders"></a>

Can be one or two dimensional

## User Interaction <a name="user-interaction"></a>

Buttons and sliders respond to touch screen signals

## Data Logging <a name="data-logging"></a>

A properly-formatted thumb drive can store data.

