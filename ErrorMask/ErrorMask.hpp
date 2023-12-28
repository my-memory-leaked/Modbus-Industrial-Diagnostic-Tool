#pragma once
#include <Singleton.hpp>
#include <stdint.h>

class ErrorMask : public Singleton<ErrorMask>
{
    friend class Singleton<ErrorMask>;
public:
    /* Fault 1 mask */
    static constexpr uint8_t NO_REACTION = 0b10000000;
    static constexpr uint8_t INTERNAL_ERROR = 0b01000000;
    static constexpr uint8_t TORQUE_FAULT_CLOSE = 0b00100000;
    static constexpr uint8_t TORQUE_FAULT_OPEN = 0b00010000;
    static constexpr uint8_t PHASE_FAILURE = 0b00001000;
    static constexpr uint8_t THERMAL_FAULT = 0b00000100;
    static constexpr uint8_t MAINS_FAULT = 0b00000010;
    static constexpr uint8_t CONFIGURATION_ERROR = 0b00000001;

    /* Fault 2 mask */
    static constexpr uint8_t INCORRECT_PHASE_SEQ = 0b10000000;
    static constexpr uint8_t CONT_ERROR_REMOTE = 0b01000000;
    static constexpr uint8_t INCORRECT_ROTATION = 0b001000000;

private:
    ErrorMask();
    ~ErrorMask();
};
