#ifndef __AY_3_8910_HPP__
#define __AY_3_8910_HPP__

namespace AY3
{
    typedef enum
    {
        CHANNEL_A = 0X1,
        CHANNEL_B = 0X2,
        CHANNEL_C = 0X4,
        CHANNEL_NOISE = 0X8,
    } CHANNEL;

    void begin();

    // Bus Mode Control
    void bus_mode_inactive();
    void bus_mode_read();
    void bus_mode_write();
    void bus_mode_latch_address();

    // Register/Data Control
    void regset_period(CHANNEL ch, uint16_t data);
    void regset_enable(CHANNEL ch, bool enable);
    void regset_ampltd(CHANNEL ch, uint8_t data, bool envlpe_enable=false);
    void regset_envlpe();
    void regset_eshape();
}
#endif