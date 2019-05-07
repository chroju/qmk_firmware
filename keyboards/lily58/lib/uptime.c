#include <stdio.h>
#include "lily58.h"

char uptime_str[36] = {};

void set_uptime(void) {
  uint32_t uptime_millsec = timer_read32();
  uint32_t uptime_sec = uptime_millsec / 1000;
  int seconds = (int)(uptime_sec % 60);
  int minutes = (int)(uptime_sec / 60  % 60);
  int hour = (int)(uptime_sec / 60 / 60);
  snprintf(uptime_str, sizeof(uptime_str), "uptime: %02d:%02d:%02d", hour, minutes, seconds);
}

const char *read_uptime(void) {
  return uptime_str;
}
