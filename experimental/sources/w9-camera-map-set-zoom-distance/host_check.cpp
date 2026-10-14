// Host contract for SetZoomDistance__6Cameraf. Not an EE plan input.
#include "set_zoom_distance.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static float get(const unsigned char* c, int off) { float v; std::memcpy(&v, c + off, 4); return v; }
static void put(unsigned char* c, int off, float v) { std::memcpy(c + off, &v, 4); }
static void putw(unsigned char* c, int off, int v) { std::memcpy(c + off, &v, 4); }

int main() {
    unsigned char camera[400], before[400];
    const float blends[] = {-0.5f, 0.0f, 0.25f, 1.0f, 3.0f};
    const float dists[] = {170.0f, 300.0f, 500.0f, 600.0f, 100.0f};
    for (float blend : blends) {
        for (float d : dists) {
            for (int a = 0; a < 3; ++a) {
                for (int b = 0; b < 3; ++b) {
                    std::memset(camera, 0, sizeof camera);
                    put(camera, 176, 170.0f); put(camera, 184, 500.0f);
                    put(camera, 144, blend);
                    putw(camera, 84, a); putw(camera, 88, b);
                    put(camera, 188, 90.0f); put(camera, 192, 60.0f); put(camera, 196, 30.0f);
                    std::memcpy(before, camera, sizeof camera);
                    SetZoomDistance__6Cameraf(camera, d);
                    const float lim = blend < 1.0f ? blend : 1.0f;
                    const float t[3] = {90.0f, 60.0f, 30.0f};
                    expect(get(camera, 140) == d, "distance stored");
                    expect(get(camera, 216) == 1.0f - (d - 170.0f) / (500.0f - 170.0f), "ratio");
                    expect(get(camera, 252) == t[b] + (t[a] - t[b]) * lim, "blend of the two entries");
                    unsigned char want[400];
                    std::memcpy(want, before, sizeof want);
                    put(want, 140, get(camera, 140)); put(want, 216, get(camera, 216)); put(want, 252, get(camera, 252));
                    expect(std::memcmp(want, camera, sizeof camera) == 0, "only +140, +216, +252 change");
                }
            }
        }
    }
    return failures ? 1 : 0;
}
