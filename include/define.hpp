#pragma once

#include <Arduino.h>

#define PIN_SDA D16
#define PIN_SCL D17

#define PIN_L1 D15
#define PIN_L2 D13
#define PIN_L3 D12
#define PIN_L4 D11
#define PIN_L5 D10
#define PIN_L6 D14

#define PIN_R1 D18
#define PIN_R2 D20
#define PIN_R3 D21
#define PIN_R4 D22
#define PIN_R5 D26
#define PIN_R6 D19

#define PIN_SEL D7
#define PIN_START D6

#define PIN_MODE D0
#define PIN_RAPID D1
#define PIN_SPEED D2

#define PIN_MIDI_3V3 D27

#define KEY_RELEASE_WAIT (10000u)
#define KEY_CANCEL_WAIT (80000u)
#define KEY_TRIGGER_WAIT (100000u)

#define FLASH_TARGET_OFFSET 0x1F0000

typedef struct
{
  uint16_t vid;
  uint16_t pid;
  const char *manufacturer;
  const char *product;
} UsbIdentityProfile;

// USB VID/PID profile IDs used by keyAssign[].usbProfile.
enum
{
  USB_ID_PROFILE_PICO = 0,
  USB_ID_PROFILE_Z,
  USB_ID_PROFILE_8bitDo_SN30Pro,
  USB_ID_PROFILE_DUAL_SHOCK_3,
  USB_ID_PROFILE_HORI_FIGHTING_STICK_V3,
  USB_ID_PROFILE_HORI_SOULCALIBUR_V,
  USB_ID_PROFILE_RETROBIT_GENESIS_6B,
  USB_ID_PROFILE_MAX
};

// USB identity table indexed by USB_ID_PROFILE_*.
const UsbIdentityProfile usbIdentityProfiles[USB_ID_PROFILE_MAX] = {
    {0x2E8A, 0x0003, "Raspberry Pi", "Pico"},                 // USB_ID_PROFILE_PICO
    {0x33DD, 0x0013, "Shining", "X68000 Z JOYPAD"},           // USB_ID_PROFILE_Z
    {0x2DC8, 0x6001, "8BitDo", "8BitDo SN30 Pro"},            // USB_ID_PROFILE_8bitDo_SN30Pro
    {0x054C, 0x0268, "Sony", "PLAYSTATION(R)3 Controller"},   // USB_ID_PROFILE_DUAL_SHOCK_3
    {0x054C, 0x0CE6, "HORI CO.,LTD.", "FIGHTING STICK V3"},   // USB_ID_PROFILE_HORI_FIGHTING_STICK_V3
    {0x24C6, 0x5506, "HORI CO.,LTD.", "SOULCALIBUR V Stick"}, // USB_ID_PROFILE_HORI_SOULCALIBUR_V
    {0x0CA3, 0x0026, "Retro-Bit", "SEGA Genesis Controller"}  // USB_ID_PROFILE_RETROBIT_GENESIS_6B
};

#define JOYKEY_TUD_HID_REPORT_DESC_GAMEPAD(...)                                                            \
  HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),                                                                  \
      HID_USAGE(HID_USAGE_DESKTOP_JOYSTICK),                                                               \
      HID_COLLECTION(HID_COLLECTION_APPLICATION), /* Report ID if any */                                   \
      __VA_ARGS__                                 /* Button Map */                                         \
          HID_LOGICAL_MIN(0x00),                                                                           \
      HID_LOGICAL_MAX(0x01),                                                                               \
      HID_PHYSICAL_MIN(0),                                                                                 \
      HID_PHYSICAL_MAX(1),                                                                                 \
      HID_REPORT_SIZE(1),                                                                                  \
      HID_REPORT_COUNT(16),                                                                                \
      HID_USAGE_PAGE(HID_USAGE_PAGE_BUTTON),                                                               \
      HID_USAGE_MIN(1),                                                                                    \
      HID_USAGE_MAX(16),                                                                                   \
      HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), /* 8 bit X, Y, Z, Rz, Rx, Ry (min 0, max 255 ) */ \
      HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),                                                              \
      HID_USAGE(HID_USAGE_DESKTOP_X),                                                                      \
      HID_USAGE(HID_USAGE_DESKTOP_Y),                                                                      \
      HID_PHYSICAL_MIN(0x00),                                                                              \
      HID_PHYSICAL_MAX_N(0xff, 2),                                                                         \
      HID_LOGICAL_MIN(0x00),                                                                               \
      HID_LOGICAL_MAX_N(0xff, 2),                                                                          \
      HID_REPORT_SIZE(8),                                                                                  \
      HID_REPORT_COUNT(2),                                                                                 \
      HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),                                                   \
      HID_COLLECTION_END

typedef struct
{
  uint8_t buttons[2]; ///< Buttons mask for currently pressed buttons
  uint8_t x;          ///< Delta x  movement of left analog-stick
  uint8_t y;          ///< Delta y  movement of left analog-stick
  //  uint8_t   dpad;
} Joykey_hid_gamepad_report_t;

typedef struct
{
  uint8_t modifier; ///< key modifier
  uint8_t reserved;
  uint8_t led;      ///> LED out
  uint8_t usage[6]; ///> key usage
} my_hid_keyboard_report_t;

// キー表示位置定義用構造体
typedef struct
{
  uint8_t x;
  uint8_t y;
} DrawKeyPos;

// キー表示位置定義
const DrawKeyPos drawKeyPos[] = {
    {52, 54}, {40, 36}, {28, 30}, {16, 34}, {4, 35}, {28, 42}, {68, 54}, {80, 36}, {92, 30}, {104, 34}, {116, 35}, {92, 42}, {52, 24}, {68, 24}};

// JOYKEYの入力キーコード定義
const uint8_t keys[] = {
    PIN_L1, PIN_L2, PIN_L3, PIN_L4, PIN_L5, PIN_L6,
    PIN_R1, PIN_R2, PIN_R3, PIN_R4, PIN_R5, PIN_R6,
    PIN_SEL, PIN_START};

const uint8_t keyChar[] = {
    '^', 'v', '<', '>', 'A', 'B', 'C', '4', 'L', 'R', '7', '8', 's', 'S', 'b', 'c', 'P', 'e', 'f', 'g', '/', '*', '-'};

const uint8_t interval[] = {
    124, // 4
    62,  // 8
    50,  // 10
    40,  // 12
    34,  // 14
    31,  // 16
    27,  // 18
    25   // 20
};

typedef struct
{
  uint8_t mode;
  uint8_t rapidIdx;
  bool rapidKeys[sizeof(keys)];
} ConfigData;

enum
{
  RID_KEYBOARD = 1,
  RID_JOYPAD
};

const uint8_t SIGNATURE[] = "JK0.02";
