// An Arduino sketch for implementing MIDI into some old synth gear
// This is a 49 keys version, but you can add more
// Code and schematics by Anton Yurchenko, 2026
// https://github.com/jurczenko91, https://instagram.com/jurczenko91

#include <MIDI.h>

// MIDI setup
MIDI_CREATE_DEFAULT_INSTANCE();

const int numberOfKeys = 49;

// Shift registers setup
const int dataPin = 8;   // DS
const int latchPin = 9;  // ST_CP
const int clockPin = 10; // SH_CP

const int numberOfRegisters = (numberOfKeys + 7) / 8; // ceil(49/8)=7

// Keys state array 
byte noteStates[numberOfRegisters];

const byte firstNote = 36; // C2, if your keyboard starts from C2

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI); // Receiving all channels

  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  clearAllNotes();

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
}

void loop() {
  MIDI.read();
}

void handleNoteOn(byte channel, byte note, byte velocity) {
  if (note >= firstNote && note < firstNote + numberOfKeys) {
    int index = note - firstNote;
    setNoteState(index, true);
  }
}

void handleNoteOff(byte channel, byte note, byte velocity) {
  if (note >= firstNote && note < firstNote + numberOfKeys) {
    int index = note - firstNote;
    setNoteState(index, false);
  }
}

void setNoteState(int index, bool state) {
  int reg = index / 8;
  int bit = index % 8;

  if (state) {
    noteStates[reg] |= (1 << bit);  // Press = HIGH (1)
  } else {
    noteStates[reg] &= ~(1 << bit); // Release = LOW (0)
  }

  updateShiftRegisters();
}

void updateShiftRegisters() {
  digitalWrite(latchPin, LOW);
  // Sending data in reverse order (the last register is the first)
  for (int i = numberOfRegisters - 1; i >= 0; i--) {
    shiftOut(dataPin, clockPin, MSBFIRST, noteStates[i]);
  }
  digitalWrite(latchPin, HIGH);
}

void clearAllNotes() {
  for (int i = 0; i < numberOfRegisters; i++) {
    noteStates[i] = 0x00; // All LOW (released)
  }
  updateShiftRegisters();
}
