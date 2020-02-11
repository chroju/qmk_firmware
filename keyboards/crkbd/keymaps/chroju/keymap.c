#include QMK_KEYBOARD_H
#include "bootloader.h"
#ifdef PROTOCOL_LUFA
  #include "lufa.h"
  #include "split_util.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif

extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2
#define _ADJUST 3

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  RGBRST
};

enum {
  TD_CTL_GUI = 0,
  TD_LCBRS,
  TD_RCBRS,
  TD_LANG
};

enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3,
};

typedef struct {
  bool is_press_action;
  int state;
} tap;

int lang_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (!state->pressed) return SINGLE_TAP;
    else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    return DOUBLE_TAP;
  }
  else return 6; //magic number. At some point this method will expand to work for more presses
}

//instanalize an instance of 'tap' for the 'x' tap dance.
static tap xtap_state = {
  .is_press_action = true,
  .state = 0
};

void x_finished_1 (qk_tap_dance_state_t *state, void *user_data) {
  xtap_state.state = lang_dance(state);
  switch (xtap_state.state) {
    case SINGLE_TAP:                     // 単押しで「英数」と「無変換」　Lowerレイヤーがトグルされている場合はレイヤーをオフにする
        register_code(KC_F21);
        register_code(KC_LANG2);
        break;
    case SINGLE_HOLD:                   // 長押しでLowerレイヤーをオンにする
        layer_on(_RAISE);
        break;
    case DOUBLE_TAP:                    // ダブルタップでLowerレイヤーをトグル
        register_code(KC_F22);
        register_code(KC_LANG1);
        break;
  }
}

void x_reset_1 (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_state.state) {
    case SINGLE_TAP:
        unregister_code(KC_F21);
        unregister_code(KC_LANG2);
        break;
    case SINGLE_HOLD:
        layer_off(_RAISE);
        break;
    case DOUBLE_TAP:
        unregister_code(KC_F22);
        unregister_code(KC_LANG1);
        break;
  }
  xtap_state.state = 0;
}

void dance_left_brackets (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    register_code (KC_LBRC);
    unregister_code (KC_LBRC);
  } else if (state->count == 2) {
    register_code (KC_LSFT);
    register_code (KC_9);
    unregister_code (KC_9);
    unregister_code (KC_LSFT);
  } else if (state->count == 3) {
    register_code (KC_LSFT);
    register_code (KC_LBRC);
    unregister_code (KC_LBRC);
    unregister_code (KC_LSFT);
  }
};

void dance_right_brackets (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    register_code (KC_RBRC);
    unregister_code (KC_RBRC);
  } else if (state->count == 2) {
    register_code (KC_LSFT);
    register_code (KC_0);
    unregister_code (KC_0);
    unregister_code (KC_LSFT);
  } else if (state->count == 3) {
    register_code (KC_RSFT);
    register_code (KC_RBRC);
    unregister_code (KC_RBRC);
    unregister_code (KC_RSFT);
  }
};

qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_CTL_GUI] = ACTION_TAP_DANCE_DOUBLE(KC_LCTRL, KC_LGUI),
  [TD_LCBRS] = ACTION_TAP_DANCE_FN(dance_left_brackets),
  [TD_RCBRS] = ACTION_TAP_DANCE_FN(dance_right_brackets),
  [TD_LANG]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, x_finished_1, x_reset_1)
};

#define KC______ KC_TRNS
#define KC_XXXXX KC_NO
#define KC_LOWER LOWER
#define KC_RAISE RAISE
#define KC_RST   RESET
#define KC_LRST  RGBRST
#define KC_LTOG  RGB_TOG
#define KC_LHUI  RGB_HUI
#define KC_LHUD  RGB_HUD
#define KC_LSAI  RGB_SAI
#define KC_LSAD  RGB_SAD
#define KC_LVAI  RGB_VAI
#define KC_LVAD  RGB_VAD
#define KC_LMOD  RGB_MOD
#define KC_SWAP  LCG_SWP
#define KC_NRML  LCG_NRM
#define KC_CAD   LCTL(LALT(KC_DEL))
#define KC_EMJI  LGUI(LCTL(KC_SPACE))
#define KC_SFTESC LSFT_T(KC_ESC)
#define KC_RASLNG TD(TD_LANG)
#define KC_ENTSFT LSFT_T(KC_ENTER)
#define KC_SPCALT LALT_T(KC_SPACE)
#define KC_LCBRS TD(TD_LCBRS)
#define KC_RCBRS TD(TD_RCBRS)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        TAB,     Q,     W,     E,     R,     T,                      Y,     U,     I,     O,     P,  BSLS,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LCTL,     A,     S,     D,     F,     G,                      H,     J,     K,     L,  SCLN,  QUOT,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
     SFTESC,     Z,     X,     C,     V,     B,                      N,     M,  COMM,   DOT,  SLSH,  EMJI,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                   LGUI, LOWER,SPCALT,   ENTSFT,RASLNG,  BSPC \
                              //`--------------------'  `--------------------'
  ),

  [_RAISE] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
       LRST,    F1,    F2,    F3,    F4,    F5,                     F6,    F7,    F8,    F9,   F10,   F12,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LTOG,  LHUI,  LSAI,  LVAI, XXXXX,  SWAP,                   LEFT,  DOWN,    UP, RIGHT,  VOLU,  BRIU,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LMOD,  LHUD,  LSAD,  LVAD, XXXXX,  NRML,                  XXXXX, XXXXX, XXXXX,  MUTE,  VOLD,  BRID,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                   LGUI, LOWER,SPCALT,   ENTSFT,RASLNG,  BSPC \
                              //`--------------------'  `--------------------'
  ),

  [_LOWER] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
       EXLM,     1,     2,     3,     4,     5,                      6,     7,     8,     9,     0, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       TILD,    AT,  HASH,   DLR,  PERC, LCBRS,                  RCBRS,  MINS,   EQL,  ASTR, XXXXX, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
        GRV, XXXXX, XXXXX,  CIRC,  AMPR,  LCBR,                   RCBR,  UNDS,  PLUS, XXXXX, XXXXX, XXXXX,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                   LGUI, LOWER,SPCALT,   ENTSFT,RASLNG,  BSPC \
                              //`--------------------'  `--------------------'
  )
};

int RGB_current_mode;

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

// Setting ADJUST layer RGB back to default
/* void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) { */
/*   if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) { */
/*     layer_on(layer3); */
/*   } else { */
/*     layer_off(layer3); */
/*   } */
/* } */

void matrix_init_user(void) {
    #ifdef RGBLIGHT_ENABLE
      RGB_current_mode = rgblight_config.mode;
    #endif
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
        iota_gfx_init(!has_usb());   // turns on the display
    #endif
}

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

// When add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_bootmagic_state(bool swap);
const char *read_logo(void);
void set_uptime(void);
const char *read_uptime(void);

void matrix_scan_user(void) {
   iota_gfx_task();
}

void matrix_render_user(struct CharacterMatrix *matrix) {
  if (is_master) {
    matrix_write_ln(matrix, read_bootmagic_state(keymap_config.swap_lctl_lgui));
    matrix_write_ln(matrix, read_layer_state());
    matrix_write(matrix, read_uptime());
  } else {
    matrix_write(matrix, read_logo());
  }
}

void matrix_update(struct CharacterMatrix *dest, const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;
  matrix_clear(&matrix);
  matrix_render_user(&matrix);
  matrix_update(&display, &matrix);
}
#endif//SSD1306OLED

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
#ifdef SSD1306OLED
    set_uptime();
#endif
    // set_timelog();
  }

  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        persistent_default_layer_set(1UL<<_QWERTY);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        /* update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST); */
      } else {
        layer_off(_LOWER);
        /* update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST); */
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        /* update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST); */
      } else {
        layer_off(_RAISE);
        /* update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST); */
      }
      return false;
      break;
    case RGB_MOD:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          rgblight_mode(RGB_current_mode);
          rgblight_step();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      return false;
      break;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      break;
  }
  return true;
}
