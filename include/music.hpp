#ifndef __MUSIC_HPP__
#define __MUSIC_HPP__

#include <Arduino.h>
#include "ay-3-8910.hpp"

namespace MUSIC
{
    typedef uint16_t NOTE_FREQ;
    typedef uint8_t REGISTER;

    static const REGISTER MIDDLE_OCTAVE = 4;

    const NOTE_FREQ C  	= 0x1DE;
    const NOTE_FREQ Db   = 0x1C3;
    const NOTE_FREQ D  	= 0x1AA;
    const NOTE_FREQ Eb   = 0x192;
    const NOTE_FREQ E  	= 0x17B;
    const NOTE_FREQ F  	= 0x166;
    const NOTE_FREQ Gb   = 0x152;
    const NOTE_FREQ G  	= 0x13F;
    const NOTE_FREQ Ab   = 0x12D;
    const NOTE_FREQ A  	= 0x11C;
    const NOTE_FREQ Bb   = 0x10C;
    const NOTE_FREQ B  	= 0xFD;

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

    void play(NOTE_FREQ note, REGISTER octave=MIDDLE_OCTAVE, COMMAND command=ON, AY3::CHANNEL channel=AY3::CHANNEL_A);
    void play(NOTE note, COMMAND command=ON, AY3::CHANNEL channel=AY3::CHANNEL_A);
}

#endif
