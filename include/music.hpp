#ifndef __MUSIC_HPP__
#define __MUSIC_HPP__

#include <Arduino.h>
#include "ay-3-8910.hpp"

namespace MUSIC
{
    typedef uint16_t NOTE_FREQ;
    typedef uint8_t REGISTER;

    static const NOTE_FREQ HOLD_FLAG = 0XFFFF;
    static const NOTE_FREQ REST_FLAG = 0X0;
    static const REGISTER MIDDLE_OCTAVE = 4;

    /// @note The value of period registers can be determined as follows:
    /// Period Value = (Clock Frequency / Interrupt Frequency) / (Target Frequency)
    /// Where:
    /// Clock Frequency = 2MHz = 2,000,000 Hz
    /// Interrupt Frequency = 16Hz

    const NOTE_FREQ C = 0x1DE;
    const NOTE_FREQ Db = 0x1C3;
    const NOTE_FREQ D = 0x1AA;
    const NOTE_FREQ Eb = 0x192;
    const NOTE_FREQ E = 0x17B;
    const NOTE_FREQ F = 0x166;
    const NOTE_FREQ Gb = 0x152;
    const NOTE_FREQ G = 0x13F;
    const NOTE_FREQ Ab = 0x12D;
    const NOTE_FREQ A = 0x11C;
    const NOTE_FREQ Bb = 0x10C;
    const NOTE_FREQ B = 0xFD;

    typedef enum
    {
        OFF,
        ON
    } COMMAND;

    typedef struct
    {
        NOTE_FREQ note;
        REGISTER octave;
    } NOTE;

    const NOTE_FREQ MIDI_NOTE_MAP[] = {
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

    const NOTE HOLD = {HOLD_FLAG, 0};
    const NOTE REST = {REST_FLAG, 0};

    const uint16_t PITCHBEND_MAX = 0x4000;
    const uint16_t PITCHBEND_MID = 0x2000;
    const uint16_t PITCHBEND_MIN = 0x0000;

    void play(const NOTE note, const AY3::CHANNEL ch = AY3::CHANNEL_A, const COMMAND command = ON, const uint16_t pitchbend = PITCHBEND_MID);
    void play_track(const NOTE *track, const unsigned long bpm, const size_t len, const size_t nchannels);
}

#endif
