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
    3. [Sliders](#sliders)
4. [User Interaction](#user-interaction)
5. [Data Logging](#data-logging)

## Introduction <a name="introduction"></a>

The amazing Arduino GIGA R1 WiFi with the Arduino GIGA Display Shield

~~~cpp
GigaDAQ daq(PORTRAIT_USBDOWN);
~~~

| Option | Description |
| ---  | --- |		        		                          
| LANDSCAPE_USBRIGHT  | Wide screen, USB ports on the right (typical)  |     
| PORTRAIT_USBUP | Tall, narrow screen, USB ports facing up (unusual) |
| LANDSCAPE_USBLEFT | Wide screen, USB ports on the left (unusual) |      

## Percentage Concept  <a name="percentage-concept"></a>

In laying out a user interface, the designer needs to know whether it will be used in
Portrait or Landscape mode, or whether it will have to switch between the two.

100 x 100 grid.

0, 0 is the top left corner and y increases downward.

## Controls <a name="controls"></a>

These are created in the setup() step

### Text Boxes <a name="text-boxes"></a>

Holds Text

- Textbox Constructor <a name="textbox-constructor"></a>

```cpp
daq.textbox[0] = Textbox("Box name", 10, 5, 60, 22, BLACK, WHITE);
```
- Textbox setDisplayText() <a name="textbox-setdisplaytext"></a>

```cpp
daq.textbox[0].setDisplayText("New text string");
```


### Buttons <a name="buttons"></a>

Responds to pushes

### Sliders <a name="sliders"></a>

Can be one or two dimensional

## User Interaction <a name="user-interaction"></a>

Buttons and sliders respond to touch screen signals

## Data Logging <a name="data-logging"></a>

A properly-formatted thumb drive can store data.

