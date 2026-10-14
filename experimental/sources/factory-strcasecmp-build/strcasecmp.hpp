#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4, "target scalar representation");
static_assert(sizeof(void*) == 4 && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target32 little endian addresses");
extern "C" {
// Original mapped minimum view 0x47e171..0x47e2f0. This is NOT a declared
// 384-byte game allocation: it spans bytes preceding the _ctype_ symbol.
// The original classification center is byte128 of this imported view.
extern const unsigned char norrath_factory_ctype_minimum_view[];
int norrath_factory_strcasecmp(const char* left, const char* right);
}
