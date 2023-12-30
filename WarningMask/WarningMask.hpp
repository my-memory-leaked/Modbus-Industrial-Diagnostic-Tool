#pragma once
#include <Singleton.hpp>
#include <stdint.h>

class WarningMask : public Singleton<WarningMask>
{
    friend class Singleton<WarningMask>;
public:
    // Byte 17: Warnings 1
    static constexpr uint8_t NO_REACTION = 0b10000000;
    static constexpr uint8_t SIL_FAULT = 0b01000000;
    static constexpr uint8_t TORQUE_WARN_OPEN = 0b00100000;
    static constexpr uint8_t TORQUE_WARN_CLOSE = 0b00010000;
    static constexpr uint8_t FQM_STATE_FAULT = 0b00000100;
    static constexpr uint8_t MAINTENANCE_REQUIRED = 0b00000000;

    // Byte 18: Warnings 2
    static constexpr uint8_t CONFIG_WARNING = 0b10000000;
    static constexpr uint8_t RTC_NOT_SET = 0b01000000;
    static constexpr uint8_t RTC_BUTTON_CELL = 0b00100000;
    static constexpr uint8_t DC_EXTERNAL = 0b00000100;
    static constexpr uint8_t CONTROLS_TEMP = 0b00000000;

    // Byte 19: Warnings 3
    static constexpr uint8_t OP_TIME_WARNING = 0b10000000;
    static constexpr uint8_t WRM_RUNNING = 0b01000000;
    static constexpr uint8_t WRM_STARTS = 0b00100000;
    static constexpr uint8_t INTERNAL_WARNING = 0b00010000;
    static constexpr uint8_t INPUT_AIN1 = 0b00001000;
    static constexpr uint8_t INPUT_AIN2 = 0b00000100;
    static constexpr uint8_t WRM_FOC_CABLE_BUDGET = 0b00000010;
    static constexpr uint8_t FO_CABLE_BUDEGT = 0b00000000;

    // Byte 20: Warnings 4
    static constexpr uint8_t PVST_FAULT = 0b10000000;
    static constexpr uint8_t PVST_ABORT = 0b01000000;
    static constexpr uint8_t FAILURE_BEHAV_ACTIVE = 0b00100000;
    static constexpr uint8_t FOC_CONNECTION_REQUIRED = 0b00001000;
    static constexpr uint8_t PVST_REQUIRED = 0b00000100;
    static constexpr uint8_t SETPOINT_POS = 0b00000010;


private:
    WarningMask();
    ~WarningMask();
};
