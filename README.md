# jackKeySplitter: A MIDI keyboard splitter for JACK

## Functionality
jackKeySplitter is a JACK client that accepts a single MIDI input and demultiplexes it into two or more outputs. I wrote it both to learn about MIDI handling in JACK, and to facilitate the simultaneous synthesis of multiple virtual instruments with a single MIDI keyboard. (It's more fun to improvise in the treble range if you can simulate a bass with the left hand.)

## Usage
```./jkeysplit [UPPER BOUNDARY]...```
### In Brief
  To split the keyboard in half (at "Middle C", or C5, or MIDI note 60):
  1. Start the JACK server.
  2. ```make``` the project and run ```./jkeysplit 60```
  3. Connect a MIDI source to the KeySplitter input port
  4. Use the KeySplitter output ports 0 and 1 as desired. Notes between 0 and 60 will output at port 0; notes between 60 and 127 will be sent through port 1.
### In Detail
  ```jkeysplit``` accepts ascending integer arguments between 0 and 126 (representing MIDI notes). If ```n``` integers are passed, then the keyboard will be split into ```n + 1``` MIDI outputs. It's possible to split the keyboard into as many segments as desired, with a theoretically negligible effect on performance. An array is used as a look-up table to determine which output port should receive an incoming MIDI event, so as to avoid unnecessary calculation in the realtime ```process()``` function.
