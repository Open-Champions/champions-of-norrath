#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#include "raster.cpp"
using namespace vslots_b;
static int failures, n, order[64];
static float view[16];
static int copy_calls, tr_calls, mul_calls, set_calls, draw_calls, destroy_calls, draw_index[8], draw_answers[8], destroy_flag[4];
static void *copy_dest, *tr_dest[8], *mul_dest[8], *mul_a[8], *mul_b[8], *set_arg[8], *destroyed[4], *draw_scene, *draw_camera;
static const void *tr_first[8], *tr_second[8];
static float tr_scale[8];
static unsigned draw_flags;
static unsigned char* group_obj;
static int shrink_after = -1;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
extern "C" const void* GetModelView__C8VIRaster(const void*) { order[n++] = 1; return view; }
extern "C" void sceVu0CopyMatrix(void* dest, const void* source) {
    order[n++] = 2; copy_dest = dest; ++copy_calls; std::memcpy(dest, source, 64);
}
extern "C" void Transform__10VIMatrix44RC7VIVect3fT1(void* self, const void* first, const void* second, float scale) {
    order[n++] = 3; tr_dest[tr_calls] = self; tr_first[tr_calls] = first; tr_second[tr_calls] = second; tr_scale[tr_calls++] = scale;
    static_cast<float*>(self)[0] = 2.0f;
}
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(void* self, const void* a, const void* b) {
    order[n++] = 4; mul_dest[mul_calls] = self; mul_a[mul_calls] = const_cast<void*>(a); mul_b[mul_calls++] = const_cast<void*>(b);
}
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(void*, const void* matrix) { order[n++] = 5; set_arg[set_calls++] = const_cast<void*>(matrix); }
extern "C" int RasterSprite__7VISceneR8VICameraiUi(void* scene, void* camera, int index, unsigned flags) {
    order[n++] = 6; draw_scene = scene; draw_camera = camera; draw_flags = flags; draw_index[draw_calls] = index;
    if (draw_calls == shrink_after) write_word(address(group_obj) + 64u, 1u);
    return draw_answers[draw_calls++];
}
extern "C" void _$_10VIMatrix44(void* self, int flags) { order[n++] = 7; destroyed[destroy_calls] = self; destroy_flag[destroy_calls++] = flags; }
static void reset() {
    n = copy_calls = tr_calls = mul_calls = set_calls = draw_calls = destroy_calls = 0;
    std::memset(draw_answers, 0, sizeof draw_answers);
}
int main() {
    unsigned char group[68 + 32 * 3 + 16], camera[4], scene[4], raster[4];
    group_obj = group;
    for (int i = 0; i < 16; ++i) view[i] = 50.0f + i;
    std::memset(group, 0, sizeof group);
    for (int i = 0; i < 3; ++i) {
        write_word(address(group) + 68u + 32u * i, 20u + i);
        write_single(address(group) + 84u + 32u * i, 0.5f + i);
    }
    write_word(address(group) + 64u, 3u);
    int r = Raster__13VIGroupSpriteR8VICameraR7VISceneR8VIRasterUi(group, camera, scene, raster, 0x44u);
    expect(r == 0 && draw_calls == 3, "three members, no answer: 0");
    expect(order[0] == 1 && order[1] == 2 && order[2] == 3 && order[3] == 4 && order[4] == 5 && order[5] == 6 && order[6] == 3, "setup then transform, mul, set, draw per member");
    expect(draw_index[0] == 20 && draw_index[1] == 21 && draw_index[2] == 22, "member words +0");
    expect(tr_first[1] == group + 68 + 32 + 4 && tr_second[1] == group + 68 + 32 + 20, "vector arguments at +4 and +20");
    expect(tr_scale[0] == 0.5f && tr_scale[1] == 1.5f && tr_scale[2] == 2.5f, "scale at +16");
    expect(mul_dest[0] == set_arg[0] && mul_a[0] == tr_dest[0] && mul_b[0] == copy_dest, "product = member * saved, then installed");
    expect(copy_dest != tr_dest[0] && copy_dest != mul_dest[0] && tr_dest[0] != mul_dest[0], "three distinct locals");
    expect(draw_scene == scene && draw_camera == camera && draw_flags == 0x44u, "draw arguments");
    expect(destroy_calls == 3 && destroyed[0] == mul_dest[0] && destroyed[1] == tr_dest[0] && destroyed[2] == copy_dest, "destroy product, member, saved");
    expect(destroy_flag[0] == 2 && destroy_flag[1] == 2 && destroy_flag[2] == 2, "flag 2");
    reset();
    draw_answers[1] = -3;
    r = Raster__13VIGroupSpriteR8VICameraR7VISceneR8VIRasterUi(group, camera, scene, raster, 0x44u);
    expect(r == 1, "any non-zero answer gives 1, later zero keeps it");
    reset();
    write_word(address(group) + 64u, 0u);
    r = Raster__13VIGroupSpriteR8VICameraR7VISceneR8VIRasterUi(group, camera, scene, raster, 0x44u);
    expect(r == 0 && draw_calls == 0 && copy_calls == 1 && destroy_calls == 3, "zero members");
    reset();
    write_word(address(group) + 64u, 0xffffffffu);
    r = Raster__13VIGroupSpriteR8VICameraR7VISceneR8VIRasterUi(group, camera, scene, raster, 0x44u);
    expect(r == 0 && draw_calls == 0, "negative count");
    reset();
    write_word(address(group) + 64u, 3u);
    shrink_after = 0;
    r = Raster__13VIGroupSpriteR8VICameraR7VISceneR8VIRasterUi(group, camera, scene, raster, 0x44u);
    expect(draw_calls == 1 && r == 0, "count re-read after each member");
    return failures ? 1 : 0;
}
