# em04MIDI
Arduino based MIDI implementation for Elektronika EM-04 string machine
<img src="https://github.com/jurczenko91/em04MIDI/blob/main/headpic.jpg" width=500>

Among my vintage synthesisers, there’s a rather interesting Soviet-made instrument - the Elektronika EM-04 string synthesiser. Besides its good points, such as a pretty decent sound, an analogue chorus based on BR-series chips, and a real transistor Moog filter (!) in the brass section (I have a project involving that one too), it also has one drawback that’s common to many vintage synthesisers - no MIDI control.

## Intro

If you look at the original EM-04 schematic, it’s pretty clear that the notes are switched on and off in a very straightforward way. The contact of each key goes to the tone generator board for its particular note, directly to the control input of its K561KT3 chip (the Soviet equivalent of the CD4066), which switches the notes together with a K561LA7. The common keyboard bus is connected to the +15 V supply through a KS133A zener diode (3.3 V).

<table>
  <tr><td><img src="https://github.com/jurczenko91/em04MIDI/blob/main/1.jpg" width=500></td></tr>
  <tr><td align=center>Pic. 1. Tone generator</td></tr>
</table>
<table>
  <tr><td><img src="https://github.com/jurczenko91/em04MIDI/blob/main/2.jpeg" width=500></td></tr>
  <tr><td align=center>Pic. 2. Harmonics synthesis block</td></tr>
</table>
<table>
  <tr><td><img src="https://github.com/jurczenko91/em04MIDI/blob/main/3.jpeg" width=500></td></tr>
  <tr><td align=center>Pic. 3. Block diagram</td></tr>
</table>

So, to keep modifications to the original circuitry to a minimum and leave the instrument’s keyboard fully functional, I decided to make the MIDI control just as straightforward: use CD4066 chips to connect the common bus to the individual key contacts. These chips will be controlled by an Arduino.

We’ll also need some 74HC595 shift registers, since the Arduino simply doesn’t have enough pins to control everything, as well as CD40109 level shifters. The latter are needed to convert the 5 V control signals to 12 V, since the CD4066 switches voltages of around 10 V.

## Schematics

So, I came up with the schematic below:
<table>
  <tr><td><img src="https://github.com/jurczenko91/em04MIDI/blob/main/em04MIDIschematic.png" width=700></td></tr>
  <tr><td align=center>Pic. 4. EM-04 MIDI schematic</td></tr>
</table>

I breadboarded and tested that with led diodes connected to the switched pins of the cd4066, and it worked well. I also tried to connect three notes of the EM04 to the corresponding pins, and it also worked well.
I will upload the PCB after some more tests.

## Code
Arduino is a perfect and rather cheap way to work with MIDI, so I made a sketch. It's a pretty basic program, which reads a MIDI message and then drives shift registers. You can find the sketch in the files section.
