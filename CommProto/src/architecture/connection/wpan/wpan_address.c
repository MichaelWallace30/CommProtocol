#include <CommProto/architecture/connection/wpan/wpan_address.h>

#include <ctype.h>
#include <string.h>

const address64 WPAN_IEEE_ADDRESS_UNDEFINED = { 
  { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } 
} ;


const address64 WPAN_IEEE_ADDRESS_BROADCAST = {
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF }
};


const address64 WPAN_IEEE_ADDRESS_COORDINATOR = {
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};


char* format_address64(char* buffer, const address64* addr) {
  int i;
  const uint8_t* b;
  char* p;
  uint_fast8_t ch;

  p = buffer;
  b = addr->b;
  for (i = 8;;) {
    ch = *b++;
    *p++ = "0123456789abcdef"[ch >> 4];
    *p++ = "0123456789abcdef"[ch & 0x0F];
  
    if (--i) {
      *p++ = ADDR64_FORMAT_SEPARATOR;  
    } else {
      *p = '\0';
      break;  
    }
  }

  return buffer;
}


bool_t address64_is_equal(const address64* addr1, const address64* addr2) {
  return (addr1 && addr2 && addr1->l[0] == addr2->l[0] && addr1->l[1] == addr2->l[1]);
}


bool_t address64_is_zero(const address64* addr) {
  return !(addr && (addr->l[0] || addr->l[1]));
}


int32_t address64_parse(address64* addr, const char* str) {
  uint_fast8_t i;
  uint8_t* b;
  int32_t ret;
  
  i = 8;
  if (str != NULL && addr != NULL) {
    while (isspace((uint8_t ) *str)) {
      ++str;
    }

    for (b = addr->b; i; ++b, --i) {
      ret = xbee_hex_str_to_bytes(str);
      if (ret < 0) {
        break;
      }

      *b = (uint8_t )ret;
      str += 2;

      if (*str && !isxdigit((uint8_t ) *str)) {
        ++str;
      }
    }
  }

  if (i == 0) {
    return 0;
  }

  if (addr != NULL) {
    *addr = *WPAN_IEEE_ADDR_ALL_ZEROS;
  }

  return XBEE_INVALID;
};