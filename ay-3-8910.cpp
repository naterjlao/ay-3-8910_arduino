#include <Arduino.h>
#include "include/ay-3-8910.hpp"

#define DEBUG

void AY3::begin()
{
    // Configure 2MHz Clock Signal on Pin 9
    TCCR1A = bit(COM1A0);
    TCCR1B = bit(WGM12) | bit(CS10);
    OCR1A = 3;
    pinMode(9, OUTPUT);

    // Configure Pins 0-7 for Data Registers
    DDRD = 0XFF;  // Set all to output
    PORTD = 0X00; // Set GND to all

    // Configure the A0-A2 Mode/Reset Pins
    // A0 - Reset
    // A1 - BDIR
    // A2 - BC1
    // Note BC2, latched high in hardware
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);

    // Set Bus Mode to Inactive
    bus_mode_inactive();

    // Reset the PSG
    digitalWrite(A0, LOW);
    delay(50);
    digitalWrite(A0, HIGH);
}

//-----------------------------------------------------------------------------
/// @brief Sets the BDIR and BC1 pins to Inactive state.
/// @note BC2 is latched HIGH in hardware.
/// @details Register states are set as follows:
/// Register      | State
/// ------------- | -------
/// BDIR (A1)     |  0
/// BC1 (A2)      |  0
/// BC2 (LATCHED) |  1
//-----------------------------------------------------------------------------
void AY3::bus_mode_inactive()
{
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
}

//-----------------------------------------------------------------------------
/// @brief Sets the BDIR and BC1 pins to Data Read state.
/// @note BC2 is latched HIGH in hardware.
/// @details Register states are set as follows:
/// Register      | State
/// ------------- | -------
/// BDIR (A1)     | 0
/// BC1 (A2)      | 1
/// BC2 (LATCHED) | 1
//-----------------------------------------------------------------------------
void AY3::bus_mode_read()
{
    digitalWrite(A1, LOW);
    digitalWrite(A2, HIGH);
}

//-----------------------------------------------------------------------------
/// @brief Sets the BDIR and BC1 pins to Data Write state.
/// @note BC2 is latched HIGH in hardware.
/// @details Register states are set as follows:
/// Register      | State
/// ------------- | -------
/// BDIR (A1)     | 1
/// BC1 (A2)      | 0
/// BC2 (LATCHED) | 1
//-----------------------------------------------------------------------------
void AY3::bus_mode_write()
{
    digitalWrite(A1, HIGH);
    digitalWrite(A2, LOW);
}

//-----------------------------------------------------------------------------
/// @brief Sets the BDIR and BC1 pins to Latch Address state.
/// @note BC2 is latched HIGH in hardware.
/// @details Register states are set as follows:
/// Register      | State
/// ------------- | -------
/// BDIR (A1)     | 1
/// BC1 (A2)      | 1
/// BC2 (LATCHED) | 1
//-----------------------------------------------------------------------------
void AY3::bus_mode_latch_address()
{
    digitalWrite(A1, HIGH);
    digitalWrite(A2, HIGH);
}

static const uint16_t INVALID_REG = 0XFFFF;

//-----------------------------------------------------------------------------
/// @details Register Mapping is described in the following table.
/// Register | Data Description
/// -------- | ----------------------------------------------
/// R0       | Channel A Tone Period - Fine    (Lower 8-bits)
/// R1       | Channel A Tone Period - Course  (Upper 4-bits)
/// R2       | Channel B Tone Period - Fine    (Lower 8-bits)
/// R3       | Channel B Tone Period - Course  (Upper 4-bits)
/// R4       | Channel C Tone Period - Fine    (Lower 8-bits)
/// R5       | Channel C Tone Period - Course  (Upper 4-bits)
/// R6       | Noise Period (5-bits)
//-----------------------------------------------------------------------------
void AY3::regset_period(AY3::CHANNEL ch, uint16_t data)
{
    const uint16_t UPPER_MASK = 0X0F;
    const uint16_t LOWER_MASK = 0XFF;
    const uint16_t NOISE_MASK = 0X1F;
    uint16_t reg = INVALID_REG;

    // Determine register location
    switch (ch)
    {
    case AY3::CHANNEL_A:
        reg = (01 << 8) | (00 << 0);
        break;

    case AY3::CHANNEL_B:
        reg = (03 << 8) | (02 << 0);
        break;

    case AY3::CHANNEL_C:
        reg = (05 << 8) | (04 << 0);
        break;

    case AY3::CHANNEL_NOISE:
        reg = 06;
        break;

    default:
        reg = INVALID_REG;
        break;
    }

    if (reg != INVALID_REG)
    {
#ifdef DEBUG
        Serial.print("REGSET PERIOD: reg=0x");
        Serial.print(reg, HEX);
#endif
        if (ch != AY3::CHANNEL_NOISE)
        {
            // Write to tone course register
            AY3::bus_mode_inactive();
            AY3::bus_mode_latch_address();
            PORTD = static_cast<uint8_t>(reg >> 8);
#ifdef DEBUG
            Serial.print(", course_addr=0x");
            Serial.print(PORTD, HEX);
#endif
            AY3::bus_mode_inactive();
            PORTD = static_cast<uint8_t>((data >> 8) & UPPER_MASK);
#ifdef DEBUG
            Serial.print(", course_data=0x");
            Serial.print(PORTD, HEX);
#endif
            AY3::bus_mode_write();

            // Write to tone fine register
            AY3::bus_mode_inactive();
            AY3::bus_mode_latch_address();
            PORTD = static_cast<uint8_t>(reg & LOWER_MASK);
#ifdef DEBUG
            Serial.print(", fine_addr=0x");
            Serial.print(PORTD, HEX);
#endif
            AY3::bus_mode_inactive();
            PORTD = static_cast<uint8_t>(data & LOWER_MASK);
#ifdef DEBUG
            Serial.print(", fine_data=0x");
            Serial.print(PORTD, HEX);
#endif
            AY3::bus_mode_write();
            AY3::bus_mode_inactive();
        }
        else
        {
            /// @todo Noise register tone set
        }
#ifdef DEBUG
        Serial.print("\r\n");
#endif
    }
}

void AY3::regset_enable(AY3::CHANNEL ch, uint16_t data)
{
}
void AY3::regset_ampltd(AY3::CHANNEL ch, uint16_t data, bool envlpe_enable)
{
}
void AY3::regset_envlpe(AY3::CHANNEL ch, uint16_t data)
{
}
void AY3::regset_eshape(AY3::CHANNEL ch, uint16_t data)
{
}