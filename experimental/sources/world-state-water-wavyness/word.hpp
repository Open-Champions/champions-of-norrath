#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(int) == 4 && sizeof(float) == 4, "target scalar representation");
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target byte order");
extern "C" {
extern float worldWaterWavyness;
}
