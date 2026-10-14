// Host contract for Reset__5Wheel. Not an EE plan input.
#include "reset_wheel.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    unsigned char wheel[416];
    unsigned char want[416];
    std::memset(wheel, 0xa5, sizeof wheel);
    std::memcpy(want, wheel, sizeof wheel);
    Reset__5Wheel(wheel);
    const int cleared[] = {360, 364, 368, 392};
    for (int index = 0; index < 4; ++index) std::memset(want + cleared[index], 0, 4);
    expect(std::memcmp(wheel, want, sizeof wheel) == 0, "exactly words 360, 364, 368 and 392 are cleared");
    expect(wheel[372] == 0xa5 && wheel[388] == 0xa5 && wheel[396] == 0xa5, "neighbours are kept");
    return failures ? 1 : 0;
}
