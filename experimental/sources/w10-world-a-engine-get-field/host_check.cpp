// Host contract for engineGetField__Fv. Not an EE plan input.
#include "engine_get_field.cpp"

extern "C" int draw[16] = {};

int main() {
    if (engineGetField__Fv() != 0) return 1;
    draw[2] = 0x12345678;
    if (engineGetField__Fv() != 0x12345678) return 2;
    // neighbouring words must not be read
    draw[1] = -1; draw[3] = -1; draw[2] = 0;
    if (engineGetField__Fv() != 0) return 3;
    draw[2] = -0x7fffffff;
    if (engineGetField__Fv() >= 0) return 4;
    return 0;
}
