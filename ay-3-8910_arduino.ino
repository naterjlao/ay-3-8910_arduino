#include "include/ay-3-8910.hpp"

void setup()
{
    AY3::begin();

    // Enable
    AY3::bus_mode_latch_address();
    PORTD = 07;
    AY3::bus_mode_inactive();

    AY3::bus_mode_write();
    PORTD = B00111110;
    AY3::bus_mode_inactive();

    // Set Amplitude
    AY3::bus_mode_latch_address();
    PORTD = 010;
    AY3::bus_mode_inactive();

    AY3::bus_mode_write();
    PORTD = B00000001;
    AY3::bus_mode_inactive();
}

void loop()
{
    AY3::regset_period(AY3::CHANNEL_A, 0x11C);
    delay(200);
    AY3::regset_period(AY3::CHANNEL_A, 0x238);
    delay(200);
}