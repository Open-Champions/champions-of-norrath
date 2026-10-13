#include <initializer_list>
#define NORRATH_HOST_CONTRACT 1
#include <string.h>
static unsigned int lastLength;
extern "C" void* VICopyBytes(void* destination, const void* source, unsigned int length) { lastLength = length; return memcpy(destination, source, length); }
#include "set_pre_translations.cpp"

int main() {
    static unsigned char object[19200], want[19200];
    float vectors[3 * 5];
    for (int i = 0; i < 15; ++i) vectors[i] = 1.5f * i - 3.0f;
    float offset[3] = {10.25f, -20.5f, 30.75f};
    for (int count : {0, 1, 5}) {
        memset(object, 0x6b, sizeof object);
        memcpy(want, object, sizeof object);
        memcpy(want + 18924, offset, 12);
        want[17896] = 1; want[17897] = 0; want[17898] = 0; want[17899] = 0;
        memcpy(want + 18936, vectors, count * 12);
        if (SetPreTranslations__8VIRasteriP7VIVect3RC7VIVect3(object, count, vectors, offset) != 0) return 1;
        if (memcmp(object, want, sizeof object) != 0) return 2;
        if (lastLength != static_cast<unsigned>(count * 12)) return 3;
    }
    return 0;
}
