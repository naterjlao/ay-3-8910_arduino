#include "include/music.hpp"

static uint16_t calc_frequency(const MUSIC::NOTE_FREQ note,
    const MUSIC::REGISTER octave)
{
    return (octave > MUSIC::MIDDLE_OCTAVE) ? (note >> (octave - MUSIC::MIDDLE_OCTAVE)) : (note << (MUSIC::MIDDLE_OCTAVE - octave));
}

void MUSIC::play(const NOTE note, const AY3::CHANNEL ch = AY3::CHANNEL_A, const COMMAND command = ON, const uint16_t pitchbend = PITCHBEND_MID)
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

void MUSIC::play_track(const NOTE *track, const unsigned long bpm, const size_t len, const size_t nchannels)
{
    const AY3::CHANNEL ch_map[3] = {AY3::CHANNEL_A, AY3::CHANNEL_B, AY3::CHANNEL_C};
    for (size_t marquee = 0; marquee < len; marquee++)
    {
        for (size_t ch_idx = 0; ch_idx < nchannels; ch_idx++)
        {
            NOTE note = track[(ch_idx * len) + marquee];
            AY3::CHANNEL ch = ch_map[ch_idx % 3];
            if (note.note != MUSIC::HOLD_FLAG)
            {
                MUSIC::play(note,ch,(note.note == MUSIC::REST_FLAG) ? MUSIC::OFF : MUSIC::ON);
            }
        }
        delay(bpm); /** @todo this is incorrect, should calculate delay bpm from bpm*/
    }
}