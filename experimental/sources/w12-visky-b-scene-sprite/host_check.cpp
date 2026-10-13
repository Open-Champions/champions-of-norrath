static unsigned char* host_records;
#define NORRATH_WORD_ADDRESS(word) (host_records + ((word) - 0x5000u))
#include "sprite.cpp"

int main() {
    alignas(8) static unsigned char scene[2048];
    alignas(8) static unsigned char records[12 * 6];
    host_records = records;
    for (int i = 0; i < 6; ++i) {
        unsigned* r = reinterpret_cast<unsigned*>(records + i * 12);
        r[0] = 0xa0000000u + i;
        r[1] = 0xbad00000u + i;
        r[2] = 0xbad10000u + i;
    }
    *reinterpret_cast<unsigned*>(scene + 1004) = 0x5000;
    for (int i = 0; i < 6; ++i)
        if (Sprite__7VIScenei(scene, i) != 0xa0000000u + i) return 1 + i;
    *reinterpret_cast<unsigned*>(scene + 1004) = 0x5000 + 12;
    if (Sprite__7VIScenei(scene, 0) != 0xa0000001u) return 10;
    if (Sprite__7VIScenei(scene, 2) != 0xa0000003u) return 11;
    return 0;
}
