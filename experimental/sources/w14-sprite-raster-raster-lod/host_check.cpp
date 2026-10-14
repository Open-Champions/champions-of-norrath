#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#include "raster.cpp"
using namespace vslots_b;
static int failures, view_calls, type_calls, buffer_calls, draw_calls, drawn_index, drawn_result = 77;
static int type_seen[8], type_answer[16];
static void *draw_scene, *draw_camera, *view_raster;
static unsigned draw_flags;
static float view[16];
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
extern "C" const void* GetModelView__C8VIRaster(const void* raster) { ++view_calls; view_raster = const_cast<void*>(raster); return view; }
extern "C" int GetSpriteType__7VIScenei(void*, int index) { type_seen[type_calls++] = index; return type_answer[index]; }
extern "C" int GetNextColorBuffer__7VIScene(void*) { ++buffer_calls; return 0; }
extern "C" int RasterSprite__7VISceneR8VICameraiUi(void* scene, void* camera, int index, unsigned flags) {
    ++draw_calls; draw_scene = scene; draw_camera = camera; drawn_index = index; draw_flags = flags; return drawn_result;
}
static unsigned char sprite[160], camera[4], scene[4], raster[4];
static void reset() { type_calls = buffer_calls = draw_calls = 0; }
static void setup(unsigned flags, int count, float t0, float t1, float t2) {
    std::memset(sprite, 0, sizeof sprite);
    write_word(address(sprite) + 64u, flags);
    write_word(address(sprite) + 68u, static_cast<unsigned>(count));
    write_single(address(sprite) + 72u, t0);  write_word(address(sprite) + 76u, 10u);
    write_single(address(sprite) + 80u, t1);  write_word(address(sprite) + 84u, 11u);
    write_single(address(sprite) + 88u, t2);  write_word(address(sprite) + 92u, 12u);
    write_single(address(sprite) + 96u, 1000.0f);  // beyond the last level: not read, must not matter
    reset();
}
static int run() { return Raster__11VILODSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 0x31u); }
int main() {
    view[12] = 3.0f; view[13] = 4.0f; view[14] = 12.0f;  // distance 13, squared 169
    // flag bit 0 set: asks for the model view, nothing else
    setup(1u, 3, 5.0f, 20.0f, 30.0f);
    expect(run() == 0 && view_calls == 1 && draw_calls == 0 && type_calls == 0, "bit 0 set: returns 0 without drawing");
    expect(view_raster == raster, "model view of the raster");
    // threshold0 above the distance: level 0 drawn, no type queries
    setup(0xfffffffeu, 3, 14.0f, 20.0f, 30.0f);
    expect(run() == 77, "result of the draw returned");
    expect(draw_calls == 1 && drawn_index == 10 && type_calls == 0 && draw_scene == scene && draw_camera == camera && draw_flags == 0x31u, "level 0 drawn");
    // threshold0 below, threshold1 above: level 1 drawn after passing level 0
    setup(0, 3, 5.0f, 14.0f, 30.0f);
    expect(run() == 77 && draw_calls == 1 && drawn_index == 11, "level 1 drawn");
    expect(type_calls == 1 && type_seen[0] == 10 && buffer_calls == 0, "passed level 0 queried, type not 1");
    // type 1 passes take a color buffer
    type_answer[10] = 1; type_answer[11] = 2; type_answer[12] = 1;
    setup(0, 3, 5.0f, 6.0f, 14.0f);
    run();
    expect(draw_calls == 1 && drawn_index == 12 && type_calls == 2 && buffer_calls == 1, "two passed, one of type 1");
    // equal squares are not "below": threshold 13 stops
    setup(0, 3, 5.0f, 13.0f, 30.0f);
    run();
    expect(drawn_index == 11, "equal threshold stops at that level");
    type_answer[10] = 2;
    setup(0, 3, 13.0f, 20.0f, 30.0f);
    run();
    expect(drawn_index == 10 && type_calls == 0, "equal first threshold: level 0");
    // all thresholds below: past the end, nothing drawn
    type_answer[10] = type_answer[11] = type_answer[12] = 1;
    setup(0, 3, 1.0f, 2.0f, 3.0f);
    expect(run() == 0 && draw_calls == 0 && type_calls == 3 && buffer_calls == 3, "past the last level: returns 0");
    setup(0, 2, 1.0f, 2.0f, 3.0f);
    expect(run() == 0 && draw_calls == 0 && type_calls == 2, "count limits the levels");
    // empty list
    setup(0, 0, 1.0f, 2.0f, 3.0f);
    expect(run() == 0 && draw_calls == 0 && type_calls == 0, "count 0 and far: nothing");
    setup(0, 0, 100.0f, 2.0f, 3.0f);
    expect(run() == 0 && draw_calls == 0, "count 0 and near: nothing");
    // negative count is not caught
    setup(0, -1, 100.0f, 2.0f, 3.0f);
    expect(run() == 77 && draw_calls == 1 && drawn_index == 10, "negative count draws level 0");
    // squared sum uses all three components, signs ignored
    view[12] = -3.0f; view[13] = -4.0f; view[14] = -12.0f;
    setup(0, 3, 12.99f, 20.0f, 30.0f);
    run();
    expect(drawn_index == 11, "negative components: same distance");
    setup(0, 3, 13.01f, 20.0f, 30.0f);
    run();
    expect(drawn_index == 10, "distance 13 is below 13.01");
    view[12] = 0.0f; view[13] = 0.0f; view[14] = 13.0f;
    setup(0, 3, 12.99f, 20.0f, 30.0f);
    run();
    expect(drawn_index == 11, "z alone");
    view[12] = 13.0f; view[14] = 0.0f;
    setup(0, 3, 12.99f, 20.0f, 30.0f);
    run();
    expect(drawn_index == 11, "x alone");
    view[12] = 0.0f; view[13] = 13.0f;
    setup(0, 3, 12.99f, 20.0f, 30.0f);
    run();
    expect(drawn_index == 11, "y alone");
    return failures ? 1 : 0;
}
