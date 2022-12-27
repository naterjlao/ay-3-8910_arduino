#include "include/music.hpp"

static uint16_t calc_frequency(MUSIC::NOTE_FREQ note, MUSIC::REGISTER octave)
{
    return (octave > MUSIC::MIDDLE_OCTAVE) ? (note >> (octave - MUSIC::MIDDLE_OCTAVE)) : (note << (MUSIC::MIDDLE_OCTAVE - octave));
}

void MUSIC::play(NOTE_FREQ note, REGISTER octave = MIDDLE_OCTAVE, COMMAND command = ON, AY3::CHANNEL ch = AY3::CHANNEL_A)
{
    note = calc_frequency(note, octave);
    if (command == MUSIC::ON)
    {
        AY3::regset_enable(ch, true);
        AY3::regset_period(ch, note);
    }
    else
    {
        AY3::regset_enable(ch, false);
    }
}

void MUSIC::play(NOTE note, COMMAND command = ON, AY3::CHANNEL channel = AY3::CHANNEL_A)
{
    MUSIC::play(note.note, note.octave, command, channel);
}