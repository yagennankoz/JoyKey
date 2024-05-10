#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <hardware/gpio.h>
#include <hardware/flash.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_TinyUSB.h>

#include "define.hpp"
#include "keyAssign.hpp"


Adafruit_SSD1306    oled(128, 64, &Wire, -1);
uint8_t const desc_hid_report[] =
{
    MY_TUD_HID_REPORT_DESC_GAMEPAD()
};
Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);
my_hid_gamepad_report_t     pad_report;

const DrawKeyPos drawKeyPos[] = {
    {52, 54}, {40, 36}, {28, 30}, {16, 34}, {4, 35}, {28, 42},
    {68, 54}, {80, 36}, {92, 30}, {104, 34}, {116, 35}, {92, 42},
    {52, 24}, {68, 24}
};

uint8_t mode = 0;

uint8_t rapidIdx = 0;
bool rapidKeys[sizeof(keys)];
bool rapidTrigger;
unsigned long fireTime;
unsigned long lastFireTime;
unsigned long keyReleaseTime;
unsigned long cancelTime;
unsigned long blastWaitTime;
unsigned long blastTime;

uint8_t keyCode[6] = {0};

bool stsOutputs[INPUT_MAX] = {false};
bool stsSel;
bool stsStart;
bool stsMode;
bool stsRapid;
bool stsSpeed;
bool stsKeys[sizeof(keys)] = {0};

bool lastOutputs[INPUT_MAX] = {false};
bool lastSel;
bool lastStart;
bool lastMode;
bool lastRapid;
bool lastSpeed;
bool lastKeys[sizeof(keys)] = {0};

bool cancel = false;
bool blastWait = false;
bool blast = false;

void save_setting()
{
    uint8_t write_data[FLASH_PAGE_SIZE];
    CONFIG_DATA *config = (CONFIG_DATA *)&write_data[sizeof(SIGNATURE)];

    memcpy(write_data, SIGNATURE, sizeof(SIGNATURE));
    config->mode = mode;
    config->rapidIdx = rapidIdx;
    for (int i = 0; i < sizeof(keys); i++) {
        config->rapidKeys[i] = rapidKeys[i];
    }

    noInterrupts();
    flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
    flash_range_program(FLASH_TARGET_OFFSET, write_data, FLASH_PAGE_SIZE);
    interrupts();
}

void load_setting()
{
    const uint8_t *saved_data = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);
    CONFIG_DATA *config = (CONFIG_DATA *)&saved_data[sizeof(SIGNATURE)];

    if (0 != memcmp(saved_data, SIGNATURE, sizeof(SIGNATURE))) {
        mode = 0;
        rapidIdx = 0;
    } else {
        mode = config->mode;
        rapidIdx = config->rapidIdx;
        for (int i = 0; i < sizeof(keys); i++) {
            rapidKeys[i] = config->rapidKeys[i];
        }
    }
}

void drawDisplay() {
    oled.clearDisplay();
    oled.setTextSize(1);
    for (uint8_t i = 0; i < 14; i++) {
        if (rapidKeys[i]) {
            oled.fillRoundRect(drawKeyPos[i].x - 3, drawKeyPos[i].y - 3, 12, 12, 3, SSD1306_WHITE);
            oled.setTextColor(SSD1306_BLACK);
        } else {
            oled.drawRoundRect(drawKeyPos[i].x - 3, drawKeyPos[i].y - 3, 12, 12, 3, SSD1306_WHITE);
            oled.setTextColor(SSD1306_WHITE);
        }
        oled.setCursor(drawKeyPos[i].x, drawKeyPos[i].y);
        oled.printf("%c", keyChar[keyAssign[mode][i]]);
        oled.setCursor(drawKeyPos[i].x+1, drawKeyPos[i].y);
        oled.printf("%c", keyChar[keyAssign[mode][i]]);
    }
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(4,7);
    oled.printf("MODE");
    oled.setCursor(106,7);
    oled.printf("sps");
    oled.setTextSize(2);
    oled.setCursor(30,0);
    oled.printf("%d", mode + 1);
    oled.setCursor(82,00);
    oled.printf("%02d", (1000 / (interval[rapidIdx] * 2)));
    oled.drawLine(0,16,128,16,SSD1306_WHITE);
    oled.drawLine(0,18,128,18,SSD1306_WHITE);

    if (keyAssign[mode][sizeof(keys)] == INPUT_MODE_Z) {
        oled.drawBitmap(55, 1, bitmap_z, 16, 12,  SSD1306_WHITE);
    }
    oled.display();
}

bool isKeyEnabled() {
    unsigned long now = micros();
    if (ULONG_MAX - keyReleaseTime <= KEY_RELEASE_WAIT) {
        if ((KEY_RELEASE_WAIT + (ULONG_MAX - keyReleaseTime)) < now) {
            return true;
        }
    } else if (keyReleaseTime + KEY_RELEASE_WAIT < now) {
        return true;
    }
    return false;
}

void setup() {
    pinMode(LED_BUILTIN,    OUTPUT);

    for (uint8_t i = 0; i < sizeof(keys); i++) {
        pinMode(keys[i], INPUT_PULLUP);
    }

    pinMode(PIN_SEL, INPUT_PULLUP);
    pinMode(PIN_START, INPUT_PULLUP);
    pinMode(PIN_MODE, INPUT_PULLUP);
    pinMode(PIN_RAPID, INPUT_PULLUP);
    pinMode(PIN_SPEED, INPUT_PULLUP);

    Wire.available();
    Wire.setSDA(PIN_SDA);
    Wire.setSCL(PIN_SCL);
    Wire.begin(); 

    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    oled.setTextColor(SSD1306_WHITE);
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setCursor(0,0);
    oled.printf("STARTUP");
    oled.display();

    TinyUSB_Device_Init(0);
    usb_hid.begin();

    lastSel = false;
    lastStart = false;
    lastMode = false;
    lastRapid = false;
    lastSpeed = false;

    lastFireTime = micros();
    rapidTrigger = true;

    keyReleaseTime = micros();

    load_setting();

    drawDisplay();

    pinMode(PIN_MIDI_3V3, OUTPUT);
    digitalWrite(PIN_MIDI_3V3, HIGH);
}

void loop() {
    // 連射制御
    fireTime = micros();
    if (ULONG_MAX - lastFireTime <= (unsigned long)interval[rapidIdx] * 1000) {
        if ((interval[rapidIdx] * 1000 + (ULONG_MAX - lastFireTime)) < fireTime) {
            rapidTrigger = !rapidTrigger;
            lastFireTime = fireTime;
        }
    } else if (lastFireTime + (unsigned long)interval[rapidIdx] * 1000 < fireTime) {
        rapidTrigger = !rapidTrigger;
        lastFireTime = fireTime;
    }

    // キー読み出し
    for (uint8_t i = 0; i < sizeof(keys); i++) {
        lastKeys[i] = stsKeys[i];
        stsKeys[i] = !digitalRead(keys[i]);
    }

    stsRapid = !digitalRead(PIN_RAPID);
    // 連射キー設定（トグル）
    if (stsRapid && isKeyEnabled()) {
        for (uint8_t i = 0; i < sizeof(keys); i++) {
            if(stsKeys[i] && !lastKeys[i]) {
                rapidKeys[i] = !rapidKeys[i];
            }
        }
        lastRapid = stsRapid;
        save_setting();
        drawDisplay();
        return;
    }

    // モード変更
    stsMode = !digitalRead(PIN_MODE);
    if (stsMode && !lastMode && isKeyEnabled()) {
        mode++;
        if (mode >= sizeof(keyAssign) / KEY_ASSIGN_NUM) {
            mode = 0;
        }
        lastMode = stsMode;
        save_setting();
        drawDisplay();
        return;
    }

    // 連射速度設定
    stsSpeed = !digitalRead(PIN_SPEED);
    if (stsSpeed & !lastSpeed && isKeyEnabled()) {
        rapidIdx++;
        if (rapidIdx >= sizeof(interval)) {
            rapidIdx = 0;
        }
        save_setting();
        drawDisplay();
    }

    memset(stsOutputs, 0, INPUT_MAX);
    bool fire = false;
    for (uint8_t i = 0; i < sizeof(keys); i++) {
        stsOutputs[keyAssign[mode][i]] |= (stsKeys[i] & (rapidKeys[i] ? rapidTrigger : true));
        fire |= (stsKeys[i] & rapidKeys[i] & rapidTrigger);
    }
    // キャンセル判定
    if (stsOutputs[INPUT_CANCEL] && !lastOutputs[INPUT_CANCEL]) {
        cancel = true;
        cancelTime = micros();
    }
    // ブラスター判定
    if (stsOutputs[INPUT_BLAST] && !lastOutputs[INPUT_BLAST] && !blast) {
        blastWait = true;
        blastWaitTime = micros();
    }
    // キャンセル実行
    if (cancel && (cancelTime + KEY_CANCEL_WAIT > micros())) {
        stsOutputs[INPUT_TRG_A] = false;
        stsOutputs[INPUT_TRG_B] = false;
        stsOutputs[INPUT_TRG_C] = false;
        fire = false;
    } else {
        cancel = false;
    }
    // ブラスター前キャンセル
    if (blastWait) {
        if (blastWaitTime + KEY_CANCEL_WAIT > micros()) {
            stsOutputs[INPUT_LEFT] = false;
            stsOutputs[INPUT_RIGHT] = false;
            stsOutputs[INPUT_UP] = false;
            stsOutputs[INPUT_DOWN] = false;
            stsOutputs[INPUT_TRG_A] = false;
            stsOutputs[INPUT_TRG_B] = false;
            fire = false;
        } else {
            blast = true;
            blastTime = micros();
            blastWait = false;
        }
    }
    // ブラスター
    if (blast) {
        if (blastTime + KEY_TRIGGER_WAIT > micros()) {
            stsOutputs[INPUT_TRG_A] = true;
            stsOutputs[INPUT_TRG_B] = true;
        } else {
            blast = false;
        }
    }
    if (fire) {
        digitalWrite(LED_BUILTIN, HIGH);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
    }

    memset(&pad_report, 0, sizeof(pad_report));
    pad_report.x = (stsOutputs[INPUT_LEFT] ? 0 : 127) + (stsOutputs[INPUT_RIGHT] ? 127 : 0);
    pad_report.y = (stsOutputs[INPUT_UP] ? 0 : 127) + (stsOutputs[INPUT_DOWN] ? 127 : 0);
    pad_report.buttons[0] = stsOutputs[INPUT_TRG_C]
                        | stsOutputs[INPUT_TRG_B] << 1
                        | stsOutputs[INPUT_TRG_A] << 2
                        | stsOutputs[INPUT_03] << 3
                        | stsOutputs[INPUT_L1] << 4
                        | stsOutputs[INPUT_R1] << 5
                        | stsOutputs[INPUT_06] << 6
                        | stsOutputs[INPUT_07] << 7;
    pad_report.buttons[1] = stsOutputs[INPUT_SELECT]
                        | stsOutputs[INPUT_START] << 1
                        | stsOutputs[INPUT_10] << 2
                        | stsOutputs[INPUT_11] << 3;

    if (TinyUSBDevice.mounted() &&usb_hid.ready()) {
        usb_hid.sendReport(0, &pad_report, sizeof(pad_report));
    }

    if ((lastMode && !stsMode)
     || (lastRapid && !stsRapid)
     || (lastSpeed && !stsSpeed)
     || (lastSel && !stsSel)
     || (lastStart && !stsStart)) {
        keyReleaseTime = micros();
    }
    lastMode = stsMode;
    lastRapid = stsRapid;
    lastSpeed = stsSpeed;
    memcpy(lastOutputs, stsOutputs, sizeof(lastOutputs));
}
