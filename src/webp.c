#include "emscripten.h"
#include "calendar.h"

EMSCRIPTEN_KEEPALIVE
int getDayOfWeek() {
  return dayOfWeekParsed(15, 4, 2019);
}
