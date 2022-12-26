#include "include/ay-3-8910.hpp"

void setup()
{
    AY3::begin();

    // Enable Output
    AY3::regset_enable((AY3::CHANNEL_A | AY3::CHANNEL_B | AY3::CHANNEL_C), true);

    // Set Amplitude
    AY3::bus_mode_latch_address();
    PORTD = 010;
    AY3::bus_mode_inactive();

    AY3::bus_mode_write();
    PORTD = B00000001;
    AY3::bus_mode_inactive();

    // Set Amplitude
    AY3::bus_mode_latch_address();
    PORTD = 011;
    AY3::bus_mode_inactive();

    AY3::bus_mode_write();
    PORTD = B00000001;
    AY3::bus_mode_inactive();

    // Set Amplitude
    AY3::bus_mode_latch_address();
    PORTD = 012;
    AY3::bus_mode_inactive();

    AY3::bus_mode_write();
    PORTD = B00000001;
    AY3::bus_mode_inactive();
}

void loop()
{
    int idx;
    idx = 0;
    AY3::regset_period(AY3::CHANNEL_C, 0x238);
    while (idx < 4)
    {
        AY3::regset_enable(AY3::CHANNEL_C, idx % 2 == 0);
        AY3::regset_period(AY3::CHANNEL_B, 0x11C);
        delay(300);
        AY3::regset_period(AY3::CHANNEL_B, 0xEF);
        delay(300);
        AY3::regset_period(AY3::CHANNEL_B, 0xBE);
        delay(300);
        idx++;
    }

    idx = 0;
    AY3::regset_period(AY3::CHANNEL_C, 0x27E);
    while (idx < 4)
    {
        AY3::regset_enable(AY3::CHANNEL_C, idx % 2 == 0);
        AY3::regset_period(AY3::CHANNEL_B, 0x13F);
        delay(300);
        AY3::regset_period(AY3::CHANNEL_B, 0xEF);
        delay(300);
        AY3::regset_period(AY3::CHANNEL_B, 0xBE);
        delay(300);
        idx++;
    }

    idx = 0;
    AY3::regset_period(AY3::CHANNEL_C, 0x2A4);
    while (idx < 4)
    {
        AY3::regset_enable(AY3::CHANNEL_C, idx % 2 == 0);
        AY3::regset_period(AY3::CHANNEL_B, 0x152);
        delay(300);
        AY3::regset_period(AY3::CHANNEL_B, 0xEF);
        delay(300);
        AY3::regset_period(AY3::CHANNEL_B, 0xBE);
        delay(300);
        idx++;
    }

    idx = 0;
    AY3::regset_period(AY3::CHANNEL_C, 0x2CC);
    while (idx < 2)
    {
        AY3::regset_enable(AY3::CHANNEL_C, idx % 2 == 0);
        AY3::regset_period(AY3::CHANNEL_B, 0x166);
        delay(300);
        AY3::regset_period(AY3::CHANNEL_B, 0xEF);
        delay(300);
        AY3::regset_period(AY3::CHANNEL_B, 0xBE);
        delay(300);
        idx++;
    }

    idx = 0;
    AY3::regset_period(AY3::CHANNEL_C, 0x2F6);
    while (idx < 2)
    {
        AY3::regset_enable(AY3::CHANNEL_C, idx % 2 == 0);
        AY3::regset_period(AY3::CHANNEL_B, 0x17B);
        delay(300);
        AY3::regset_period(AY3::CHANNEL_B, 0xEF);
        delay(300);
        AY3::regset_period(AY3::CHANNEL_B, 0xBE);
        delay(300);
        idx++;
    }
}