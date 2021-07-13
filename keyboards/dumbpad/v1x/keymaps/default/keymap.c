/* Copyright 2020 imchipwood
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "print.h"

uint8_t knob_mode = 0;

enum my_keycodes {
  INC_KMODE
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
            BASE LAYER
    /-----------------------------------------------------`
    |             |    7    |    8    |    9    |  Bkspc  |
    |             |---------|---------|---------|---------|
    |             |    4    |    5    |    6    |   Esc   |
    |             |---------|---------|---------|---------|
    |             |    1    |    2    |    3    |   Tab   |
    |-------------|---------|---------|---------|---------|
    | Left mouse  |  TT(1)  |    0    |    .    |  Enter  |
    \-----------------------------------------------------'
    */
    [0] = LAYOUT(
                    KC_7,      KC_8,    KC_9,             KC_BSPC,
                    KC_4,      KC_5,    KC_6,             LALT(KC_TAB),
                    KC_1,      KC_2,    KC_3,             A(S(KC_TAB)),
        KC_MPLY,    TT(1),     KC_0,    LSFT_T(KC_DOT),   KC_ENTER
    ),
    /*
            SUB LAYER
    /-----------------------------------------------------`
    |             |         |         |         |  Reset  |
    |             |---------|---------|---------|---------|
    |             |         |         |         |    +    |
    |             |---------|---------|---------|---------|
    |             |         |         |         |    -    |
    |-------------|---------|---------|---------|---------|
    |    LOCK     |         |         |         |    =    |
    \-----------------------------------------------------'
    */

    
    [1] = LAYOUT(
                    _______,     _______,     _______,      RESET,
                    _______,     _______,     _______,      KC_KP_PLUS,
                    _______,     _______,     _______,      KC_KP_MINUS,
        KC_BTN1,    _______,     _______,     _______,      INC_KMODE
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case INC_KMODE: {
            if (record->event.pressed) {
                if (knob_mode < 3) {
                    knob_mode++;
                }
                else {
                    knob_mode = 0;
                }

                uprintf("knob mode %u\n", knob_mode);
            }
            return false;
        }
        default:
        return true;
    }
    // If console is enabled, it will print the matrix position and status of each key pressed
/*
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: %u, col: %u, row: %u, pressed: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed);
#endif
*/
    return true;
}

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    //debug_enable = true;
    //debug_matrix = true;
    //debug_keyboard = true;
    //debug_mouse = true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    /*  Custom encoder control - handles CW/CCW turning of encoder
     *  Default behavior:
     *    main layer:
     *       CW: move mouse right
     *      CCW: move mouse left
     *    other layers:
     *       CW: = (equals/plus - increase slider in Adobe products)
     *      CCW: - (minus/underscore - decrease slider in adobe products)
     */
    if (index == 0) {
        switch (get_highest_layer(layer_state)) {
            case 0:
                // main layer - move mouse right (CW) and left (CCW)
                if (clockwise) {
                    tap_code(KC_AUDIO_VOL_DOWN);
                } else {
                    tap_code(KC_AUDIO_VOL_UP);
                }
                break;

            default:
                // main layer - move mouse right (CW) and left (CCW)
                if (clockwise) {
                    if (knob_mode == 0 || knob_mode == 1) {
                            tap_code(KC_MS_R);
                        }
                        else {
                            tap_code(KC_MS_D);
                        }
                } else {
                    if (knob_mode == 0 || knob_mode == 1) {
                        tap_code(KC_MS_L);
                    }
                    else {
                        tap_code(KC_MS_U);
                    }
                }
                break;
        }
    }
    return true;
}
