#include <stdio.h>
#include "crkbd.h"

char typespeed_str[70];
uint32_t type_count = 0;

void set_typespeed(void){
    type_count++;

    uint32_t uptime_millsec = timer_read32();
    uint32_t minutes = uptime_millsec / 60000;
    if (minutes == 0) {
      minutes = 1;
    }
    float type_speed = (float)type_count / minutes;

    char str_type_speed[12];
    dtostrf(type_speed, 6, 2, str_type_speed);

    /* snprintf(typespeed_str, sizeof(typespeed_str), "Speed: %d keys/m", type_speed); */
    snprintf(typespeed_str, sizeof(typespeed_str), "Speed: %s keys/m", str_type_speed);
}

const char *read_typespeed(void) {
  return typespeed_str;
}
