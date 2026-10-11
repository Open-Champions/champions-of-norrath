#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4, "target scalar representation");
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target byte order");
extern "C" {
extern unsigned char norrath_loader_current_name_view[80];
}
