/* -*- mode: c++ -*-
 * Atreus -- Chrysalis-enabled Sketch for the Atreus2
 * Copyright (C) 2018, 2019  Keyboard.io, Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "Kaleidoscope.h"
#include "Kaleidoscope-EEPROM-Settings.h"
#include "Kaleidoscope-EEPROM-Keymap.h"
#include "Kaleidoscope-FocusSerial.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-OneShot.h"
#include "Kaleidoscope-Qukeys.h"
#include "Kaleidoscope-SpaceCadet.h"
#include "Kaleidoscope-USB-Quirks.h"

#define MO(n) ShiftToLayer(n)
#define TG(n) LockLayer(n)

// Macros
enum {
  DVORAK_MACRO,
  QUKEYS,
  QUIRKS,
};

#define Key_Exclamation LSHIFT(Key_1)
#define Key_At LSHIFT(Key_2)
#define Key_Hash LSHIFT(Key_3)
#define Key_Dollar LSHIFT(Key_4)
#define Key_Percent LSHIFT(Key_5)
#define Key_Caret LSHIFT(Key_6)
#define Key_And LSHIFT(Key_7)
#define Key_Star LSHIFT(Key_8)
#define Key_Plus LSHIFT(Key_Equals)

// Keymaps
enum {
  DVORAK,
  FUN,
  UPPER
};

/* *INDENT-OFF* */
KEYMAPS(

  [DVORAK] = KEYMAP_STACKED
  (
       Key_Quote     ,Key_Comma ,Key_Period   ,Key_P          ,Key_Y
      ,Key_A         ,Key_O     ,Key_E        ,Key_U          ,Key_I
      ,Key_Semicolon ,Key_Q     ,Key_J        ,Key_K          ,Key_X         ,Key_Backtick
      ,Key_Esc       ,Key_Tab   ,OSM(LeftGui) ,OSM(LeftShift) ,Key_Backspace ,Key_LeftControl

                      ,Key_F     ,Key_G   ,Key_C     ,Key_R     ,Key_L
                      ,Key_D     ,Key_H   ,Key_T     ,Key_N     ,Key_S
       ,Key_Backslash ,Key_B     ,Key_M   ,Key_W     ,Key_V     ,Key_Z
       ,OSM(LeftAlt)  ,Key_Space ,MO(FUN) ,Key_Minus ,Key_Slash ,Key_Enter
  ),

  [FUN] = KEYMAP_STACKED
  (
       Key_Exclamation ,Key_At           ,Key_UpArrow   ,Key_Dollar           ,Key_Percent
      ,Key_LeftParen   ,Key_LeftArrow    ,Key_DownArrow ,Key_RightArrow       ,Key_RightParen
      ,Key_LeftBracket ,Key_RightBracket ,Key_Hash      ,Key_LeftCurlyBracket ,Key_RightCurlyBracket ,Key_Caret
      ,TG(UPPER)       ,Key_Insert       ,Key_LeftGui   ,Key_LeftShift        ,Key_Delete            ,Key_LeftControl

                   ,Key_PageUp   ,Key_7 ,Key_8      ,Key_9 ,Key_Backspace
                   ,Key_PageDown ,Key_4 ,Key_5      ,Key_6 ,M(QUIRKS)
      ,Key_And     ,Key_Star     ,Key_1 ,Key_2      ,Key_3 ,Key_Plus
      ,Key_LeftAlt ,Key_Space    ,___   ,Key_Period ,Key_0 ,Key_Equals
   ),

  [UPPER] = KEYMAP_STACKED
  (
       Key_Insert ,Key_Home                 ,Key_UpArrow   ,Key_End        ,Key_PageUp
      ,Key_Delete ,Key_LeftArrow            ,Key_DownArrow ,Key_RightArrow ,Key_PageDown
      ,XXX        ,Consumer_VolumeIncrement ,XXX           ,XXX            ,___          ,___
      ,M(DVORAK)  ,Consumer_VolumeDecrement ,___           ,___            ,___          ,M(QUKEYS)

                ,Key_UpArrow   ,Key_F7     ,Key_F8          ,Key_F9         ,Key_F10
                ,Key_DownArrow ,Key_F4     ,Key_F5          ,Key_F6         ,Key_F11
      ,___      ,XXX           ,Key_F1     ,Key_F2          ,Key_F3         ,Key_F12
      ,___      ,___           ,M(DVORAK)  ,Key_PrintScreen ,Key_ScrollLock ,Consumer_PlaySlashPause
   )
)
/* *INDENT-ON* */

KALEIDOSCOPE_INIT_PLUGINS(
  EEPROMSettings,
  EEPROMKeymap,
  Focus,
  FocusEEPROMCommand,
  FocusSettingsCommand,
  OneShot,
  SpaceCadet,
  MouseKeys,
  Macros,
  Qukeys,
  USBQuirks
);

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  switch (macroIndex) {
  case QUKEYS:
    Qukeys.toggle();
    break;
  case DVORAK:
    Layer.move(DVORAK);
    break;
  case QUIRKS:
    USBQuirks.toggleKeyboardProtocol();
    break;
  default:
    break;
  }

  return MACRO_NONE;
}

// https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/device/technomancy/Atreus2.h
void setup() {
  QUKEYS(
    kaleidoscope::plugin::Qukey(0, KeyAddr(1, 0), Key_LeftControl), // a
    kaleidoscope::plugin::Qukey(0, KeyAddr(2, 0), Key_LeftShift),   // ;

    kaleidoscope::plugin::Qukey(0, KeyAddr(1, 11), Key_LeftControl), // s
    kaleidoscope::plugin::Qukey(0, KeyAddr(2, 11), Key_LeftShift),   // z

    kaleidoscope::plugin::Qukey(0, KeyAddr(3, 5), Key_Tab),   // Tab on left thumb / ctl
    // kaleidoscope::plugin::Qukey(0, KeyAddr(3, 6), Key_Enter), // Enter on right thumb / alt
  )
  Qukeys.setHoldTimeout(250);
  Qukeys.setOverlapThreshold(80);
  // Start with Qukeys off
  Qukeys.toggle();

  Kaleidoscope.setup();
  SpaceCadet.disable();
  EEPROMKeymap.setup(5);
}

void loop() {
  Kaleidoscope.loop();
}
