#include <SoftwareSerial.h>

#include "include/ay-3-8910.hpp"
#include "include/music.hpp"

using namespace MUSIC;

SoftwareSerial midi_in = SoftwareSerial(10, 11);

void setup()
{
    AY3::begin();

    // Set Amplitude MIN 1, MAX 15
    AY3::regset_ampltd(AY3::CHANNEL_A, 2); // OR with 0x100 for envelope generation 
    AY3::regset_ampltd(AY3::CHANNEL_B, 2);
    AY3::regset_ampltd(AY3::CHANNEL_C, 2);

    MUSIC::play(MUSIC::REST, AY3::CHANNEL_A, MUSIC::COMMAND::OFF);
    MUSIC::play(MUSIC::REST, AY3::CHANNEL_B, MUSIC::COMMAND::OFF);
    MUSIC::play(MUSIC::REST, AY3::CHANNEL_C, MUSIC::COMMAND::OFF);

    // Setup MIDI Input
    pinMode(10, INPUT);
    pinMode(11, OUTPUT); // This is not used
    midi_in.begin(31250);
}

/// @note assuming channel 1
const byte MIDI_NOTE_ON = 0x90;
const byte MIDI_NOTE_OFF = 0x80;
const byte MIDI_AFTERTOUCH = 0xD0;
const byte MIDI_CLOCK_CH0 = 0xF8;

MUSIC::NOTE note;
byte notes_on = 0;
byte command_in;
byte note_in;
byte velocity_in;

void loop()
{

    if (midi_in.available() > 0)
    {
        command_in = midi_in.read();
        switch (command_in)
        {
        case MIDI_NOTE_ON:
            while (midi_in.available() == 0);
            note_in = midi_in.read();
            while (midi_in.available() == 0);
            velocity_in = midi_in.read();

            note.note = MUSIC::MIDI_NOTE_MAP[note_in % 12];
            note.octave = note_in / 12;
            MUSIC::play(note, AY3::CHANNEL_A, MUSIC::COMMAND::ON);
            notes_on++;
            break;
        case MIDI_NOTE_OFF:
            while (midi_in.available() == 0);
            note_in = midi_in.read();
            while (midi_in.available() == 0);
            velocity_in = midi_in.read();
            notes_on = (notes_on - 1) % 16;
            if (!(notes_on > 0))
            {
                MUSIC::play(MUSIC::REST, AY3::CHANNEL_A, MUSIC::COMMAND::OFF);
            }
            break;
        case MIDI_AFTERTOUCH:
            // do nothing
            while (midi_in.available() == 0);
            midi_in.read();
            break;
        case MIDI_CLOCK_CH0:
            // do nothing
        default:
            break;
        }
    }
}