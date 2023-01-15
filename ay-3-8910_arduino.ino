#include <SoftwareSerial.h>

#include "include/ay-3-8910.hpp"
#include "include/music.hpp"

using namespace MUSIC;

#define NCHANNELS 3
#define BPM 107
#define NTRACKS 8
#define TRACK_LEN 16

// WHAT YOU KNOW?
const NOTE track[NTRACKS][NCHANNELS][TRACK_LEN] = {
    {// 1                    |                       |                       |
     {REST, REST, {Db, 5}, HOLD, {E, 5}, HOLD, {Gb, 5}, HOLD, {E, 5}, HOLD, REST, {Db, 5}, HOLD, REST, {Db, 5}, HOLD},
     {REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST},
     {{Gb, 2}, HOLD, REST, {Gb, 2}, REST, REST, {Gb, 2}, REST, {Gb, 2}, HOLD, REST, {Gb, 2}, REST, REST, {Gb, 2}, REST}},
    {// 2                    |                       |                       |
     {REST, REST, {Db, 5}, HOLD, {E, 5}, HOLD, {Gb, 5}, HOLD, {E, 5}, HOLD, REST, {B, 4}, HOLD, REST, {Db, 5}, HOLD},
     {REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST},
     {{Gb, 2}, HOLD, REST, {Gb, 2}, REST, REST, {Gb, 2}, REST, {Db, 3}, HOLD, HOLD, REST, {Ab, 2}, HOLD, HOLD, HOLD}},
    {// 3                    |                       |                       |
     {REST, REST, {Db, 5}, HOLD, {E, 5}, HOLD, {Gb, 5}, HOLD, {E, 5}, HOLD, REST, {Db, 5}, HOLD, REST, {Db, 5}, HOLD},
     {REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST},
     {{A, 2}, HOLD, REST, {A, 2}, REST, REST, {A, 2}, REST, {A, 2}, HOLD, REST, {A, 2}, REST, REST, {A, 2}, REST}},
    {// 4                    |                       |                       |
     {{A, 5}, HOLD, HOLD, {Ab, 5}, HOLD, HOLD, {Db, 5}, REST, {A, 5}, HOLD, HOLD, {Ab, 5}, HOLD, HOLD, {B, 4}, HOLD},
     {{Gb, 5}, HOLD, HOLD, {E, 5}, HOLD, HOLD, {Ab, 4}, REST, {Gb, 5}, HOLD, HOLD, {E, 5}, HOLD, HOLD, {Ab, 4}, HOLD},
     {{A, 2}, HOLD, REST, {A, 2}, REST, REST, {A, 2}, REST, {B, 2}, HOLD, HOLD, REST, {Db, 2}, HOLD, HOLD, HOLD}},
    {// 5                    |                       |                       |
     {REST, REST, {Db, 5}, HOLD, {E, 5}, HOLD, {Gb, 5}, HOLD, {E, 5}, HOLD, REST, {Db, 5}, HOLD, REST, {Db, 5}, HOLD},
     {REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST},
     {{D, 2}, HOLD, REST, {D, 2}, REST, REST, {D, 2}, REST, {D, 2}, HOLD, REST, {D, 2}, REST, REST, {D, 2}, REST}},
    {// 6                    |                       |                       |
     {REST, REST, {Db, 5}, HOLD, {E, 5}, HOLD, {Gb, 5}, HOLD, {E, 5}, HOLD, REST, {B, 4}, HOLD, REST, {Db, 5}, HOLD},
     {REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST},
     {{D, 2}, HOLD, REST, {D, 2}, REST, REST, {D, 2}, REST, {A, 2}, HOLD, HOLD, REST, {E, 2}, HOLD, HOLD, HOLD}},
    {// 7                    |                       |                       |
     {REST, REST, {Db, 5}, HOLD, {E, 5}, HOLD, {Gb, 5}, HOLD, {E, 5}, HOLD, REST, {Db, 5}, HOLD, REST, {Db, 5}, HOLD},
     {REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST},
     {{Gb, 2}, HOLD, REST, {Gb, 2}, REST, REST, {Gb, 2}, REST, {Gb, 2}, HOLD, REST, {Gb, 2}, REST, REST, {Gb, 2}, REST}},
    {// 8                    |                       |                       |
     {{A, 5}, HOLD, HOLD, {Ab, 5}, HOLD, HOLD, {Db, 5}, REST, {A, 5}, HOLD, HOLD, REST, {Ab, 5}, HOLD, HOLD, REST},
     {{Gb, 5}, HOLD, HOLD, {E, 5}, HOLD, HOLD, {Ab, 4}, REST, {Gb, 5}, HOLD, HOLD, REST, {E, 5}, HOLD, HOLD, REST},
     {{Gb, 2}, HOLD, REST, {Gb, 2}, REST, REST, {Gb, 2}, REST, {Gb, 2}, HOLD, REST, REST, {Gb, 2}, HOLD, REST, REST}}};

SoftwareSerial midi_in = SoftwareSerial(10, 11);

void setup()
{
    AY3::begin();

    // Set Amplitude
    AY3::regset_ampltd(AY3::CHANNEL_A, 2);
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

const byte MIDI_NOTE_ON = 144;
const byte MIDI_NOTE_OFF = 128;

/// @todo put in own header
MUSIC::NOTE_FREQ midi_map[] =
    {
        MUSIC::C, // 0,12,24,36,48,60
        MUSIC::Db,
        MUSIC::D,
        MUSIC::Eb,
        MUSIC::E,
        MUSIC::F,
        MUSIC::Gb,
        MUSIC::G,
        MUSIC::Ab,
        MUSIC::A,
        MUSIC::Bb,
        MUSIC::B,
};

byte notes_on = 0;

void loop()
{
#if DEMO
    for (size_t track_idx = 0; track_idx < NTRACKS; track_idx++)
        MUSIC::play_track((const NOTE *)&track[track_idx], BPM, TRACK_LEN, NCHANNELS);
#else
    MUSIC::NOTE note;
    if (midi_in.available() > 2)
    {
        byte command_in = midi_in.read();
        byte note_in = midi_in.read();
        byte velocity_in = midi_in.read();
        if (command_in == MIDI_NOTE_ON)
        {
            note.note = midi_map[note_in % 12];
            note.octave = note_in / 12;
            MUSIC::play(note, AY3::CHANNEL_A, MUSIC::COMMAND::ON);
            notes_on++;
        }
        else if (command_in == MIDI_NOTE_OFF)
        {
            if (--notes_on == 0)
            {
                MUSIC::play(MUSIC::REST, AY3::CHANNEL_A, MUSIC::COMMAND::OFF);
            }
        }
    }
#endif
}