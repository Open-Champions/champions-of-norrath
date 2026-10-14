#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4,
              "target scalar representation");
static_assert(sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4,
              "target32 byte-view address bridge");
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target byte order");
extern "C" {
int norrath_string_compare(const char* left, const char* right);
unsigned char* norrath_lump_find(unsigned char* lump, char* key);
}
