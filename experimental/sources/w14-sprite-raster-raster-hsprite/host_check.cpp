#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#include "raster.cpp"
using namespace vslots_b;
static int failures, n, order[64];
static int lod_answer = 3, process_answer, blend_answer, seen_process_level;
static const void* lod_matrix;
static void *lod_self, *process_self, *process_scene, *trans_self, *trans_scene, *trans_out, *blend_self, *blend_raster;
static const void* trans_matrix;
static float view[16];
static int begin_calls, end_calls, next_calls, next_args[8], positions[8], next_index;
static void *pool_seen[3];
static int destroy_calls, destroy_flag[4];
static void* destroyed[4];
static int mul_calls, set_calls, draw_calls, draw_index[8], draw_answers[8];
static void *mul_dest[4], *set_arg[4], *draw_scene[4], *draw_camera[4];
static const void *mul_a[4], *mul_b[4];
static unsigned draw_flags[4];
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
extern "C" int CalcLODLevelIndex__9VIHSpriteRC10VIMatrix44(void* self, const void* matrix) {
    order[n++] = 1; lod_self = self; lod_matrix = matrix; return lod_answer;
}
extern "C" int Process__9VIHSpriteiR7VIScene(void* self, int level, void* scene) {
    order[n++] = 2; process_self = self; seen_process_level = level; process_scene = scene; return process_answer;
}
extern "C" const void* GetModelView__C8VIRaster(const void*) { order[n++] = 3; return view; }
extern "C" void CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(void* self, const void* matrix, void* scene, void* out) {
    order[n++] = 4; trans_self = self; trans_matrix = matrix; trans_scene = scene; trans_out = out;
    static_cast<float*>(out)[0] = 5.0f;
}
extern "C" int SetBlendMatrices__9VIHSpriteR8VIRaster(void* self, void* raster) {
    order[n++] = 5; blend_self = self; blend_raster = raster; return blend_answer;
}
extern "C" int Begin__Ct6VIPool1Z19VIHSpriteAttachment(void* pool) { order[n++] = 6; pool_seen[0] = pool; ++begin_calls; next_index = 0; return positions[0]; }
extern "C" int Next__Ct6VIPool1Z19VIHSpriteAttachmenti(void* pool, int position) {
    order[n++] = 7; pool_seen[1] = pool; next_args[next_calls++] = position; return positions[++next_index];
}
extern "C" int End__Ct6VIPool1Z19VIHSpriteAttachment(void* pool) { order[n++] = 8; pool_seen[2] = pool; ++end_calls; return 99; }
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(void* self, const void* a, const void* b) {
    order[n++] = 9; mul_dest[mul_calls] = self; mul_a[mul_calls] = a; mul_b[mul_calls] = b; ++mul_calls;
    static_cast<float*>(self)[1] = 7.0f;
}
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(void*, const void* matrix) { order[n++] = 10; set_arg[set_calls++] = const_cast<void*>(matrix); }
extern "C" int RasterSprite__7VISceneR8VICameraiUi(void* scene, void* camera, int index, unsigned flags) {
    order[n++] = 11; draw_scene[draw_calls] = scene; draw_camera[draw_calls] = camera; draw_flags[draw_calls] = flags;
    draw_index[draw_calls] = index; return draw_answers[draw_calls++];
}
extern "C" void _$_10VIMatrix44(void* self, int flags) { order[n++] = 12; destroyed[destroy_calls] = self; destroy_flag[destroy_calls++] = flags; }
static unsigned char sprite[160], camera[4], scene[4], raster[4];
static unsigned char entries[64], table[1024];
static void reset() {
    n = begin_calls = end_calls = next_calls = mul_calls = set_calls = draw_calls = destroy_calls = 0;
    std::memset(draw_answers, 0, sizeof draw_answers);
}
int main() {
    host_note(entries); host_note(table); host_note(view);
    for (int i = 0; i < 16; ++i) view[i] = 100.0f + i;
    std::memset(sprite, 0, sizeof sprite);
    write_word(address(sprite) + 64u + 8u, address_bits(table));
    write_word(address(sprite) + 96u + 8u, address_bits(entries));
    // entries: position 2 -> sprite 40, no attachment; position 1 -> sprite 41, attachment 3
    write_word(address(entries) + 32u, 40u); write_word(address(entries) + 36u, 0xffffffffu);
    write_word(address(entries) + 16u, 41u); write_word(address(entries) + 20u, 3u);
    positions[0] = 2; positions[1] = 1; positions[2] = 99;
    // Process negative
    process_answer = -4;
    reset();
    int r = Raster__9VIHSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 0x20u);
    expect(r == -1, "Process negative returns -1");
    expect(n == 5 && order[0] == 3 && order[1] == 1 && order[2] == 2 && order[3] == 12 && order[4] == 12, "model view, level, process, destroy x2");
    expect(seen_process_level == 3 && lod_self == sprite && process_self == sprite && process_scene == scene && lod_matrix == view, "level arguments");
    expect(destroy_flag[0] == 2 && destroy_flag[1] == 2 && destroyed[0] != destroyed[1], "two distinct locals destroyed, flag 2");
    // Blend negative
    process_answer = 0; blend_answer = -1;
    reset();
    r = Raster__9VIHSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 0x20u);
    expect(r == -1 && draw_calls == 0 && begin_calls == 0 && destroy_calls == 2, "SetBlendMatrices negative returns -1");
    expect(trans_self == sprite && trans_scene == scene && trans_matrix == view && blend_self == sprite && blend_raster == raster, "trans/blend arguments");
    expect(trans_out == destroyed[1], "first local is the trans output");
    // Walk
    blend_answer = 0;
    reset();
    draw_answers[0] = 0; draw_answers[1] = 0;
    r = Raster__9VIHSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 0x20u);
    expect(r == 0 && draw_calls == 2, "two entries drawn, nothing answered, result 0");
    expect(draw_index[0] == 40 && draw_index[1] == 41, "entry words +0 in walk order");
    expect(draw_scene[0] == scene && draw_camera[0] == camera && draw_flags[0] == 0x20u && draw_flags[1] == 0x20u, "draw arguments");
    expect(set_calls == 2 && set_arg[0] == trans_out, "entry without attachment sets the first local");
    expect(mul_calls == 1 && mul_dest[0] == set_arg[1] && mul_b[0] == trans_out && mul_dest[0] != trans_out, "attachment entry multiplies into the second local");
    expect(mul_a[0] == table + 3 * 256 + 48, "attachment record 3 offset 48");
    expect(next_calls == 2 && next_args[0] == 2 && next_args[1] == 1, "Next with the current position");
    expect(end_calls == 3 && begin_calls == 1, "End asked every step");
    expect(pool_seen[0] == sprite + 96 && pool_seen[1] == sprite + 96 && pool_seen[2] == sprite + 96, "pool at sprite+96");
    expect(destroy_calls == 2 && destroy_flag[0] == 2 && destroyed[0] == mul_dest[0] && destroyed[1] == trans_out, "second destroyed before first");
    // One answers non-zero -> 1, and it stays 1
    reset();
    draw_answers[0] = 5; draw_answers[1] = 0;
    r = Raster__9VIHSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 0x20u);
    expect(r == 1, "first non-zero answer gives 1, later zero does not reset");
    reset();
    draw_answers[0] = 0; draw_answers[1] = -9;
    r = Raster__9VIHSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 0x20u);
    expect(r == 1, "negative answer is non-zero");
    // Empty walk
    positions[0] = 99;
    reset();
    r = Raster__9VIHSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 0x20u);
    expect(r == 0 && draw_calls == 0 && destroy_calls == 2, "empty pool");
    return failures ? 1 : 0;
}
