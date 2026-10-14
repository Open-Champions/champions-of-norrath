// Host check only; not a build input. Compiled with -ffp-contract=off.
#include <cstring>
static float view[16]; static int calls;
extern "C" const float* GetModelView__C8VIRaster(const unsigned char*) { ++calls; return view; }
#include "calc_lod_level_index.cpp"
#include <cstdio>
int main() {
    alignas(8) unsigned char sprite[256];
    std::memset(sprite, 0, sizeof sprite);
    float seg[3] = {1.0f, 2.0f, 3.0f};
    view[12] = 1.0f; view[13] = 2.0f; view[14] = 3.0f;
    float th[4] = {2.0f, 5.0f, 10.0f, 20.0f};
    std::memcpy(sprite + 172, th, sizeof th);
    *reinterpret_cast<int*>(sprite + 168) = 4;
    auto run = [&](float dx) {
        view[12] = 1.0f + dx; calls = 0;
        int r = CalcLODLevelIndex__9VIHSpriteR13VIPickSegmentR8VIRaster(sprite, reinterpret_cast<unsigned char*>(seg), nullptr);
        return r;
    };
    bool ok = run(0.0f) == -1 && calls == 3;
    ok = ok && run(1.0f) == -1;      // 1 < 4
    ok = ok && run(2.0f) == 0;       // 4 is not < 4, 4 < 25
    ok = ok && run(4.9f) == 0 && run(5.0f) == 1 && run(9.9f) == 1 && run(10.0f) == 2 && run(19.0f) == 2 && run(20.0f) == 3 && run(1000.0f) == 3;
    ok = ok && run(-5.0f) == 1;      // sign of the difference does not matter
    *reinterpret_cast<int*>(sprite + 168) = 0;
    ok = ok && run(100.0f) == -1;
    *reinterpret_cast<int*>(sprite + 168) = -3;
    ok = ok && run(100.0f) == -1;
    *reinterpret_cast<int*>(sprite + 168) = 4;
    float nan = 0.0f; nan = nan / nan;
    th[0] = nan; std::memcpy(sprite + 172, th, sizeof th);
    ok = ok && run(0.0f) == 0;       // a NaN threshold never stops the walk; 0 < 25
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
