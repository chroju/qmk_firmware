
#include QMK_KEYBOARD_H
#include <stdio.h>
#include "crkbd.h"

char                   bootmagic_state_str[24];
extern keymap_config_t keymap_config;

const char *read_bootmagic_state(void) {
  if (keymap_config.swap_lalt_lgui) {
    snprintf(bootmagic_state_str, sizeof(bootmagic_state_str), "Mode: Win");
  } else {
    snprintf(bootmagic_state_str, sizeof(bootmagic_state_str), "Mode: macOS");
  }

  return bootmagic_state_str;
}
