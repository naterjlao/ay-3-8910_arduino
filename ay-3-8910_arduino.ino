#include "include/ay-3-8910.hpp"
#include "include/music.hpp"

void setup()
{
    AY3::begin();

    // Set Amplitude
    AY3::regset_ampltd(AY3::CHANNEL_A,1);
    AY3::regset_ampltd(AY3::CHANNEL_B,1);
    AY3::regset_ampltd(AY3::CHANNEL_C,2);
}

const MUSIC::NOTE scale[] = {
    {MUSIC::C, 4},
    {MUSIC::D, 4},
    {MUSIC::Eb, 4},
    {MUSIC::F, 4},
    {MUSIC::G, 4},
    {MUSIC::Ab, 4},
    {MUSIC::B, 4},
    {MUSIC::C, 5},
    {MUSIC::D, 5},
    {MUSIC::Eb, 5},
    {MUSIC::F, 5},
    {MUSIC::G, 5},
    {MUSIC::Ab, 5},
    {MUSIC::B, 5},
    {MUSIC::C, 6},
    {MUSIC::B, 5},
    {MUSIC::Ab, 5},
    {MUSIC::G, 5},
    {MUSIC::F, 5},
    {MUSIC::Eb, 5},
    {MUSIC::D, 5},
    {MUSIC::C, 5},
    {MUSIC::B, 4},
    {MUSIC::Ab, 4},
    {MUSIC::G, 4},
    {MUSIC::F, 4},
    {MUSIC::Eb, 4},
    {MUSIC::D, 4}};
const size_t scale_len = sizeof(scale) / sizeof(MUSIC::NOTE);
void loop()
{
    for (size_t idx = 0; idx < scale_len; idx++)
    {
        MUSIC::play(scale[idx],AY3::CHANNEL_A);
        delay(125);
        MUSIC::play(scale[(idx+2)%scale_len],AY3::CHANNEL_B);
        delay(125);
        MUSIC::play(scale[(idx+4)%scale_len],AY3::CHANNEL_C);
        delay(125);
    }
}