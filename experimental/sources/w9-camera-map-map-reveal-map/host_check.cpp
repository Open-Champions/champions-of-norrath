// Host contract for revealMap__3mapG6Point3. Not an EE plan input.
#include "reveal_map.cpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

int _3map$mapwidth;
int _3map$mapheight;
unsigned char _3map$maptiles[432 * 64];

static float stub_x, stub_y;
static int stub_calls;
static float stub_seen[2];
extern "C" void worldToMap__3mapP6Point2PfT2(const float* point, float* out_x, float* out_y) {
    ++stub_calls;
    stub_seen[0] = point[0];
    stub_seen[1] = point[1];
    *out_x = stub_x;
    *out_y = stub_y;
}

static int failures = 0;
static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

// Independent model: visit every tile of the map and decide membership by
// distance, then apply the nibble rule with doubles.
static void model(unsigned char* tiles, int width, int height, float mx, float my) {
    for (int yy = 0; yy < height; ++yy) {
        for (int xx = 0; xx < width; ++xx) {
            const bool in_x = xx >= std::max(0.0, std::round(double(mx) - 8.0)) && xx <= std::round(double(mx) + 8.0);
            const bool in_y = yy >= std::max(0.0, std::round(double(my) - 8.0)) && yy <= std::round(double(my) + 8.0);
            const float dx = mx - float(xx), dy = my - float(yy);
            const float d2 = dx * dx + dy * dy;
            if (!in_x || !in_y || d2 > 64.0f) continue;
            double v = std::round((1.0 - std::sqrt(double(d2)) * 0.125) * 254.0);
            int iv = int(v);
            if (iv >= 256) iv = 255;
            iv >>= 4;
            unsigned char& b = tiles[432 * yy + 2 * xx + 1];
            int hi = b >> 4;
            if (hi < iv) hi = iv;
            b = (unsigned char)((b & 0xf) | (hi << 4));
        }
    }
}

int main() {
    const float centres[] = {-20.0f, -8.5f, -3.25f, 0.0f, 0.5f, 4.75f, 10.0f, 23.5f, 30.25f, 40.0f, 60.0f};
    const int dims[][2] = {{20, 20}, {40, 33}, {7, 50}, {0, 5}, {5, 0}};
    int cases = 0;
    for (const auto& d : dims) {
        for (float cx : centres) {
            for (float cy : centres) {
                static unsigned char want[sizeof _3map$maptiles];
                for (int i = 0; i < (int)sizeof want; ++i) _3map$maptiles[i] = want[i] = (unsigned char)((i * 37 + 11) & 0xff);
                // Make some tiles already highly revealed.
                for (int i = 1; i < (int)sizeof want; i += 64) _3map$maptiles[i] = want[i] = 0xfa;
                _3map$mapwidth = d[0];
                _3map$mapheight = d[1];
                stub_x = cx; stub_y = cy;
                stub_calls = 0;
                const float point[3] = {123.0f, -456.0f, 789.0f};
                revealMap__3mapG6Point3(point);
                model(want, d[0], d[1], cx, cy);
                expect(stub_calls == 1, "one conversion call");
                expect(stub_seen[0] == 123.0f && stub_seen[1] == -456.0f, "x and y go to the conversion");
                expect(std::memcmp(want, _3map$maptiles, sizeof want) == 0, "tile bytes match the model");
                ++cases;
            }
        }
    }
    // One concrete tile: centre on a tile gives v = 254 >> 4 = 15.
    for (int i = 0; i < 432; ++i) _3map$maptiles[i] = 0x01;
    _3map$mapwidth = 10; _3map$mapheight = 1;
    stub_x = 3.0f; stub_y = 0.0f;
    const float p[3] = {0, 0, 0};
    revealMap__3mapG6Point3(p);
    expect(_3map$maptiles[2 * 3 + 1] == 0xf1, "centre tile reaches 15 and keeps the low nibble");
    expect(_3map$maptiles[2 * 3] == 0x01, "byte 0 untouched");
    expect(_3map$maptiles[2 * 4 + 1] >> 4 == ((int)std::round((1.0 - 0.125) * 254.0) >> 4), "neighbour tile");
    expect(cases == 605, "all combinations visited");
    return failures ? 1 : 0;
}
