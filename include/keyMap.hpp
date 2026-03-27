#pragma once

#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

#define KEY_ASSIGN_NUM (14u)
#define KEY_ASSIGN_KEY_NUM (6u)

enum
{
  INPUT_MODE_Z = 0,
  INPUT_MODE_W,
  INPUT_MODE_ROGUE,
  INPUT_MODE_GRADIUS,
  INPUT_MODE_LAGOON,
  INPUT_MODE_NEMESIS,
};

// キー出力デバイス種別
typedef enum
{
  DVC_KEY,
  DVC_PAD,
} Device;

// USBデバイス種別
typedef enum
{
  USB_DVC_KEY,
  USB_DVC_PAD,
  USB_DVC_COMPOSITE
} UsbDevice;

// プリセット用内部キーコード
typedef enum
{
  K_NOP = 0,
  K_PD_UP,         // JOYPAD UP
  K_PD_DOWN,       // JOYPAD DOWN
  K_PD_LEFT,       // JOYPAD LEFT
  K_PD_RIGHT,      // JOYPAD RIGHT
  K_PD_TRG_A,      // JOYPAD TRG A
  K_PD_TRG_B,      // JOYPAD TRG B
  K_PD_TRG_C,      // JOYPAD TRG C
  K_PD_04,         // JOYPAD TRG 04
  K_PD_L1,         // JOYPAD L1
  K_PD_R1,         // JOYPAD R1
  K_PD_07,         // JOYPAD TRG 07
  K_PD_08,         // JOYPAD TRG 08
  K_PD_SELECT,     // JOYPAD SELECT
  K_PD_START,      // JOYPAD START
  K_PD_11,         // JOYPAD TRG 11
  K_PD_12,         // JOYPAD TRG 12
  K_PD_CANCEL,     // JOYPAD FUNC CANCEL
  K_PD_BLAST,      // JOYPAD FUNC BLAST FOR GRANADA
  K_KB_ESC,        // KB ESC
  K_KB_1,          // KB 1
  K_KB_2,          // KB 2
  K_KB_3,          // KB 3
  K_KB_4,          // KB 4
  K_KB_5,          // KB 5
  K_KB_6,          // KB 6
  K_KB_7,          // KB 7
  K_KB_8,          // KB 8
  K_KB_9,          // KB 9
  K_KB_0,          // KB 0
  K_KB_A,          // KB A
  K_KB_B,          // KB B
  K_KB_C,          // KB C
  K_KB_D,          // KB D
  K_KB_E,          // KB E
  K_KB_F,          // KB F
  K_KB_G,          // KB G
  K_KB_H,          // KB H
  K_KB_I,          // KB I
  K_KB_J,          // KB J
  K_KB_K,          // KB K
  K_KB_L,          // KB L
  K_KB_M,          // KB M
  K_KB_N,          // KB N
  K_KB_O,          // KB O
  K_KB_P,          // KB P
  K_KB_Q,          // KB Q
  K_KB_R,          // KB R
  K_KB_S,          // KB S
  K_KB_T,          // KB T
  K_KB_U,          // KB U
  K_KB_V,          // KB V
  K_KB_W,          // KB W
  K_KB_X,          // KB X
  K_KB_Y,          // KB Y
  K_KB_Z,          // KB Z
  K_KB_MINUS,      // KB -
  K_KB_PLUS,       // KB +
  K_KB_EQUAL,      // KB =
  K_KB_AT,         // KB @
  K_KB_ASTERISK,   // KB *
  K_KB_COLON,      // KB :
  K_KB_SEMICOLON,  // KB ;
  K_KB_GT,         // KB >
  K_KB_LT,         // KB <
  K_KB_DOT,        // KB .
  K_KB_COMMA,      // KB ,
  K_KB_LT_SQ_BRKT, // KB [
  K_KB_RT_SQ_BRKT, // KB ]
  K_KB_F1,         // KB F1
  K_KB_F2,         // KB F2
  K_KB_F3,         // KB F3
  K_KB_F4,         // KB F4
  K_KB_F5,         // KB F5
  K_KB_F6,         // KB F6
  K_KB_F7,         // KB F7
  K_KB_F8,         // KB F8
  K_KB_F9,         // KB F9
  K_KB_F10,        // KB F10
  K_KB_OPT1,       // KB OPT1
  K_KB_OPT2,       // KB OPT2
  K_KB_UP,         // KB UP
  K_KB_DOWN,       // KB DOWN
  K_KB_LEFT,       // KB LEFT
  K_KB_RIGHT,      // KB RIGHT
  K_KB_TENKEY_1,   // KB TENKEY 1
  K_KB_TENKEY_2,   // KB TENKEY 2
  K_KB_TENKEY_3,   // KB TENKEY 3
  K_KB_TENKEY_4,   // KB TENKEY 4
  K_KB_TENKEY_5,   // KB TENKEY 5
  K_KB_TENKEY_6,   // KB TENKEY 6
  K_KB_TENKEY_7,   // KB TENKEY 7
  K_KB_TENKEY_8,   // KB TENKEY 8
  K_KB_TENKEY_9,   // KB TENKEY 9
  K_KB_TENKEY_0,   // KB TENKEY 0
  K_KB_SPACE,      // KB SPACE
  K_KB_ENTER,      // KB ENTER
  K_KB_TAB,        // KB TAB
  K_KB_GRAVE,      // KB GRAVE (X68k BREAK/JIS 全角)
  K_KB_LEFT_ALT,   // KB LEFT ALT (X68k XF1)
  K_KB_MUHENKAN,   // KB 無変換 (X68k XF2)
  K_KB_SHIFT,      // KB SHIFT
  K_MAX
} InternalKeyDef;

// JOYPAD出力キーコード
#define OUT_PAD_UP (0x00)     // JOYPAD UP
#define OUT_PAD_DOWN (0x01)   // JOYPAD DOWN
#define OUT_PAD_LEFT (0x02)   // JOYPAD LEFT
#define OUT_PAD_RIGHT (0x03)  // JOYPAD RIGHT
#define OUT_PAD_TRG_A (0x04)  // JOYPAD TRG A
#define OUT_PAD_TRG_B (0x05)  // JOYPAD TRG B
#define OUT_PAD_TRG_C (0x06)  // JOYPAD TRG C
#define OUT_PAD_04 (0x07)     // JOYPAD TRG 04
#define OUT_PAD_L1 (0x08)     // JOYPAD L1
#define OUT_PAD_R1 (0x09)     // JOYPAD R1
#define OUT_PAD_07 (0x0A)     // JOYPAD TRG 06
#define OUT_PAD_08 (0x0B)     // JOYPAD TRG 07
#define OUT_PAD_SELECT (0x0C) // JOYPAD SELECT
#define OUT_PAD_START (0x0D)  // JOYPAD START
#define OUT_PAD_11 (0x0E)     // JOYPAD TRG 10
#define OUT_PAD_12 (0x0F)     // JOYPAD TRG 11
#define OUT_PAD_CANCEL (0x10) // JOYPAD FUNC CANCEL
#define OUT_PAD_BLAST (0x11)  // JOYPAD FUNC BLAST FOR GRANADA
#define OUT_PAD_NOP (0x12)    // JOYPAD NOP
#define OUT_PAD_MAX (0x13)

// プリセット用内部キーコード→出力先定義用構造体
typedef struct
{
  Device device;    // 出力デバイス
  uint8_t modifier; // キーボード出力modifier
  uint8_t usage;    // 出力コード
  uint8_t disp;     // 画面表示文字
} KeyDef;

#define MODIFIER_NONE 0

// プリセット用内部キーコード→出力先定義
KeyDef keyDefTbl[] = {
    /* K_NOP */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_NOP, ' '},                              // JOYPAD UP
    /* K_PD_UP */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_UP, '^'},                             // JOYPAD UP
    /* K_PD_DOWN */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_DOWN, 'v'},                         // JOYPAD DOWN
    /* K_PD_LEFT */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_LEFT, '<'},                         // JOYPAD LEFT
    /* K_PD_RIGHT */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_RIGHT, '>'},                       // JOYPAD RIGHT
    /* K_PD_TRG_A */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_TRG_A, 'A'},                       // JOYPAD TRG A
    /* K_PD_TRG_B */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_TRG_B, 'B'},                       // JOYPAD TRG B
    /* K_PD_TRG_C */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_TRG_C, 'C'},                       // JOYPAD TRG C
    /* K_PD_04 */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_04, '4'},                             // JOYPAD TRG 4
    /* K_PD_L1 */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_L1, 'L'},                             // JOYPAD L1
    /* K_PD_R1 */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_R1, 'R'},                             // JOYPAD R1
    /* K_PD_07 */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_07, '7'},                             // JOYPAD TRG 7
    /* K_PD_08 */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_08, '8'},                             // JOYPAD TRG 8
    /* K_PD_SELECT */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_SELECT, '?'},                     // JOYPAD SELECT
    /* K_PD_START */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_START, '!'},                       // JOYPAD START
    /* K_PD_CANCEL */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_CANCEL, '#'},                     // JOYPAD CANCEL
    /* K_PD_11 */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_11, '1'},                             // JOYPAD TRG 4
    /* K_PD_12 */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_12, '2'},                             // JOYPAD TRG 4
    /* K_PD_BLAST */ {DVC_PAD, MODIFIER_NONE, OUT_PAD_BLAST, '*'},                       // JOYPAD BLAST
    /* K_KB_ESC */ {DVC_KEY, MODIFIER_NONE, HID_KEY_ESCAPE, 'e'},                        // KB ESC
    /* K_KB_1 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_1, '1'},                               // KB 1
    /* K_KB_2 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_2, '2'},                               // KB 2
    /* K_KB_3 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_3, '3'},                               // KB 3
    /* K_KB_4 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_4, '4'},                               // KB 4
    /* K_KB_5 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_5, '5'},                               // KB 5
    /* K_KB_6 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_6, '6'},                               // KB 6
    /* K_KB_7 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_7, '7'},                               // KB 7
    /* K_KB_8 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_8, '8'},                               // KB 8
    /* K_KB_9 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_9, '9'},                               // KB 9
    /* K_KB_0 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_0, '0'},                               // KB 0
    /* K_KB_A */ {DVC_KEY, MODIFIER_NONE, HID_KEY_A, 'A'},                               // KB A
    /* K_KB_B */ {DVC_KEY, MODIFIER_NONE, HID_KEY_B, 'B'},                               // KB B
    /* K_KB_C */ {DVC_KEY, MODIFIER_NONE, HID_KEY_C, 'C'},                               // KB C
    /* K_KB_D */ {DVC_KEY, MODIFIER_NONE, HID_KEY_D, 'D'},                               // KB D
    /* K_KB_E */ {DVC_KEY, MODIFIER_NONE, HID_KEY_E, 'E'},                               // KB E
    /* K_KB_F */ {DVC_KEY, MODIFIER_NONE, HID_KEY_F, 'F'},                               // KB F
    /* K_KB_G */ {DVC_KEY, MODIFIER_NONE, HID_KEY_G, 'G'},                               // KB G
    /* K_KB_H */ {DVC_KEY, MODIFIER_NONE, HID_KEY_H, 'H'},                               // KB H
    /* K_KB_I */ {DVC_KEY, MODIFIER_NONE, HID_KEY_I, 'I'},                               // KB I
    /* K_KB_J */ {DVC_KEY, MODIFIER_NONE, HID_KEY_J, 'J'},                               // KB J
    /* K_KB_K */ {DVC_KEY, MODIFIER_NONE, HID_KEY_K, 'K'},                               // KB K
    /* K_KB_L */ {DVC_KEY, MODIFIER_NONE, HID_KEY_L, 'L'},                               // KB L
    /* K_KB_M */ {DVC_KEY, MODIFIER_NONE, HID_KEY_M, 'M'},                               // KB M
    /* K_KB_N */ {DVC_KEY, MODIFIER_NONE, HID_KEY_N, 'N'},                               // KB N
    /* K_KB_O */ {DVC_KEY, MODIFIER_NONE, HID_KEY_O, 'O'},                               // KB O
    /* K_KB_P */ {DVC_KEY, MODIFIER_NONE, HID_KEY_P, 'P'},                               // KB P
    /* K_KB_Q */ {DVC_KEY, MODIFIER_NONE, HID_KEY_Q, 'Q'},                               // KB Q
    /* K_KB_R */ {DVC_KEY, MODIFIER_NONE, HID_KEY_R, 'R'},                               // KB R
    /* K_KB_S */ {DVC_KEY, MODIFIER_NONE, HID_KEY_S, 'S'},                               // KB S
    /* K_KB_T */ {DVC_KEY, MODIFIER_NONE, HID_KEY_T, 'T'},                               // KB T
    /* K_KB_U */ {DVC_KEY, MODIFIER_NONE, HID_KEY_U, 'U'},                               // KB U
    /* K_KB_V */ {DVC_KEY, MODIFIER_NONE, HID_KEY_V, 'V'},                               // KB V
    /* K_KB_W */ {DVC_KEY, MODIFIER_NONE, HID_KEY_W, 'W'},                               // KB W
    /* K_KB_X */ {DVC_KEY, MODIFIER_NONE, HID_KEY_X, 'X'},                               // KB X
    /* K_KB_Y */ {DVC_KEY, MODIFIER_NONE, HID_KEY_Y, 'Y'},                               // KB Y
    /* K_KB_Z */ {DVC_KEY, MODIFIER_NONE, HID_KEY_Z, 'Z'},                               // KB Z
    /* K_KB_MINUS */ {DVC_KEY, MODIFIER_NONE, HID_KEY_MINUS, '-'},                       // KB -
    /* K_KB_PLUS */ {DVC_KEY, KEYBOARD_MODIFIER_LEFTSHIFT, HID_KEY_SEMICOLON, '+'},      // KB +
    /* K_KB_EQUAL */ {DVC_KEY, KEYBOARD_MODIFIER_LEFTSHIFT, HID_KEY_MINUS, '='},         // KB =
    /* K_KB_AT */ {DVC_KEY, MODIFIER_NONE, HID_KEY_BRACKET_LEFT, '@'},                   // KB @
    /* K_KB_ASTERISK */ {DVC_KEY, KEYBOARD_MODIFIER_LEFTSHIFT, HID_KEY_APOSTROPHE, '*'}, // KB *
    /* K_KB_COLON */ {DVC_KEY, MODIFIER_NONE, HID_KEY_APOSTROPHE, ':'},                  // KB :
    /* K_KB_SEMICOLON */ {DVC_KEY, MODIFIER_NONE, HID_KEY_SEMICOLON, ';'},               // KB ;
    /* K_KB_GT */ {DVC_KEY, KEYBOARD_MODIFIER_LEFTSHIFT, HID_KEY_PERIOD, '>'},           // KB >
    /* K_KB_LT */ {DVC_KEY, KEYBOARD_MODIFIER_LEFTSHIFT, HID_KEY_COMMA, '<'},            // KB <
    /* K_KB_DOT */ {DVC_KEY, MODIFIER_NONE, HID_KEY_PERIOD, '.'},                        // KB .
    /* K_KB_COMMA */ {DVC_KEY, MODIFIER_NONE, HID_KEY_COMMA, ','},                       // KB ,
    /* K_KB_LT_SQ_BRKT */ {DVC_KEY, MODIFIER_NONE, HID_KEY_BRACKET_RIGHT, '['},          // KB [
    /* K_KB_RT_SQ_BRKT */ {DVC_KEY, MODIFIER_NONE, HID_KEY_EUROPE_1, ']'},               // KB ]
    /* K_KB_F1 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_F1, '1'},                             // KB F1
    /* K_KB_F2 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_F2, '2'},                             // KB F2
    /* K_KB_F3 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_F3, '3'},                             // KB F3
    /* K_KB_F4 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_F4, '4'},                             // KB F4
    /* K_KB_F5 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_F5, '5'},                             // KB F5
    /* K_KB_F6 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_F6, '6'},                             // KB F6
    /* K_KB_F7 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_F7, '7'},                             // KB F7
    /* K_KB_F8 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_F8, '8'},                             // KB F8
    /* K_KB_F9 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_F9, '9'},                             // KB F9
    /* K_KB_F10 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_F10, '0'},                           // KB F10
    /* K_KB_OPT1 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_F11, '1'},                          // KB OPT1
    /* K_KB_OPT2 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_F12, '2'},                          // KB OPT2
    /* K_KB_UP */ {DVC_KEY, MODIFIER_NONE, HID_KEY_ARROW_UP, '^'},                       // KB UP
    /* K_KB_DOWN */ {DVC_KEY, MODIFIER_NONE, HID_KEY_ARROW_DOWN, 'v'},                   // KB DOWN
    /* K_KB_LEFT */ {DVC_KEY, MODIFIER_NONE, HID_KEY_ARROW_LEFT, '<'},                   // KB LEFT
    /* K_KB_RIGHT */ {DVC_KEY, MODIFIER_NONE, HID_KEY_ARROW_RIGHT, '>'},                 // KB RIGHT
    /* K_KB_TENKEY_1 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_KEYPAD_1, '1'},                 // KB TENKEY 1
    /* K_KB_TENKEY_2 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_KEYPAD_2, '2'},                 // KB TENKEY 2
    /* K_KB_TENKEY_3 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_KEYPAD_3, '3'},                 // KB TENKEY 3
    /* K_KB_TENKEY_4 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_KEYPAD_4, '4'},                 // KB TENKEY 4
    /* K_KB_TENKEY_5 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_KEYPAD_5, '5'},                 // KB TENKEY 5
    /* K_KB_TENKEY_6 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_KEYPAD_6, '6'},                 // KB TENKEY 6
    /* K_KB_TENKEY_7 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_KEYPAD_7, '7'},                 // KB TENKEY 7
    /* K_KB_TENKEY_8 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_KEYPAD_8, '8'},                 // KB TENKEY 8
    /* K_KB_TENKEY_9 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_KEYPAD_9, '9'},                 // KB TENKEY 9
    /* K_KB_TENKEY_0 */ {DVC_KEY, MODIFIER_NONE, HID_KEY_KEYPAD_0, '0'},                 // KB TENKEY 0
    /* K_KB_SPACE */ {DVC_KEY, MODIFIER_NONE, HID_KEY_SPACE, ' '},                       // KB SPACE
    /* K_KB_ENTER */ {DVC_KEY, MODIFIER_NONE, HID_KEY_ENTER, 'E'},                       // KB ENTER
    /* K_KB_TAB */ {DVC_KEY, MODIFIER_NONE, HID_KEY_TAB, 'T'},                           // KB TAB
    /* K_KB_GRAVE */ {DVC_KEY, MODIFIER_NONE, HID_KEY_GRAVE, '~'},                       // KB GRAVE (X68K BREAK/ JIS 全角)
    /* K_KB_LEFT_ALT */ {DVC_KEY, KEYBOARD_MODIFIER_LEFTALT, HID_KEY_NONE, 'A'},         // KB LEFT ALT (X68k XF1) E2
    /* K_KB_MUHENKAN */ {DVC_KEY, MODIFIER_NONE, HID_KEY_KANJI5, 'M'},                   // KB 無変換 (X68k XF2) 8B
    /* K_KB_SHIFT */ {DVC_KEY, KEYBOARD_MODIFIER_LEFTSHIFT, HID_KEY_NONE, ' '}           // KB 無変換 (X68k XF2) 8B
};
