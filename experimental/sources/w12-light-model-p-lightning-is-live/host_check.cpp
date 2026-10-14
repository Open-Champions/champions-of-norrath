// Host contract for P_LightningIsLive__Fi. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" {
unsigned char bolt[20 * 1844];
int P_LightningIsLive__Fi(int);
}
static void set(int slot, int handle, int flag, int a, int b) {
    unsigned char* r = bolt + slot * 1844;
    std::memcpy(r + 1840, &handle, 4);
    r[1836] = static_cast<unsigned char>(flag);
    std::memcpy(r + 1824, &a, 4);
    std::memcpy(r + 1828, &b, 4);
}
int main() {
    int failures = 0;
    auto expect = [&](int handle, int want, const char* what) {
        int got = P_LightningIsLive__Fi(handle);
        if (got != want) { std::fprintf(stderr, "fail: %s handle %d got %d\n", what, handle, got); ++failures; }
    };
    set(3, 0x103, 1, 7, 9);
    expect(0x103, 1, "live");
    expect(0, 0, "zero handle");
    expect(0x203, 0, "other generation, same slot");
    set(3, 0x103, 0, 7, 9); expect(0x103, 0, "flag clear");
    set(3, 0x103, 1, 0, 9); expect(0x103, 0, "no first trail");
    set(3, 0x103, 1, 7, 0); expect(0x103, 0, "no second trail");
    set(3, 0x103, 0xff, 7, 9); expect(0x103, 1, "any nonzero flag byte");
    if (failures) return 1;
    std::printf("P_LightningIsLive contract passed\n");
    return 0;
}
