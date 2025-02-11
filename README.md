# GigaDAQ
DAQ library for Arduino GIGA with Display Shield

## Table of Contents

1. [Introduction](#introduction)
2. [Percentage Concept](#percentage-concept)
3. [Controls](#controls)
    1. [Text Boxes](#text-boxes)
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

### Buttons <a name="buttons"></a>

Responds to pushes

### Sliders <a name="sliders"></a>

Can be one or two dimensional

## User Interaction <a name="user-interaction"></a>

Buttons and sliders respond to touch screen signals

## Data Logging <a name="data-logging"></a>

A properly-formatted thumb drive can store data.

