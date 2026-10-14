// Host contract for worldHitscanWithObjects; not an EE input.
#include "world_hitscan_with_objects.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>
static float wallResult; static int hitscans, boxes; static int lastFlags;
static std::vector<void*> listed; static std::vector<std::vector<float>> boxLows, boxHighs;
float worldHitscan__FP12_worldHeaderG6Point3T1fi(void*, const float*, const float*, int flags) { ++hitscans; lastFlags = flags; return wallResult; }
void objectFindInBox__FG6Point3T0PP10GameObjecti(const float* low, const float* high, void** list, int capacity) {
    ++boxes; boxLows.push_back({low[0], low[1], low[2]}); boxHighs.push_back({high[0], high[1], high[2]});
    if (capacity != 256) std::puts("fail: capacity");
    for (size_t i = 0; i < listed.size(); ++i) list[i] = listed[i];
    list[listed.size()] = nullptr;
}
struct Obj { alignas(8) unsigned char b[64]; };
static Obj make(float x, float y, unsigned flags, float radius) {
    Obj o; std::memset(o.b, 0, sizeof o.b);
    std::memcpy(o.b + 12, &flags, 4); std::memcpy(o.b + 16, &x, 4); std::memcpy(o.b + 20, &y, 4); std::memcpy(o.b + 28, &radius, 4);
    return o;
}
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main() {
    int bad = 0;
    float start[3] = {0, 0, 0}, dir[3] = {100, 0, 0};
    void* hit = reinterpret_cast<void*>(1);
    // non-positive limit: -1 with no calls and the slot untouched
    hitscans = boxes = 0; wallResult = 0.5f;
    if (worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(nullptr, start, dir, 0.0f, nullptr, &hit, 0, 0) != -1.0f || hitscans || hit != reinterpret_cast<void*>(1)) { std::puts("fail: zero limit"); bad = 1; }
    // tiny direction: the slot is cleared, result -1, no box query
    float tiny[3] = {0.001f, 0.001f, 5.0f};
    wallResult = -1.0f;
    if (worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(nullptr, start, tiny, 1.0f, nullptr, &hit, 7, 0) != -1.0f || hit != nullptr || boxes != 0 || lastFlags != 7) { std::puts("fail: tiny"); bad = 1; }
    // no wall, no objects: -1 (nothing hit)
    hitscans = boxes = 0; boxLows.clear(); boxHighs.clear(); listed.clear();
    float r = worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(nullptr, start, dir, 1.0f, nullptr, &hit, 0, 0);
    if (r != -1.0f || boxes != 1 || !near(boxHighs[0][0], 240.0f) || !near(boxLows[0][0], 0.0f) || boxLows[0][2] != -10000.0f || boxHighs[0][2] != 10000.0f) { std::puts("fail: empty scan"); bad = 1; }
    // a wall hit gives the wall fraction when no object is nearer
    wallResult = 0.25f; boxes = 0;
    r = worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(nullptr, start, dir, 1.0f, nullptr, &hit, 0, 0);
    if (!near(r, 0.25f) || hit != nullptr || boxes != 1) { std::puts("fail: wall only"); bad = 1; }
    // objects: radius 10 at (50, 5) is entered at 50 - sqrt(100 - 25) = 41.34 -> fraction 0.4134; a closer ignored
    // one, a flagged one, a one out of the beam and one behind the start do not count
    Obj good = make(50, 5, 0, 10), ignored = make(20, 0, 0, 10), flagged = make(10, 0, 1, 10), wide = make(60, 11, 0, 10), behind = make(-30, 0, 0, 5), big = make(40, 0, 0x40000, 0);
    listed = {&flagged, &ignored, &wide, &behind, &good};
    wallResult = -1.0f; boxes = 0;
    r = worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(nullptr, start, dir, 1.0f, &ignored, &hit, 0, 0);
    if (!near(r, (50.0f - std::sqrt(75.0f)) / 100.0f) || hit != &good) { std::printf("fail: object hit %f\n", r); bad = 1; }
    // flagged objects count only with allow and bit 0x40000; the 60.0 radius applies then
    Obj fl = make(100, 0, 0x40001, 3);
    listed = {&fl};
    r = worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(nullptr, start, dir, 3.0f, nullptr, &hit, 0, 0);
    if (r != -1.0f || hit != nullptr) { std::puts("fail: flagged without allow"); bad = 1; }
    r = worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(nullptr, start, dir, 3.0f, nullptr, &hit, 0, 1);
    if (!near(r, 0.4f) || hit != &fl) { std::printf("fail: allowed flagged %f\n", r); bad = 1; }
    // long rays walk several boxes and move the cursor
    listed.clear(); boxes = 0; boxLows.clear(); boxHighs.clear();
    worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(nullptr, start, dir, 5.0f, nullptr, nullptr, 0, 0);
    if (boxes != 3 || !near(boxLows[2][0], 480.0f) || !near(boxHighs[2][0], 720.0f)) { std::printf("fail: long ray %d\n", boxes); bad = 1; }
    if (!bad) std::puts("worldHitscanWithObjects ok");
    return bad;
}
