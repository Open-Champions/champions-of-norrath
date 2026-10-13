// Host contract for willDraw__3mapP12_worldHeaderf. Not an EE plan input.
#include "will_draw.cpp"

#include <cstdio>
#include <cstring>

int glistp;
int gstart;
int GLISTSIZE;

static int failures = 0;
static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    unsigned char header[160];
    for (int flags = 0; flags < 4; ++flags) {
        const int a = flags & 1, b = (flags >> 1) & 1;
        std::memset(header, 0, sizeof header);
        std::memcpy(header + 108, &a, 4);
        std::memcpy(header + 112, &b, 4);
        const bool gate = a != 0 || b != 0;
        // free = 20000 - (0x800 >> 3) = 19744: above the limit.
        GLISTSIZE = 20000; gstart = 0x1000; glistp = 0x1800;
        expect(willDraw__3mapP12_worldHeaderf(header, 5.0f) == (gate ? 1 : 0), "gate on the two header words");
        expect(willDraw__3mapP12_worldHeaderf(header, 2.0f) == (gate ? 1 : 0), "value 2 is enough");
        expect(willDraw__3mapP12_worldHeaderf(header, 1.99f) == 0, "truncates below 2");
        expect(willDraw__3mapP12_worldHeaderf(header, -3.0f) == 0, "negative value");
        expect(willDraw__3mapP12_worldHeaderf(header, 2.99f) == (gate ? 1 : 0), "truncation not rounding");
    }
    std::memset(header, 0, sizeof header);
    header[108] = 1;
    // Free space boundary: GLISTSIZE - (diff >> 3) must be at least 12000.
    GLISTSIZE = 12000 + 100; gstart = 0; glistp = 800;
    expect(willDraw__3mapP12_worldHeaderf(header, 9.0f) == 1, "free exactly 12000");
    glistp = 808;
    expect(willDraw__3mapP12_worldHeaderf(header, 9.0f) == 0, "free 11999");
    glistp = 807;
    expect(willDraw__3mapP12_worldHeaderf(header, 9.0f) == 1, "seven spare bytes do not count as a unit");
    glistp = 815;
    expect(willDraw__3mapP12_worldHeaderf(header, 9.0f) == 0, "a full eighth byte unit does");
    GLISTSIZE = 12000; gstart = 800; glistp = 0;
    expect(willDraw__3mapP12_worldHeaderf(header, 9.0f) == 1, "negative difference grows free space");
    GLISTSIZE = 11990; gstart = 800; glistp = 0;
    expect(willDraw__3mapP12_worldHeaderf(header, 9.0f) == 1, "difference -800 gives free 12090");
    // Only the word at +112 set.
    std::memset(header, 0, sizeof header);
    header[112] = 1;
    expect(willDraw__3mapP12_worldHeaderf(header, 9.0f) == 1, "word at +112 alone opens the gate");
    return failures ? 1 : 0;
}
