#pragma once

#include <Arduino.h>

#define PIN_SDA         D16
#define PIN_SCL         D17

#define PIN_L1          D15
#define PIN_L2          D13
#define PIN_L3          D12
#define PIN_L4          D11
#define PIN_L5          D10
#define PIN_L6          D14

#define PIN_R1          D18
#define PIN_R2          D20
#define PIN_R3          D21
#define PIN_R4          D22
#define PIN_R5          D26
#define PIN_R6          D19

#define PIN_SEL         D7
#define PIN_START       D6

#define PIN_MODE        D0
#define PIN_RAPID       D1
#define PIN_SPEED       D2

#define PIN_MIDI_3V3    D27

#define INPUT_MODE_Z    (00u)
#define INPUT_MODE_W    (01u)

#define KEY_RELEASE_WAIT  (10000u)
#define KEY_CANCEL_WAIT  (80000u)
#define KEY_TRIGGER_WAIT  (100000u)

#define KEY_ASSIGN_NUM  (15u)

#define FLASH_TARGET_OFFSET         0x1F0000

typedef enum {
  INPUT_UP = 0,
  INPUT_DOWN,
  INPUT_LEFT,
  INPUT_RIGHT,
  INPUT_TRG_A,
  INPUT_TRG_B,
  INPUT_TRG_C,
  INPUT_03,
  INPUT_L1,
  INPUT_R1,
  INPUT_06,
  INPUT_07,
  INPUT_SELECT,
  INPUT_START,
  INPUT_10,
  INPUT_11,
  INPUT_12,
  INPUT_13,
  INPUT_14,
  INPUT_15,
  INPUT_CANCEL,
  INPUT_BLAST,
  INPUT_NOP,
  INPUT_MAX
} INPUT_KEYS;

#define MY_TUD_HID_REPORT_DESC_GAMEPAD(...) \
    HID_USAGE_PAGE ( HID_USAGE_PAGE_DESKTOP     )                 ,\
    HID_USAGE      ( HID_USAGE_DESKTOP_JOYSTICK  )                 ,\
    HID_COLLECTION ( HID_COLLECTION_APPLICATION )                 ,\
        /* Report ID if any */\
        __VA_ARGS__ \
        /* Button Map */ \
        HID_LOGICAL_MIN    ( 0x00                                   ) ,\
        HID_LOGICAL_MAX    ( 0x01                                   ) ,\
        HID_PHYSICAL_MIN   ( 0                                      ) ,\
        HID_PHYSICAL_MAX   ( 1                                      ) ,\
        HID_REPORT_SIZE    ( 1                                      ) ,\
        HID_REPORT_COUNT   ( 12                                     ) ,\
        HID_USAGE_PAGE     ( HID_USAGE_PAGE_BUTTON                  ) ,\
        HID_USAGE_MIN      ( 1                                      ) ,\
        HID_USAGE_MAX      ( 12                                     ) ,\
        HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
        HID_REPORT_COUNT   ( 4                                      ) ,\
        HID_INPUT          ( HID_CONSTANT | HID_ARRAY | HID_ABSOLUTE ) ,\
        /* 8 bit X, Y, Z, Rz, Rx, Ry (min 0, max 255 ) */ \
        HID_USAGE_PAGE     ( HID_USAGE_PAGE_DESKTOP                 ) ,\
        HID_USAGE          ( HID_USAGE_DESKTOP_X                    ) ,\
        HID_USAGE          ( HID_USAGE_DESKTOP_Y                    ) ,\
        HID_PHYSICAL_MIN   ( 0x00                                   ) ,\
        HID_PHYSICAL_MAX_N ( 0xff, 2                                ) ,\
        HID_LOGICAL_MIN    ( 0x00                                   ) ,\
        HID_LOGICAL_MAX_N  ( 0xff, 2                                ) ,\
        HID_REPORT_SIZE    ( 8                                      ) ,\
        HID_REPORT_COUNT   ( 2                                      ) ,\
        HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
  HID_COLLECTION_END


typedef struct MY_TU_ATTR_PACKED
{
  uint8_t   buttons[2];   ///< Buttons mask for currently pressed buttons
  uint8_t   x;            ///< Delta x  movement of left analog-stick
  uint8_t   y;            ///< Delta y  movement of left analog-stick
//  uint8_t   dpad;
}my_hid_gamepad_report_t;

typedef struct {
  uint8_t x;
  uint8_t y;
} DrawKeyPos;


const uint8_t keys[] = {
    PIN_L1, PIN_L2, PIN_L3, PIN_L4, PIN_L5, PIN_L6,
    PIN_R1, PIN_R2, PIN_R3, PIN_R4, PIN_R5, PIN_R6,
    PIN_SEL, PIN_START
};

const uint8_t keyChar[] = {
    '^','v','<','>','A','B','C','4','L','R','7','8','s','S','b','c','P','e','f','g','/','*','-'
};

const uint8_t interval[] = {
    124,    // 4
    62,     // 8
    31,     // 16
    25      // 20
};

const uint8_t bitmap_z[] = 
{
    0b01111111, 0b11111111,
    0b11111111, 0b11111110,
    0b00000001, 0b11111100,
    0b00000011, 0b11111000,

    0b00000111, 0b11110000,
    0b00000000, 0b00000000,
    0b00011111, 0b11000000,
    0b00000000, 0b00000000,
    0b00111111, 0b10000000,

    0b01111111, 0b00000000,
    0b11111111, 0b11111111,
    0b11111111, 0b11111110
};

typedef struct {
    uint8_t mode;
    uint8_t rapidIdx;
    bool rapidKeys[sizeof(keys)];
} CONFIG_DATA;

const uint8_t SIGNATURE[] = "JK0.02";
