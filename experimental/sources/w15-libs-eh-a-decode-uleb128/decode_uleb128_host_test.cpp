// Host check only; not an EE build input. Build the source with -Ddecode_uleb128=w15_decode_uleb128.
#include <cstdio>
extern "C" const unsigned char* w15_decode_uleb128(const unsigned char* cursor, unsigned* value);
static int check(const unsigned char* in, unsigned expect, int used) {
    unsigned v = 0xdeadbeef;
    const unsigned char* end = w15_decode_uleb128(in, &v);
    return v == expect && end == in + used;
}
int main() {
    const unsigned char one[] = {0x00}; if (!check(one, 0, 1)) return 1;
    const unsigned char b7f[] = {0x7f}; if (!check(b7f, 0x7f, 1)) return 2;
    const unsigned char two[] = {0x80, 0x01}; if (!check(two, 128, 2)) return 3;
    const unsigned char ff7f[] = {0xff, 0x7f}; if (!check(ff7f, 0x3fff, 2)) return 4;
    const unsigned char three[] = {0xe5, 0x8e, 0x26}; if (!check(three, 624485, 3)) return 5;
    const unsigned char five[] = {0xff, 0xff, 0xff, 0xff, 0x0f}; if (!check(five, 0xffffffffu, 5)) return 6;
    // Past 32 bits the shift wraps modulo 32, like the compiled shift.
    const unsigned char six[] = {0x80, 0x80, 0x80, 0x80, 0x80, 0x03}; if (!check(six, 0x03u << (35 & 31), 6)) return 7;
    // Padding bytes with the continuation bit still consume input.
    const unsigned char pad[] = {0x81, 0x80, 0x00}; if (!check(pad, 1, 3)) return 8;
    std::puts("decode_uleb128 ok");
    return 0;
}
