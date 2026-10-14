// Host check for setupHalo: level selection, persisted halo bytes, converted fields, mode argument.
#include "setup_halo.cpp"
#include <cstring>
static int calls; static int last_mode; static unsigned char seen[16]; static const void* seen_ptr;
extern "C" void modelSetHalo__FP7HaloDefi(void* h, int mode) { ++calls; last_mode = mode; seen_ptr = h; std::memcpy(seen, h, 16); }
int main() {
    alignas(8) unsigned char record[32] = {};
    // Initial bytes of the persistent record (no call has changed it yet).
    modelSetHalo__FP7HaloDefi(&halo, 99);
    const unsigned char start[16] = {0x0a, 0x0a, 0x44, 0x40, 0, 0, 0x48, 0x43, 0x3c, 0x3c, 0x44, 0x48, 0, 0, 0xc8, 0x42};
    if (std::memcmp(seen, start, 16) != 0) return 1;
    calls = 0;
    setupHalo__4itemPQ24item11descriptionf(record, 3.0f);                  // bit clear: level 0
    if (calls != 1 || last_mode != 0 || std::memcmp(seen, start, 16) != 0) return 2;
    unsigned long long flags = 0x200000000ull;
    std::memcpy(record + 8, &flags, 8);
    setupHalo__4itemPQ24item11descriptionf(record, 2.0f);                  // bit set: level 7, s = 0.775
    if (calls != 2 || last_mode != 2 || seen_ptr != &halo) return 3;
    float first, second; std::memcpy(&first, seen + 4, 4); std::memcpy(&second, seen + 12, 4);
    if (first != 400.0f || second != 200.0f) return 4;
    const float s = 7.0f * __builtin_bit_cast(float, 0x3ccccccc) + __builtin_bit_cast(float, 0x3f19999a);
    if (seen[3] != static_cast<int>(s * 64.0f) || seen[11] != static_cast<int>(s * 72.0f)) return 5;
    if (seen[3] != 49 || seen[11] != 55) return 6;                         // 0.775 * 64 = 49.6, * 72 = 55.8
    if (seen[0] != 0x0a || seen[1] != 0x0a || seen[2] != 0x44 || seen[8] != 0x3c || seen[9] != 0x3c || seen[10] != 0x44) return 7;
    flags = 0x400000000ull | 0x1ffffffffull;                               // other bits, bit 33 clear
    std::memcpy(record + 8, &flags, 8);
    setupHalo__4itemPQ24item11descriptionf(record, 9.0f);
    if (calls != 3 || last_mode != 0) return 8;
    std::memcpy(&first, seen + 4, 4);
    if (first != 400.0f) return 9;                                         // level 0 does not rewrite the fields
    return 0;
}
