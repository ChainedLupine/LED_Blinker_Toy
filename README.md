# LED Blinky Toy

A silly Attiny85 project.

Built using the Arduino toolchain, compiled with GCC in Sloeber IDE.

The circuit is just three daughter boards, each with a SN74HC595 shift register and 8 LEDs, plus one master board which contains the Attiny85 and interconnects.

Control is done via a 10K pot on pin 3, and a NO switch on pin 4.

Has four modes (or 5, depending if you count "off"):

* Random pattern moving from left to right
* Random pattern moving right to left
* Cylon mode (3 LEDs that wiggle side to side)
* Fake VU Meter

In off, the Attiny85 sleeps in power down mode, checking every second to see the button on pin 4 is depressed.  If so, it cycles back to the first pattern.

In all modes, the adjusement pot changes the pattern speed.

Note:  The code expects the Attiny85 to be running in 8MHz mode.  So be sure to set the fuses appropriately (ie: CKDIV8=0), especially if you've been putting Arduino bootloaders on your MCUs.  (As happened to me...)