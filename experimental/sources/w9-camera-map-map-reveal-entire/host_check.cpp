// Host contract for revealEntireMap__3mapv. Not an EE plan input.
#include "reveal_entire.cpp"

#include <cstdio>
#include <cstring>

int _3map$mapwidth;
int _3map$mapheight;
int _3map$revealed;
unsigned char _3map$maptiles[432 * 40];

static int failures = 0;
static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    const int sizes[][2] = {{0, 0}, {3, 0}, {0, 3}, {5, 4}, {216, 2}, {1, 1}, {-2, 5}, {5, -2}};
    for (const auto& s : sizes) {
        for (int i = 0; i < (int)sizeof _3map$maptiles; ++i) _3map$maptiles[i] = (unsigned char)(i * 13 + 5);
        unsigned char before[sizeof _3map$maptiles];
        std::memcpy(before, _3map$maptiles, sizeof before);
        _3map$mapwidth = s[0];
        _3map$mapheight = s[1];
        _3map$revealed = 0;
        revealEntireMap__3mapv();
        expect(_3map$revealed == 1, "revealed flag set");
        for (int y = 0; y < 40; ++y) {
            for (int x = 0; x < 216; ++x) {
                const int at = 432 * y + 2 * x;
                const bool inside = y < s[1] && x < s[0];
                expect(_3map$maptiles[at] == before[at], "byte 0 untouched");
                const unsigned char want = inside ? (before[at + 1] | 0xf0) : before[at + 1];
                expect(_3map$maptiles[at + 1] == want, "byte 1 high nibble");
            }
        }
        expect(_3map$mapwidth == s[0] && _3map$mapheight == s[1], "bounds untouched");
    }
    return failures ? 1 : 0;
}
