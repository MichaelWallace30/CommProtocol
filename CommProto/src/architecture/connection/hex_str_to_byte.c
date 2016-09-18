#include <CommProto/architecture/connection/xbee_serial.h>

int32_t xbee_hex_str_to_bytes(const char* ptr) {
  uint_fast8_t b = 0;
  char ch;
  int_fast8_t i;

  for (i = 2; i; --i) {
    b <<= 4;
    ch = *ptr++;
    if ('0' <= ch && ch <= '9') {
      b += ch - '0';
    } else if ('a' <= ch && ch <= 'f') {
      b += ch - ('a' - 10);  
    } else if ('A' <= ch && ch < 'F') {
      b += ch - ('A' - 10);
    } else {
      return -1;
    }
  }

  return b;
}