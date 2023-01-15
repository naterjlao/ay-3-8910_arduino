#include <Arduino.h>
#include "include/ay-3-8910.hpp"

static const int RESET_PIN = 8;
static const int BC1_PIN = A5;
static const int BDIR_PIN = A4;

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

    // Set Bus Mode to Inactive
    bus_mode_inactive();

    // Configure the A0-A5 Mode/Reset Pins
    // 8 - Reset
    // A4 - BDIR
    // A5 - BC1
    // Note BC2, latched high in hardware
    pinMode(RESET_PIN, OUTPUT);
    pinMode(BDIR_PIN, OUTPUT);
    pinMode(BC1_PIN, OUTPUT);

    // Reset the PSG
    digitalWrite(RESET_PIN, LOW);
    delay(1);
    digitalWrite(RESET_PIN, HIGH);
}

//-----------------------------------------------------------------------------
/// @brief Sets the BDIR and BC1 pins to Inactive state.
/// @note BC2 is latched HIGH in hardware.
/// @details Register states are set as follows:
/// Register      | State
/// ------------- | -------
/// BDIR (A4)     |  0
/// BC1 (A5)      |  0
/// BC2 (LATCHED) |  1
//-----------------------------------------------------------------------------
void AY3::bus_mode_inactive()
{
    PORTC = B00000000 | (PORTC & B11001111);
}

//-----------------------------------------------------------------------------
/// @brief Sets the BDIR and BC1 pins to Data Read state.
/// @note BC2 is latched HIGH in hardware.
/// @details Register states are set as follows:
/// Register      | State
/// ------------- | -------
/// BDIR (A4)     | 0
/// BC1 (A5)      | 1
/// BC2 (LATCHED) | 1
//-----------------------------------------------------------------------------
void AY3::bus_mode_read()
{
    PORTC = B00100000 | (PORTC & B11001111);
}

//-----------------------------------------------------------------------------
/// @brief Sets the BDIR and BC1 pins to Data Write state.
/// @note BC2 is latched HIGH in hardware.
/// @details Register states are set as follows:
/// Register      | State
/// ------------- | -------
/// BDIR (A4)     | 1
/// BC1 (A5)      | 0
/// BC2 (LATCHED) | 1
//-----------------------------------------------------------------------------
void AY3::bus_mode_write()
{
    PORTC = B00010000 | (PORTC & B11001111);
}

//-----------------------------------------------------------------------------
/// @brief Sets the BDIR and BC1 pins to Latch Address state.
/// @note BC2 is latched HIGH in hardware.
/// @details Register states are set as follows:
/// Register      | State
/// ------------- | -------
/// BDIR (A4)     | 1
/// BC1 (A5)      | 1
/// BC2 (LATCHED) | 1
//-----------------------------------------------------------------------------
void AY3::bus_mode_latch_address()
{
    PORTC = B00110000 | (PORTC & B11001111);
}

static const uint8_t INVALID_REG = 0XFF;
static const uint16_t INVALID_REG16 = 0XFFFF;

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
    uint16_t reg = INVALID_REG16;

    // Determine Register Address
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
        reg = INVALID_REG16;
        break;
    }

    if (reg != INVALID_REG16)
    {
        if (ch != AY3::CHANNEL_NOISE)
        {
            // ----- TONE COURSE ----- //
            // Register Address
            AY3::bus_mode_latch_address();
            PORTD = static_cast<uint8_t>(reg >> 8);
            AY3::bus_mode_inactive();

            // Data Value
            AY3::bus_mode_write();
            PORTD = static_cast<uint8_t>((data >> 8) & UPPER_MASK);
            AY3::bus_mode_inactive();

            // ----- TONE FINE ----- //
            // Register Address
            AY3::bus_mode_latch_address();
            PORTD = static_cast<uint8_t>(reg & LOWER_MASK);
            AY3::bus_mode_inactive();

            // Data Value
            AY3::bus_mode_write();
            PORTD = static_cast<uint8_t>(data & LOWER_MASK);
            AY3::bus_mode_inactive();
        }
        else
        {
            /// @todo Noise register tone set
        }
    }
}

void AY3::regset_enable(AY3::CHANNEL ch, bool enable)
{
    const uint8_t reg = 07;     // Register address
    static uint8_t data = 0XFF; // Stores the current state of the register
    uint8_t mask = B00111000;   /** @note Noise INOP - change to 0X0 when ready*/
    uint8_t setb = B00111000;

    if (ch & CHANNEL_A)
    {
        mask |= B00000001;
        setb |= enable ? B00000000 : B00000001;
    }

    if (ch & CHANNEL_B)
    {
        mask |= B00000010;
        setb |= enable ? B00000000 : B00000010;
    }

    if (ch & CHANNEL_C)
    {
        mask |= B00000100;
        setb |= enable ? B00000000 : B00000100;
    }

    // Set to Enable Address
    AY3::bus_mode_latch_address();
    PORTD = 07;
    AY3::bus_mode_inactive();

    // Only change the requested bit
    data = setb | (data & (~mask));

    // Write update to register
    AY3::bus_mode_write();
    PORTD = data;
    AY3::bus_mode_inactive();
}
void AY3::regset_ampltd(AY3::CHANNEL ch, uint16_t data, bool envlpe_enable)
{
    const uint8_t AMPLITUDE_MASK = 0X0F;
    uint8_t reg = INVALID_REG;

    switch (ch)
    {
    case CHANNEL_A:
        reg = 010;
        break;
    case CHANNEL_B:
        reg = 011;
        break;
    case CHANNEL_C:
        reg = 012;
        break;
    default:
        reg = INVALID_REG;
        break;
    }

    /// @todo envelope enable

    if (reg != INVALID_REG)
    {
        AY3::bus_mode_latch_address();
        PORTD = reg;
        AY3::bus_mode_inactive();

        AY3::bus_mode_write();
        PORTD = data & AMPLITUDE_MASK;
        AY3::bus_mode_inactive();
    }
}
void AY3::regset_envlpe(AY3::CHANNEL ch, uint16_t data)
{
}
void AY3::regset_eshape(AY3::CHANNEL ch, uint16_t data)
{
}