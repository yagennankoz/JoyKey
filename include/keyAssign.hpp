#pragma once

#include "define.hpp"
#include "keyMap.hpp"
#include "keyIcon.hpp"

typedef struct
{
    InternalKeyDef code[KEY_ASSIGN_KEY_NUM];
    KeyIconCode keyIcon;
} OneKeyDef;

// Key Assign Definition
typedef struct
{
    OneKeyDef key[KEY_ASSIGN_NUM];
    uint8_t icon;
    UsbDevice device;
    // USB_ID_PROFILE_* that selects VID/PID at boot.
    uint8_t usbProfile;
} KeyAssign;

enum
{
    INPUT_MODE_Z = 0,
    INPUT_MODE_GRADIUS,
    INPUT_MODE_LAGOON,
    INPUT_MODE_NEMESIS,
    INPUT_MODE_YS1,
    INPUT_MODE_YS2,
    INPUT_MODE_YS3,
    INPUT_MODE_PLAYSTATION,
};

/*
               SEL START
  L5 L4 L3 L2            R2 R3 R4 R5
        L6                  R6
              L1       R1
*/

// L1,L2,L3,L4,L5,L6, R1,R2,R3,R4,R5,R6, SEL, START, MODE
KeyAssign keyAssign[] = {
    // PLAYSTATION
    {
        // L
        K_PD_CROSS, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_CROSS,
        K_PD_CIRCLE, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_CIRCLE,
        K_PD_TRIANGLE, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_TRIANGLE,
        K_PD_SQUARE, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_SQUARE,
        K_PD_L1, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_PD_CROSS, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_CROSS,
        // R
        K_PD_PS, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_PS,
        K_PD_LEFT, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_LEFT,
        K_PD_UP, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_UP,
        K_PD_RIGHT, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_RIGHT,
        K_PD_R1, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_PD_DOWN, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_DOWN,
        // OTHERS
        K_PD_SHARE, K_PD_OPTIONS, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_SHARE,
        K_PD_OPTIONS, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_OPTION,
        // ICON
        INPUT_MODE_PLAYSTATION,
        // DEVICE
        USB_DVC_PAD,
        // USB PROFILE
        USB_ID_PROFILE_HORI_FIGHTING_STICK_V3},

    // X68000 Z JOYCARD
    {
        // L
        K_PD_BLAST, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_PD_TRG_B, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_A,
        K_PD_TRG_A, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_B,
        K_PD_R1, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_PD_L1, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_PD_TRG_A, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_B,
        // R
        K_KB_BREAK, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_STATESAVE,
        K_PD_LEFT, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_LEFT,
        K_PD_UP, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_UP,
        K_PD_RIGHT, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_RIGHT,
        K_KB_ESC, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_PAUSE,
        K_PD_DOWN, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_DOWN,
        // OTHERS
        K_PD_SELECT, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_PD_START, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        // ICON
        INPUT_MODE_Z,
        // DEVICE
        USB_DVC_COMPOSITE,
        // USB PROFILE
        USB_ID_PROFILE_Z},

    // GRADIUS
    {
        // L
        K_KB_Z, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_C, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_X, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_Z, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_C, K_KB_X, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_SHOT_AND_BOMB,
        // R
        K_KB_ENTER, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_ENTER,
        K_PD_LEFT, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_LEFT,
        K_PD_UP, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_UP,
        K_PD_RIGHT, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_RIGHT,
        K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_PD_DOWN, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_DOWN,
        // OTHERS
        K_KB_BREAK, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_STATESAVE,
        K_KB_ESC, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_PAUSE,
        // MODE
        INPUT_MODE_GRADIUS,
        // DEVICE
        USB_DVC_COMPOSITE,
        // USB PROFILE
        USB_ID_PROFILE_Z},

    // LAGOON
    {
        // L
        K_KB_XF1, K_KB_XF2, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_MAGIC,
        K_KB_XF2, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_SWORD,
        K_KB_XF1, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_JUMP,
        K_KB_I, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_E, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_S, K_KB_X, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        // R
        K_KB_SHIFT, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_ITEM,
        K_PD_LEFT, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_LEFT,
        K_PD_UP, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_UP,
        K_PD_RIGHT, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_RIGHT,
        K_KB_ENTER, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_ENTER,
        K_PD_DOWN, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_DOWN,
        // OTHERS
        K_KB_BREAK, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_STATESAVE,
        K_KB_ESC, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_PAUSE,
        // MODE
        INPUT_MODE_LAGOON,
        // DEVICE
        USB_DVC_COMPOSITE,
        // USB PROFILE
        USB_ID_PROFILE_Z},

    // NEMESIS'90KAI
    {
        // L
        K_PD_TRG_B, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_A,
        K_PD_TRG_A, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_B,
        K_PD_TRG_A, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_B,
        K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_F5, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_F5,
        K_PD_TRG_A, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_B,
        // R
        K_PD_DOWN, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_DOWN,
        K_PD_LEFT, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_LEFT,
        K_PD_UP, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_UP,
        K_PD_RIGHT, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_RIGHT,
        K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_PD_DOWN, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_DOWN,
        // OTHERS
        K_KB_BREAK, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_STATESAVE,
        K_KB_ESC, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_PAUSE,
        // ICON
        INPUT_MODE_NEMESIS,
        // DEVICE
        USB_DVC_COMPOSITE,
        // USB PROFILE
        USB_ID_PROFILE_Z},

    // Ys I
    {
        // L
        K_KB_F1, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_F1,
        K_KB_SPACE, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_ENTER, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_ENTER,
        K_KB_R, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_S, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_I, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        // R
        K_KB_F4, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_F4,
        K_KB_TENKEY_4, K_KB_LEFT, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_TENKEY_8, K_KB_UP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_TENKEY_6, K_KB_RIGHT, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_TENKEY_2, K_KB_DOWN, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        // OTHERS
        K_KB_BREAK, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_STATESAVE,
        K_KB_ESC, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_ESC,
        // ICON
        INPUT_MODE_YS1,
        // DEVICE
        USB_DVC_KEY,
        // USB PROFILE
        USB_ID_PROFILE_Z},

    // Ys II
    {
        // L
        K_KB_F1, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_F1,
        K_KB_SPACE, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_ENTER, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_ENTER,
        K_KB_E, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_S, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_I, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        // R
        K_KB_F4, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_F4,
        K_KB_TENKEY_4, K_KB_LEFT, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_TENKEY_8, K_KB_UP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_TENKEY_6, K_KB_RIGHT, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_F6, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_F6,
        K_KB_TENKEY_2, K_KB_DOWN, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        // OTHERS
        K_KB_BREAK, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_STATESAVE,
        K_KB_ESC, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_ESC,
        // ICON
        INPUT_MODE_YS2,
        // DEVICE
        USB_DVC_KEY,
        // USB PROFILE
        USB_ID_PROFILE_Z},

    // Ys III
    {
        // L
        K_KB_F1, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_F1,
        K_KB_X, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_Z, K_KB_ENTER, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_SHIFT, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_SHIFT,
        K_KB_S, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_I, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        // R
        K_KB_F4, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_F4,
        K_KB_TENKEY_4, K_KB_LEFT, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_TENKEY_8, K_KB_UP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_TENKEY_6, K_KB_RIGHT, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        K_KB_ENTER, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_ENTER,
        K_KB_TENKEY_2, K_KB_DOWN, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_NONE,
        // OTHERS
        K_KB_BREAK, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_STATESAVE,
        K_KB_ESC, K_NOP, K_NOP, K_NOP, K_NOP, K_NOP, KEY_ICON_ESC,
        // ICON
        INPUT_MODE_YS3,
        // DEVICE
        USB_DVC_KEY,
        // USB PROFILE
        USB_ID_PROFILE_Z},

};
