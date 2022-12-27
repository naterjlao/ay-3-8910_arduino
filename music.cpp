#include "include/music.hpp"

static uint16_t calc_frequency(MUSIC::NOTE_FREQ note, MUSIC::REGISTER octave)
{
    return (octave > MUSIC::MIDDLE_OCTAVE) ? (note >> (octave - MUSIC::MIDDLE_OCTAVE)) : (note << (MUSIC::MIDDLE_OCTAVE - octave));
}

void MUSIC::play(NOTE note, AY3::CHANNEL ch = AY3::CHANNEL_A, COMMAND command = ON)
{
    if (command == MUSIC::ON)
    {
        AY3::regset_enable(ch, true);
        AY3::regset_period(ch, calc_frequency(note.note, note.octave));
    }
    else
    {
        AY3::regset_enable(ch, false);
    }
}