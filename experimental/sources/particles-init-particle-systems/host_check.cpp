// Host contract for initParticleSystems__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" void initParticleSystems__Fv();
extern "C" unsigned char PS[16 * 312] = {};
extern "C" unsigned psSequenceNm = 0;

int main() {
    std::memset(PS, 0xa5, sizeof PS);
    psSequenceNm = 9u;
    initParticleSystems__Fv();
    if (psSequenceNm != 1u) {
        std::fprintf(stderr, "fail: psSequenceNm\n");
        return 1;
    }
    for (int index = 0; index < 16; ++index) {
        const unsigned char* record = PS + index * 312;
        for (int byte = 0; byte < 312; ++byte) {
            const unsigned char expect = (byte >= 304 && byte < 308) ? 0 : 0xa5;
            if (record[byte] != expect) {
                std::fprintf(stderr, "fail: record %d byte %d\n", index, byte);
                return 1;
            }
        }
    }
    return 0;
}
