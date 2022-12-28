#include "include/ay-3-8910.hpp"
#include "include/music.hpp"

using namespace MUSIC;

#define NCHANNELS 3
#define BPM 107
#define NTRACKS 8
#define TRACK_LEN 16

// WHAT YOU KNOW?
const NOTE track[NTRACKS][NCHANNELS][TRACK_LEN] = {
{
    // 1                    |                       |                       |           
    { REST, REST,{Db,5},HOLD,{E,5},HOLD,{Gb,5}, HOLD, {E,5},HOLD,REST,{Db,5}, HOLD, REST, {Db,5},HOLD},
    { REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST},
    {{Gb,2},HOLD,REST,{Gb,2}, REST, REST,{Gb,2},REST,{Gb,2},HOLD,REST,{Gb,2}, REST, REST,{Gb,2},REST}
},
{
    // 2                    |                       |                       |           
    { REST, REST,{Db,5},HOLD,{E,5},HOLD,{Gb,5}, HOLD, {E,5},HOLD,REST,{B,4}, HOLD, REST, {Db,5},HOLD},
    { REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST},
    {{Gb,2},HOLD,REST,{Gb,2}, REST, REST,{Gb,2},REST,{Db,3},HOLD, HOLD, REST,{Ab,2},HOLD,HOLD, HOLD}
},
{
    // 3                    |                       |                       |           
    { REST, REST,{Db,5},HOLD,{E,5},HOLD,{Gb,5}, HOLD, {E,5},HOLD,REST,{Db,5}, HOLD, REST, {Db,5},HOLD},
    { REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST},
    {{A,2}, HOLD, REST,{A,2}, REST, REST,{A,2}, REST,{A,2}, HOLD, REST, {A,2},REST, REST,{A,2}, REST}
},
{
    // 4                    |                       |                       |           
    {{A,5}, HOLD,HOLD,{Ab,5}, HOLD, HOLD,{Db,5},REST,{A,5}, HOLD,HOLD,{Ab,5}, HOLD, HOLD,{B,4}, HOLD},
    {{Gb,5}, HOLD,HOLD,{E,5}, HOLD, HOLD,{Ab,4},REST,{Gb,5}, HOLD,HOLD,{E,5}, HOLD, HOLD,{Ab,4},HOLD},
    {{A,2}, HOLD, REST,{A,2}, REST, REST,{A,2}, REST,{B,2},HOLD, HOLD, REST, {Db,2},HOLD,HOLD, HOLD}
},
{
    // 5                    |                       |                       |           
    { REST, REST,{Db,5},HOLD,{E,5},HOLD,{Gb,5}, HOLD, {E,5},HOLD,REST,{Db,5}, HOLD, REST, {Db,5},HOLD},
    { REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST},
    {{D,2}, HOLD, REST,{D,2}, REST ,REST,{D,2}, REST, {D,2},HOLD, REST,{D,2}, REST, REST,{D,2}, REST} 
},
{
    // 6                    |                       |                       |           
    { REST, REST,{Db,5},HOLD,{E,5},HOLD,{Gb,5}, HOLD, {E,5},HOLD,REST,{B,4}, HOLD, REST, {Db,5},HOLD},
    { REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST},
    {{D,2}, HOLD, REST,{D,2}, REST ,REST,{D,2}, REST, {A,2},HOLD, HOLD, REST,{E,2}, HOLD, HOLD, HOLD}
},
{
    // 7                    |                       |                       |           
    { REST, REST,{Db,5},HOLD,{E,5},HOLD,{Gb,5}, HOLD, {E,5},HOLD,REST,{Db,5}, HOLD, REST, {Db,5},HOLD},
    { REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST},
    {{Gb,2},HOLD,REST,{Gb,2}, REST, REST,{Gb,2},REST,{Gb,2},HOLD,REST,{Gb,2}, REST, REST,{Gb,2},REST}
},
{
    // 8                    |                       |                       |           
    {{A,5}, HOLD,HOLD,{Ab,5}, HOLD, HOLD,{Db,5},REST,{A,5}, HOLD,HOLD,  REST, {Ab,5},HOLD, HOLD,REST},
    {{Gb,5}, HOLD,HOLD,{E,5}, HOLD, HOLD,{Ab,4},REST,{Gb,5}, HOLD,HOLD, REST, {E,5}, HOLD, HOLD,REST},
    {{Gb,2},HOLD,REST,{Gb,2}, REST, REST,{Gb,2},REST,{Gb,2},HOLD, REST, REST, {Gb,2},HOLD,REST, REST}
}
};

void setup()
{
    AY3::begin();

    // Set Amplitude
    AY3::regset_ampltd(AY3::CHANNEL_A,2);
    AY3::regset_ampltd(AY3::CHANNEL_B,1);
    AY3::regset_ampltd(AY3::CHANNEL_C,2);
}

void loop()
{
    for (size_t track_idx = 0; track_idx < NTRACKS; track_idx++)
        MUSIC::play_track((const NOTE *) &track[track_idx],BPM,TRACK_LEN,NCHANNELS);
}