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
#include "keyMap.hpp"
#include "icon.hpp"
#include "keyIcon.hpp"

Adafruit_SSD1306 oled(128, 64, &Wire, -1);
uint8_t const desc_hid_report_composite_z[] = {
    JOYKEY_TUD_HID_REPORT_DESC_GAMEPAD_Z(HID_REPORT_ID(RID_JOYPAD)),
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(RID_KEYBOARD))};
uint8_t const desc_hid_report_composite[] = {
    JOYKEY_TUD_HID_REPORT_DESC_GAMEPAD(HID_REPORT_ID(RID_JOYPAD)),
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(RID_KEYBOARD))};
uint8_t const desc_hid_report_joypad_z[] = {
    JOYKEY_TUD_HID_REPORT_DESC_GAMEPAD_Z(HID_REPORT_ID(RID_JOYPAD))};
uint8_t const desc_hid_report_joypad[] = {
    JOYKEY_TUD_HID_REPORT_DESC_GAMEPAD(HID_REPORT_ID(RID_JOYPAD))};
uint8_t const desc_hid_report_joypad_noid_z[] = {
    JOYKEY_TUD_HID_REPORT_DESC_GAMEPAD_Z()};
uint8_t const desc_hid_report_joypad_noid[] = {
    JOYKEY_TUD_HID_REPORT_DESC_GAMEPAD()};
uint8_t const desc_hid_report_key[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(RID_KEYBOARD))};
Adafruit_USBD_HID usb_hid_pad;
Adafruit_USBD_HID usb_hid_key;
Joykey_hid_gamepad_report_t pad_report;
my_hid_keyboard_report_t key_report;

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

bool stsPadOut[OUT_PAD_MAX] = {false};
bool stsSel;
bool stsStart;
bool stsMode;
bool stsRapid;
bool stsSpeed;
bool stsKeys[sizeof(keys)] = {0};

bool lastPadOut[OUT_PAD_MAX] = {false};
bool lastSel;
bool lastStart;
bool lastMode;
bool lastRapid;
bool lastSpeed;
bool lastKeys[sizeof(keys)] = {0};

bool cancel = false;
bool blastWait = false;
bool blast = false;

uint8_t stsKeyModifier = MODIFIER_NONE;
bool stsKeyOut[HID_KEY_GUI_RIGHT + 1] = {false};
bool lastKeyOut[HID_KEY_GUI_RIGHT + 1] = {false};
uint8_t keyUsage[6] = {HID_KEY_NONE};
uint8_t lastKeyUsage[6] = {HID_KEY_NONE};
Joykey_hid_gamepad_report_t lastSentPadReport = {};
Joykey_hid_gamepad_z_report_t lastSentPadReportZ = {};
uint8_t lastSentKeyModifier = MODIFIER_NONE;
uint8_t lastSentKeyUsage[6] = {HID_KEY_NONE};
bool keyOn = false;
uint16_t stsKeyPos = 0;
uint16_t iconColor = SSD1306_WHITE;
uint16_t activeUsbVid = usbIdentityProfiles[USB_ID_PROFILE_PICO].vid;
uint16_t activeUsbPid = usbIdentityProfiles[USB_ID_PROFILE_PICO].pid;
const char *activeUsbManufacturer = usbIdentityProfiles[USB_ID_PROFILE_PICO].manufacturer;
const char *activeUsbProduct = usbIdentityProfiles[USB_ID_PROFILE_PICO].product;

static uint8_t const horiFeatureReport[8] = {0x21, 0x26, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00};
static uint8_t lastHoriOutputReport[8] = {0};
static constexpr uint8_t PAD_REPORT_LEN_Z = (uint8_t)sizeof(Joykey_hid_gamepad_z_report_t);

uint16_t hidPadGetReportCallback(uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
  (void)report_id;

  if (report_type == HID_REPORT_TYPE_FEATURE)
  {
    uint16_t len = (reqlen < sizeof(horiFeatureReport)) ? reqlen : (uint16_t)sizeof(horiFeatureReport);
    memcpy(buffer, horiFeatureReport, len);
    return len;
  }

  return 0;
}

void hidPadSetReportCallback(uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
  (void)report_id;

  if ((report_type == HID_REPORT_TYPE_OUTPUT || report_type == HID_REPORT_TYPE_FEATURE) && bufsize)
  {
    uint16_t len = (bufsize < sizeof(lastHoriOutputReport)) ? bufsize : (uint16_t)sizeof(lastHoriOutputReport);
    memcpy(lastHoriOutputReport, buffer, len);
  }
}

uint8_t getUsbProfileIndexByMode(uint8_t modeIdx)
{
  uint8_t profile = keyAssign[modeIdx].usbProfile;
  if (profile >= USB_ID_PROFILE_MAX)
  {
    return USB_ID_PROFILE_PICO;
  }
  return profile;
}

void updateUsbIdentityByMode(uint8_t modeIdx)
{
  uint8_t profileIdx = getUsbProfileIndexByMode(modeIdx);
  activeUsbVid = usbIdentityProfiles[profileIdx].vid;
  activeUsbPid = usbIdentityProfiles[profileIdx].pid;
  activeUsbManufacturer = usbIdentityProfiles[profileIdx].manufacturer;
  activeUsbProduct = usbIdentityProfiles[profileIdx].product;
}

void save_setting()
{
  uint8_t write_data[FLASH_PAGE_SIZE];
  ConfigData *config = (ConfigData *)&write_data[sizeof(SIGNATURE)];

  memcpy(write_data, SIGNATURE, sizeof(SIGNATURE));
  config->mode = mode;
  config->rapidIdx = rapidIdx;
  for (int i = 0; i < sizeof(keys); i++)
  {
    config->rapidKeys[i] = rapidKeys[i];
  }

  noInterrupts();
  flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
  flash_range_program(FLASH_TARGET_OFFSET, write_data, FLASH_PAGE_SIZE);
  interrupts();
}

void load_setting()
{
  const uint8_t *saved_data = (const uint8_t *)(XIP_BASE + FLASH_TARGET_OFFSET);
  ConfigData *config = (ConfigData *)&saved_data[sizeof(SIGNATURE)];

  if (0 != memcmp(saved_data, SIGNATURE, sizeof(SIGNATURE)))
  {
    mode = 0;
    rapidIdx = 0;
  }
  else
  {
    mode = config->mode;
    rapidIdx = config->rapidIdx;
    for (int i = 0; i < sizeof(keys); i++)
    {
      rapidKeys[i] = config->rapidKeys[i];
    }
  }
}

void drawDisplay()
{
  oled.clearDisplay();
  oled.setTextSize(1);
  for (uint8_t i = 0; i < KEY_ASSIGN_NUM; i++)
  {
    if (keyAssign[mode].key[i].code[0] == K_NOP)
    {
      oled.drawRect(drawKeyPos[i].x - 3, drawKeyPos[i].y - 3, 12, 12, SSD1306_WHITE);
      oled.drawLine(drawKeyPos[i].x - 3, drawKeyPos[i].y - 3, drawKeyPos[i].x + 7, drawKeyPos[i].y + 7, SSD1306_WHITE);
      oled.drawLine(drawKeyPos[i].x + 7, drawKeyPos[i].y - 3, drawKeyPos[i].x - 3, drawKeyPos[i].y + 7, SSD1306_WHITE);
    }
    else
    {
      if (rapidKeys[i])
      {
        if (keyDefTbl[keyAssign[mode].key[i].code[0]].device == DVC_PAD)
        {
          oled.fillRoundRect(drawKeyPos[i].x - 3, drawKeyPos[i].y - 3, 12, 12, 3, SSD1306_WHITE);
        }
        else
        {
          oled.fillRect(drawKeyPos[i].x - 3, drawKeyPos[i].y - 3, 12, 12, SSD1306_WHITE);
        }
        iconColor = SSD1306_BLACK;
      }
      else
      {
        if (keyDefTbl[keyAssign[mode].key[i].code[0]].device == DVC_PAD)
        {
          oled.drawRoundRect(drawKeyPos[i].x - 3, drawKeyPos[i].y - 3, 12, 12, 3, SSD1306_WHITE);
        }
        else
        {
          oled.drawRect(drawKeyPos[i].x - 3, drawKeyPos[i].y - 3, 12, 12, SSD1306_WHITE);
        }
        iconColor = SSD1306_WHITE;
      }
      if (keyAssign[mode].key[i].keyIcon == KEY_ICON_NONE)
      {
        oled.setTextColor(iconColor);
        oled.setCursor(drawKeyPos[i].x, drawKeyPos[i].y);
        oled.printf("%c", keyDefTbl[keyAssign[mode].key[i].code[0]].disp);
        oled.setCursor(drawKeyPos[i].x + 1, drawKeyPos[i].y);
        oled.printf("%c", keyDefTbl[keyAssign[mode].key[i].code[0]].disp);
      }
      else
      {
        oled.drawBitmap(drawKeyPos[i].x - 1, drawKeyPos[i].y - 1, keyIconDef[keyAssign[mode].key[i].keyIcon - 1], 8, 8, iconColor);
      }
    }
  }
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(4, 7);
  oled.printf("MODE");
  oled.setCursor(106, 7);
  oled.printf("sps");
  oled.setTextSize(2);
  oled.setCursor(30, 0);
  oled.printf("%d", mode + 1);
  oled.setCursor(82, 00);
  oled.printf("%02d", (1000 / (interval[rapidIdx] * 2)));
  oled.drawLine(0, 16, 128, 16, SSD1306_WHITE);
  oled.drawLine(0, 18, 128, 18, SSD1306_WHITE);

  switch (keyAssign[mode].icon)
  {
  case INPUT_MODE_Z:
    oled.drawBitmap(55, 1, icon_z, 16, 12, SSD1306_WHITE);
    break;
  case INPUT_MODE_GRADIUS:
    oled.drawBitmap(55, 0, icon_gradius, 16, 14, SSD1306_WHITE);
    break;
  case INPUT_MODE_LAGOON:
    oled.drawBitmap(55, 0, icon_lagoon, 16, 14, SSD1306_WHITE);
    break;
  case INPUT_MODE_NEMESIS:
    oled.drawBitmap(55, 0, icon_nemesis, 16, 14, SSD1306_WHITE);
    break;
  case INPUT_MODE_PLAYSTATION:
    oled.drawBitmap(55, 0, icon_playstation, 16, 14, SSD1306_WHITE);
    break;
  case INPUT_MODE_YS1:
    oled.drawBitmap(55, 0, icon_YS1, 16, 14, SSD1306_WHITE);
    break;
  case INPUT_MODE_YS2:
    oled.drawBitmap(55, 0, icon_YS2, 16, 14, SSD1306_WHITE);
    break;
  case INPUT_MODE_YS3:
    oled.drawBitmap(55, 0, icon_YS3, 16, 14, SSD1306_WHITE);
    break;
  }
  oled.display();
}

bool isKeyEnabled()
{
  unsigned long now = micros();
  if (ULONG_MAX - keyReleaseTime <= KEY_RELEASE_WAIT)
  {
    if ((KEY_RELEASE_WAIT + (ULONG_MAX - keyReleaseTime)) < now)
    {
      return true;
    }
  }
  else if (keyReleaseTime + KEY_RELEASE_WAIT < now)
  {
    return true;
  }
  return false;
}

void setup()
{
  load_setting();
  updateUsbIdentityByMode(mode);

  pinMode(LED_BUILTIN, OUTPUT);

  for (uint8_t i = 0; i < sizeof(keys); i++)
  {
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
  oled.setCursor(0, 0);
  oled.printf("STARTUP");
  oled.display();

  usb_hid_pad.setPollInterval(1);
  usb_hid_key.setPollInterval(1);
  usb_hid_pad.setReportCallback(hidPadGetReportCallback, hidPadSetReportCallback);

  TinyUSB_Device_Init(0);
  TinyUSBDevice.setID(activeUsbVid, activeUsbPid);
  TinyUSBDevice.setManufacturerDescriptor(activeUsbManufacturer);
  TinyUSBDevice.setProductDescriptor(activeUsbProduct);

  bool useZPadDescriptor = (getUsbProfileIndexByMode(mode) == USB_ID_PROFILE_Z);
  uint8_t const *padDescWithId = useZPadDescriptor ? desc_hid_report_joypad_z : desc_hid_report_joypad;
  uint16_t padDescWithIdLen = useZPadDescriptor ? sizeof(desc_hid_report_joypad_z) : sizeof(desc_hid_report_joypad);
  uint8_t const *padDescNoId = useZPadDescriptor ? desc_hid_report_joypad_noid_z : desc_hid_report_joypad_noid;
  uint16_t padDescNoIdLen = useZPadDescriptor ? sizeof(desc_hid_report_joypad_noid_z) : sizeof(desc_hid_report_joypad_noid);
  uint8_t const *compositeDesc = useZPadDescriptor ? desc_hid_report_composite_z : desc_hid_report_composite;
  uint16_t compositeDescLen = useZPadDescriptor ? sizeof(desc_hid_report_composite_z) : sizeof(desc_hid_report_composite);

  switch (keyAssign[mode].device)
  {
  case USB_DVC_PAD:
    usb_hid_pad.setReportDescriptor(padDescNoId, padDescNoIdLen);
    usb_hid_pad.begin();
    break;
  case USB_DVC_KEY:
    usb_hid_key.setBootProtocol(HID_ITF_PROTOCOL_KEYBOARD);
    usb_hid_key.setReportDescriptor(desc_hid_report_key, sizeof(desc_hid_report_key));
    usb_hid_key.begin();
    break;
  default:
    usb_hid_pad.setReportDescriptor(compositeDesc, compositeDescLen);
    usb_hid_key.setBootProtocol(HID_ITF_PROTOCOL_KEYBOARD);
    usb_hid_key.setReportDescriptor(desc_hid_report_key, sizeof(desc_hid_report_key));
    usb_hid_pad.begin();
    usb_hid_key.begin();
  }

  if (TinyUSBDevice.mounted())
  {
    TinyUSBDevice.detach();
    delay(10);
    TinyUSBDevice.attach();
  }

  lastSel = false;
  lastStart = false;
  lastMode = false;
  lastRapid = false;
  lastSpeed = false;

  lastFireTime = micros();
  rapidTrigger = true;

  keyReleaseTime = micros();

  drawDisplay();

  pinMode(PIN_MIDI_3V3, OUTPUT);
  digitalWrite(PIN_MIDI_3V3, HIGH);
}

void loop()
{
  // 連射制御
  fireTime = micros();
  if (ULONG_MAX - lastFireTime <= (unsigned long)interval[rapidIdx] * 1000)
  {
    if ((interval[rapidIdx] * 1000 + (ULONG_MAX - lastFireTime)) < fireTime)
    {
      rapidTrigger = !rapidTrigger;
      lastFireTime = fireTime;
    }
  }
  else if (lastFireTime + (unsigned long)interval[rapidIdx] * 1000 < fireTime)
  {
    rapidTrigger = !rapidTrigger;
    lastFireTime = fireTime;
  }

  // キー読み出し
  for (uint8_t i = 0; i < sizeof(keys); i++)
  {
    lastKeys[i] = stsKeys[i];
    stsKeys[i] = !digitalRead(keys[i]);
  }

  stsRapid = !digitalRead(PIN_RAPID);
  // 連射キー設定（トグル）
  if (stsRapid && isKeyEnabled())
  {
    for (uint8_t i = 0; i < sizeof(keys); i++)
    {
      if (stsKeys[i] && !lastKeys[i])
      {
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
  if (stsMode && !lastMode && isKeyEnabled())
  {
    uint8_t lastDevice = keyAssign[mode].device;
    uint8_t lastUsbProfile = getUsbProfileIndexByMode(mode);
    mode++;
    if (mode >= sizeof(keyAssign) / sizeof(KeyAssign))
    {
      mode = 0;
    }
    uint8_t currentUsbProfile = getUsbProfileIndexByMode(mode);
    lastMode = stsMode;
    save_setting();
    drawDisplay();
    if ((lastDevice != keyAssign[mode].device) || (lastUsbProfile != currentUsbProfile))
    {
      TinyUSBDevice.detach();
      oled.clearDisplay();
      oled.setTextColor(SSD1306_WHITE);
      oled.setCursor(0, 0);
      oled.setTextSize(1);
      oled.printf("REBOOTING...");
      oled.display();
      watchdog_enable(500, 1);
      while (1)
        ;
    }
    return;
  }

  // 連射速度設定
  stsSpeed = !digitalRead(PIN_SPEED);
  if (stsSpeed & !lastSpeed && isKeyEnabled())
  {
    rapidIdx++;
    if (rapidIdx >= sizeof(interval))
    {
      rapidIdx = 0;
    }
    save_setting();
    drawDisplay();
  }

  memset(stsPadOut, 0, OUT_PAD_MAX);
  memset(stsKeyOut, false, sizeof(stsKeyOut));
  stsKeyModifier = MODIFIER_NONE;
  memset(keyUsage, HID_KEY_NONE, sizeof(keyUsage));

  bool fire = false;
  // キー入力のマージ
  for (uint8_t i = 0; i < sizeof(keys); i++)
  {
    for (uint8_t j = 0; j < KEY_ASSIGN_KEY_NUM; j++)
    {
      if (keyDefTbl[keyAssign[mode].key[i].code[j]].device == DVC_PAD)
      {
        // JOYPAD入力
        stsPadOut[keyDefTbl[keyAssign[mode].key[i].code[j]].usage] |= (stsKeys[i] & (rapidKeys[i] ? rapidTrigger : true));
      }
      else
      {
        // キーボード入力
        stsKeyOut[keyDefTbl[keyAssign[mode].key[i].code[j]].usage] |= (stsKeys[i] & (rapidKeys[i] ? rapidTrigger : true));
        stsKeyModifier |= (stsKeys[i] & (rapidKeys[i] ? rapidTrigger : true)) ? keyDefTbl[keyAssign[mode].key[i].code[j]].modifier : MODIFIER_NONE;
        if (stsKeyModifier)
        {
          digitalWrite(LED_BUILTIN, HIGH);
        }
      }
    }
    fire |= (stsKeys[i] & rapidKeys[i] & rapidTrigger);
  }
  keyOn = false;
  uint8_t keyUsageCount = 0;

  for (uint8_t i = 0; i < sizeof(lastKeyUsage) && keyUsageCount < sizeof(keyUsage); i++)
  {
    uint8_t usage = lastKeyUsage[i];
    if (usage == HID_KEY_NONE || !stsKeyOut[usage])
    {
      continue;
    }

    bool alreadyAdded = false;
    for (uint8_t j = 0; j < keyUsageCount; j++)
    {
      if (keyUsage[j] == usage)
      {
        alreadyAdded = true;
        break;
      }
    }

    if (!alreadyAdded)
    {
      keyUsage[keyUsageCount++] = usage;
    }
  }

  for (uint16_t usage = 1; usage < sizeof(stsKeyOut) && keyUsageCount < sizeof(keyUsage); usage++)
  {
    if (!stsKeyOut[usage])
    {
      continue;
    }

    bool alreadyAdded = false;
    for (uint8_t j = 0; j < keyUsageCount; j++)
    {
      if (keyUsage[j] == usage)
      {
        alreadyAdded = true;
        break;
      }
    }

    if (!alreadyAdded)
    {
      keyUsage[keyUsageCount++] = (uint8_t)usage;
    }
  }

  keyOn = (keyUsageCount > 0);

  // キャンセル判定
  if (stsPadOut[OUT_PAD_CANCEL] && !lastPadOut[OUT_PAD_CANCEL])
  {
    cancel = true;
    cancelTime = micros();
  }
  // ブラスター判定
  if (stsPadOut[OUT_PAD_BLAST] && !lastPadOut[OUT_PAD_BLAST] && !blast)
  {
    blastWait = true;
    blastWaitTime = micros();
  }
  // キャンセル実行
  if (cancel && (cancelTime + KEY_CANCEL_WAIT > micros()))
  {
    stsPadOut[OUT_PAD_TRG_A] = false;
    stsPadOut[OUT_PAD_TRG_B] = false;
    stsPadOut[OUT_PAD_TRG_C] = false;
    fire = false;
  }
  else
  {
    cancel = false;
  }
  // ブラスター前キャンセル
  if (blastWait)
  {
    if (blastWaitTime + KEY_CANCEL_WAIT > micros())
    {
      stsPadOut[OUT_PAD_LEFT] = false;
      stsPadOut[OUT_PAD_RIGHT] = false;
      stsPadOut[OUT_PAD_UP] = false;
      stsPadOut[OUT_PAD_DOWN] = false;
      stsPadOut[OUT_PAD_TRG_A] = false;
      stsPadOut[OUT_PAD_TRG_B] = false;
      fire = false;
    }
    else
    {
      blast = true;
      blastTime = micros();
      blastWait = false;
    }
  }
  // ブラスター
  if (blast)
  {
    if (blastTime + KEY_TRIGGER_WAIT > micros())
    {
      stsPadOut[OUT_PAD_TRG_A] = true;
      stsPadOut[OUT_PAD_TRG_B] = true;
    }
    else
    {
      blast = false;
    }
  }
  if (fire)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }

  memset(&pad_report, 0, sizeof(pad_report));
  bool padUp = stsPadOut[OUT_PAD_UP];
  bool padDown = stsPadOut[OUT_PAD_DOWN];
  bool padLeft = stsPadOut[OUT_PAD_LEFT];
  bool padRight = stsPadOut[OUT_PAD_RIGHT];

  pad_report.hat = 8;
  if (padUp && !padDown)
  {
    if (padRight && !padLeft)
      pad_report.hat = 1;
    else if (padLeft && !padRight)
      pad_report.hat = 7;
    else
      pad_report.hat = 0;
  }
  else if (padDown && !padUp)
  {
    if (padRight && !padLeft)
      pad_report.hat = 3;
    else if (padLeft && !padRight)
      pad_report.hat = 5;
    else
      pad_report.hat = 4;
  }
  else if (padRight && !padLeft)
  {
    pad_report.hat = 2;
  }
  else if (padLeft && !padRight)
  {
    pad_report.hat = 6;
  }

  pad_report.x = 127;
  pad_report.y = 127;
  pad_report.z = 127;
  pad_report.rz = 127;

  uint16_t padButtons = 0;
  padButtons |= (stsPadOut[OUT_PAD_SQUARE] ? 1u : 0u) << 0;
  padButtons |= (stsPadOut[OUT_PAD_CROSS] ? 1u : 0u) << 1;
  padButtons |= (stsPadOut[OUT_PAD_CIRCLE] ? 1u : 0u) << 2;
  padButtons |= (stsPadOut[OUT_PAD_TRIANGLE] ? 1u : 0u) << 3;
  padButtons |= (stsPadOut[OUT_PAD_L1] ? 1u : 0u) << 4;
  padButtons |= (stsPadOut[OUT_PAD_R1] ? 1u : 0u) << 5;
  padButtons |= (stsPadOut[OUT_PAD_L2] ? 1u : 0u) << 6;
  padButtons |= (stsPadOut[OUT_PAD_R2] ? 1u : 0u) << 7;
  padButtons |= (stsPadOut[OUT_PAD_SHARE] ? 1u : 0u) << 8;
  padButtons |= (stsPadOut[OUT_PAD_OPTIONS] ? 1u : 0u) << 9;
  padButtons |= (stsPadOut[OUT_PAD_L3] ? 1u : 0u) << 10;
  padButtons |= (stsPadOut[OUT_PAD_R3] ? 1u : 0u) << 11;
  padButtons |= (stsPadOut[OUT_PAD_PS] ? 1u : 0u) << 12;

  pad_report.buttons[0] = (uint8_t)(padButtons & 0xFFu);
  pad_report.buttons[1] = (uint8_t)((padButtons >> 8) & 0xFFu);
  key_report.modifier = 0;
  key_report.usage[0] = HID_KEY_A;

  if (TinyUSBDevice.mounted())
  {
    if (keyAssign[mode].device == USB_DVC_COMPOSITE || keyAssign[mode].device == USB_DVC_KEY)
    {
      if ((lastSentKeyModifier != stsKeyModifier || memcmp(lastSentKeyUsage, keyUsage, sizeof(keyUsage)) != 0) && usb_hid_key.ready())
      {
        usb_hid_key.keyboardReport(RID_KEYBOARD, stsKeyModifier, keyUsage);
        lastSentKeyModifier = stsKeyModifier;
        memcpy(lastSentKeyUsage, keyUsage, sizeof(lastSentKeyUsage));
      }
    }
    if (keyAssign[mode].device == USB_DVC_COMPOSITE || keyAssign[mode].device == USB_DVC_PAD)
    {
      bool useZPadDescriptor = (getUsbProfileIndexByMode(mode) == USB_ID_PROFILE_Z);
      if (useZPadDescriptor)
      {
        Joykey_hid_gamepad_z_report_t pad_report_z = {};
        uint16_t zButtons = (uint16_t)(padButtons & 0x0FFFu);
        pad_report_z.buttons[0] = (uint8_t)(zButtons & 0xFFu);
        pad_report_z.buttons[1] = (uint8_t)((zButtons >> 8) & 0x0Fu);
        pad_report_z.x = (padLeft ? 0 : 127) + (padRight ? 127 : 0);
        pad_report_z.y = (padUp ? 0 : 127) + (padDown ? 127 : 0);

        if (memcmp(&lastSentPadReportZ, &pad_report_z, PAD_REPORT_LEN_Z) != 0 && usb_hid_pad.ready())
        {
          uint8_t padReportId = (keyAssign[mode].device == USB_DVC_PAD) ? 0 : RID_JOYPAD;
          usb_hid_pad.sendReport(padReportId, &pad_report_z, PAD_REPORT_LEN_Z);
          memcpy(&lastSentPadReportZ, &pad_report_z, PAD_REPORT_LEN_Z);
        }
      }
      else
      {
        if (memcmp(&lastSentPadReport, &pad_report, sizeof(pad_report)) != 0 && usb_hid_pad.ready())
        {
          uint8_t padReportId = (keyAssign[mode].device == USB_DVC_PAD) ? 0 : RID_JOYPAD;
          usb_hid_pad.sendReport(padReportId, &pad_report, sizeof(pad_report));
          memcpy(&lastSentPadReport, &pad_report, sizeof(lastSentPadReport));
        }
      }
    }
  }

  if ((lastMode && !stsMode) || (lastRapid && !stsRapid) || (lastSpeed && !stsSpeed) || (lastSel && !stsSel) || (lastStart && !stsStart))
  {
    keyReleaseTime = micros();
  }
  lastMode = stsMode;
  lastRapid = stsRapid;
  lastSpeed = stsSpeed;
  memcpy(lastPadOut, stsPadOut, sizeof(lastPadOut));
  memcpy(lastKeyOut, stsKeyOut, sizeof(lastKeyOut));
  memcpy(lastKeyUsage, keyUsage, sizeof(lastKeyUsage));
}
