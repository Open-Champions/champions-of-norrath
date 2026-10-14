// Host contract for initTrails__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" void initTrails__Fv();
extern "C" unsigned char trail[64 * 1040] = {};
extern "C" unsigned char bolt[20 * 1844] = {};
extern "C" unsigned trailSequenceNm = 0;

int main() {
    std::memset(trail, 0xa5, sizeof trail);
    std::memset(bolt, 0x5a, sizeof bolt);
    trailSequenceNm = 4u;
    initTrails__Fv();
    if (trailSequenceNm != 1u) {
        std::fprintf(stderr, "fail: trailSequenceNm\n");
        return 1;
    }
    for (int index = 0; index < 64; ++index) {
        const unsigned char* record = trail + index * 1040;
        if (record[0] != 0 || record[1] != 0 || record[2] != 0xa5) {
            std::fprintf(stderr, "fail: trail %d\n", index);
            return 1;
        }
    }
    for (int index = 0; index < 20; ++index) {
        const unsigned char* record = bolt + index * 1844;
        if (record[1835] != 0x5a || record[1836] != 0 || record[1837] != 0x5a) {
            std::fprintf(stderr, "fail: bolt %d\n", index);
            return 1;
        }
    }
    return 0;
}
