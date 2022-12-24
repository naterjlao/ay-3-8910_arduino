#include "include/ay-3-8910.hpp"

void setup()
{
    Serial.begin(9600);

    AY3::begin();
    AY3::regset_period(AY3::CHANNEL_B, 0xABCD);
}

void loop()
{

}