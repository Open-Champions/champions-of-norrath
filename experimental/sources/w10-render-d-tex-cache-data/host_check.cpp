// Host check for texCacheData: ring push, wrap modulo 64, full ring rejects without writing.
#include <cstdio>
struct DecodeRequest { void* texture; int value; };
DecodeRequest decodeQ[64];
int decodeQHead;
int decodeQTail;
extern "C" void texCacheData__FP8_texturei(void* texture, int value);
static int fails;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
int main() {
    int t[4];
    texCacheData__FP8_texturei(&t[0], 11);
    CHECK(decodeQHead == 1 && decodeQ[0].texture == &t[0] && decodeQ[0].value == 11);
    decodeQHead = 63; decodeQTail = 5;
    texCacheData__FP8_texturei(&t[1], 22);
    CHECK(decodeQHead == 0 && decodeQ[63].texture == &t[1] && decodeQ[63].value == 22);
    decodeQHead = 9; decodeQTail = 10;  // full: next head equals tail
    decodeQ[9].texture = &t[3]; decodeQ[9].value = 33;
    texCacheData__FP8_texturei(&t[2], 44);
    CHECK(decodeQHead == 9 && decodeQ[9].texture == &t[3] && decodeQ[9].value == 33);
    decodeQHead = 63; decodeQTail = 0;  // full across the wrap
    texCacheData__FP8_texturei(&t[2], 44);
    CHECK(decodeQHead == 63);
    if (fails) return 1;
    puts("texCacheData ok");
    return 0;
}
