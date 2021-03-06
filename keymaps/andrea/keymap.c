#include "kb.h"
//Layer names
#define _BL 0
#define _FN 1

typedef union {
  uint32_t raw;
  struct {
    bool     capslock_change :1;
  };
} user_config_t;

user_config_t user_config;
void checkMode(void);
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[_BL] = KEYMAP(
		KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_NO, KC_BSPC,
		KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS,
		KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT,
		KC_LSFT, KC_NO, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_UP, KC_RSFT,
		KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_SPC, KC_SPC, MO(_FN), KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),

	[_FN] = KEYMAP(
		KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, RGB_TOG, RGB_TOG,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PSCR, KC_SLCK, KC_BRK, KC_TRNS, KC_TRNS, KC_TRNS, RESET,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_INS, KC_HOME, KC_PGUP, KC_VOLD, KC_VOLU, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_END, KC_PGDN, KC_TRNS, KC_TRNS, KC_DEL,
		KC_TRNS, KC_RGUI, KC_RALT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case KC_CAPS:
            if(record->event.pressed) {
                user_config.capslock_change ^= 1;
                checkMode();
                eeconfig_update_user(user_config.raw);
            }
        break;
    }
	return true;
}
void checkMode(void) {
    if(user_config.capslock_change) {
        rgblight_mode(RGBLIGHT_MODE_RAINBOW_MOOD + 2);
    } else {
        rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
        rgblight_setrgb(RGB_RED);
    }
}
void keyboard_post_init_user(void) {
    rgblight_enable();
    rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
    rgblight_setrgb(RGB_RED);
    user_config.raw = eeconfig_read_user();
}

void eeconfig_init_user(void) {
    user_config.raw = 0;
    user_config.capslock_change = false;
    eeconfig_update_user(user_config.raw);
}

uint32_t layer_state_set_user(uint32_t state) {
    switch(biton32(state)) {
        case _FN:
            rgblight_mode(RGBLIGHT_MODE_CHRISTMAS);
            rgblight_setrgb(RGB_GREEN);
        break;
        default:
            checkMode();
        break;
    }
    return state;
}
