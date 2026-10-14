#define NORRATH_VSLOTS_B_HOST 1
#include <cmath>
#include <cstdio>
#include <cstring>
#include "raster.cpp"
using namespace vslots_b;
static int failures, n, order[64];
static unsigned char sprite[128], camera_obj[160], scene_obj[4], raster_obj[4];
static float view[16];
static float size_first[4], size_second[4];
static int size_calls, resize_calls, ui_answer = 7, ui_calls, process_calls, render_calls, destroy_calls, ui_draw_calls;
static const void *size_box[2];
static void *resize_volume, *resize_box, *process_volume, *render_volume, *render_camera, *render_matrix;
static float render_in[16], process_step;
static float transposed_from[2][16];
static int transposes;
static float set_in[16];
static void *m_copy_dst[4], *set_matrix, *destroyed[4];
static int copy_calls, destroy_flags[4], ui_index, set_calls;
static unsigned ui_flags;
static void *ui_scene, *ui_camera;
static float mul_a_in[16], mul_b_in[16];
static void* mul_dest;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
extern "C" void Size__C6VIBBox(void* result, const void* box) {
    order[n++] = 1; size_box[size_calls] = box;
    std::memcpy(result, size_calls == 0 ? size_first : size_second, 12);
    ++size_calls;
}
extern "C" void OnResize__14VIEffectVolumeRC6VIBBox(void* volume, const void* box) { order[n++] = 2; ++resize_calls; resize_volume = volume; resize_box = const_cast<void*>(box); }
extern "C" int GetStreamAudioUISprite__7VIScene(void*) { order[n++] = 3; ++ui_calls; return ui_answer < 0 ? -1 : ui_answer + ui_calls; }
extern "C" const void* GetModelView__C8VIRaster(const void*) { order[n++] = 4; return view; }
extern "C" void sceVu0CopyMatrix(void* dest, const void* source) { order[n++] = 5; m_copy_dst[copy_calls++] = dest; std::memcpy(dest, source, 64); }
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(void*, const void* matrix) { order[n++] = 6; ++set_calls; set_matrix = const_cast<void*>(matrix); std::memcpy(set_in, matrix, 64); }
extern "C" int RasterUISprite__7VISceneR8VICameraiUi(void* scene, void* camera, int index, unsigned flags) {
    order[n++] = 7; ++ui_draw_calls; ui_scene = scene; ui_camera = camera; ui_index = index; ui_flags = flags; return 0;
}
extern "C" void Transpose__10VIMatrix44RC10VIMatrix44(void* self, const void* source) {
    order[n++] = 8; std::memcpy(transposed_from[transposes], source, 64);
    const float* s = static_cast<const float*>(source);
    float* d = static_cast<float*>(self);
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) d[4 * c + r] = s[4 * r + c];
    ++transposes;
}
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(void* self, const void* a, const void* b) {
    order[n++] = 9; mul_dest = self; std::memcpy(mul_a_in, a, 64); std::memcpy(mul_b_in, b, 64);
    for (int i = 0; i < 16; ++i) static_cast<float*>(self)[i] = static_cast<const float*>(a)[i] + 2.0f * static_cast<const float*>(b)[i];
}
extern "C" void Process__14VIEffectVolumef(void* volume, float step) { order[n++] = 10; ++process_calls; process_volume = volume; process_step = step; }
extern "C" void Render__14VIEffectVolumeR8VICameraR10VIMatrix44(void* volume, void* camera, void* matrix) {
    order[n++] = 11; ++render_calls; render_volume = volume; render_camera = camera; render_matrix = matrix; std::memcpy(render_in, matrix, 64);
}
extern "C" void _$_10VIMatrix44(void* self, int flags) { order[n++] = 12; destroyed[destroy_calls] = self; destroy_flags[destroy_calls++] = flags; }
static void reset() {
    n = size_calls = resize_calls = ui_calls = process_calls = render_calls = destroy_calls = ui_draw_calls = transposes = copy_calls = set_calls = 0;
    std::memset(sprite, 0, sizeof sprite);
    for (int i = 0; i < 6; ++i) write_single(address(sprite) + 8u + 4u * i, 1.0f + i);  // box +8..+31
    for (int i = 0; i < 6; ++i) write_single(address(sprite) + 64u + 4u * i, 90.0f + i);
    for (int i = 0; i < 4; ++i) { size_first[i] = 10.0f; size_second[i] = 10.0f; }
}
static void expected_m2(const float* camera_matrix, const float* mv, float* out) {
    // independent evaluation: build m2 = T(camera), fix, m3 = mv + 2*m2, m2 = T(m3), fix
    float m2[16], m3[16];
    auto fix = [](float* m) {
        float t[3];
        for (int j = 0; j < 3; ++j) t[j] = -(m[3] * m[j] + m[7] * m[4 + j] + m[11] * m[8 + j]);
        for (int j = 0; j < 3; ++j) m[12 + j] = t[j];
        m[3] = m[7] = m[11] = 0.0f;
    };
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) m2[4 * c + r] = camera_matrix[4 * r + c];
    fix(m2);
    for (int i = 0; i < 16; ++i) m3[i] = mv[i] + 2.0f * m2[i];
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) out[4 * c + r] = m3[4 * r + c];
    fix(out);
}
int main() {
    for (int i = 0; i < 16; ++i) view[i] = 1.0f + 0.25f * i * (i % 3 + 1);
    float cam[16];
    for (int i = 0; i < 16; ++i) cam[i] = 0.5f * (i + 1) - 0.1f * (i * i % 7);
    std::memcpy(camera_obj + 48, cam, 64);
    // --- no resize, no UI sprite, no flag 2
    reset(); ui_answer = -1;
    int r = Raster__20VIEffectVolumeSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera_obj, scene_obj, raster_obj, 1u);
    expect(r == 1, "returns 1");
    expect(size_calls == 2 && size_box[0] == sprite + 8 && size_box[1] == sprite + 64, "sizes of the boxes at +8 and +64");
    expect(resize_calls == 0 && ui_draw_calls == 0 && render_calls == 0 && set_calls == 0, "nothing else");
    expect(destroy_calls == 1 && destroy_flags[0] == 2, "m0 destroyed");
    const int w1[3] = {1, 1, 3};
    bool ok = n == 4 + 0;
    expect(order[0] == 1 && order[1] == 1 && order[2] == 3 && order[3] == 12 && n == 4, "order sizes, ui query, destroy");
    (void)w1; (void)ok;
    // --- resize thresholds
    for (int comp = 0; comp < 3; ++comp) {
        for (int sgn = -1; sgn <= 1; sgn += 2) {
            reset(); ui_answer = -1;
            size_first[comp] = 10.0f + sgn * 1.0f;  // exactly 1: no resize
            Raster__20VIEffectVolumeSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera_obj, scene_obj, raster_obj, 0u);
            expect(resize_calls == 0, "difference of exactly 1 does nothing");
            reset(); ui_answer = -1;
            size_first[comp] = 10.0f + sgn * 1.5f;
            Raster__20VIEffectVolumeSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera_obj, scene_obj, raster_obj, 0u);
            expect(resize_calls == 1 && resize_volume == sprite + 88 && resize_box == sprite + 64, "resize with volume +88, box +64");
            ok = true;
            for (int i = 0; i < 6; ++i) ok = ok && read_single(address(sprite) + 64u + 4u * i) == 1.0f + i;
            expect(ok, "six floats copied from +8 to +64");
            expect(order[0] == 1 && order[1] == 1 && order[2] == 2, "resize right after the two sizes");
        }
    }
    // --- UI sprite path
    reset(); ui_answer = 4;
    r = Raster__20VIEffectVolumeSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera_obj, scene_obj, raster_obj, 0xfffffffdu);
    expect(ui_calls == 2 && ui_draw_calls == 1 && ui_index == 6, "UI index is the second query's answer");
    expect(copy_calls == 1 && set_calls == 1 && set_matrix == m_copy_dst[0] && std::memcmp(set_in, view, 64) == 0, "model view copied and installed");
    expect(ui_scene == scene_obj && ui_camera == camera_obj && ui_flags == 0xfffffffdu, "UI arguments");
    expect(render_calls == 0 && process_calls == 0 && destroy_calls == 1, "bit 1 clear: no render");
    const int w2[9] = {1, 1, 3, 4, 5, 6, 3, 7, 12};
    ok = n == 9;
    for (int i = 0; ok && i < 9; ++i) ok = order[i] == w2[i];
    expect(ok, "order of the UI path");
    // --- flag 2
    reset(); ui_answer = 4;
    r = Raster__20VIEffectVolumeSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera_obj, scene_obj, raster_obj, 2u);
    expect(r == 1 && render_calls == 1 && process_calls == 1, "render and process once");
    float want[16];
    expected_m2(cam, view, want);
    bool same = true;
    for (int i = 0; i < 16; ++i) same = same && std::fabs(render_in[i] - want[i]) < 1e-4f;
    expect(same, "matrix handed to Render matches the independent evaluation");
    expect(process_volume == sprite + 88 && render_volume == sprite + 88 && render_camera == camera_obj, "volume +88, camera");
    unsigned bits; std::memcpy(&bits, &process_step, 4);
    expect(bits == 0x3d088888u, "step pattern 0x3d088888");
    expect(transposes == 2 && std::memcmp(transposed_from[0], cam, 64) == 0, "first transpose reads the camera matrix at +48");
    expect(mul_a_in[0] == view[0] && std::memcmp(mul_a_in + 1, view + 1, 60) == 0, "Mul first operand is the model view");
    expect(render_matrix != mul_dest, "Render gets the transposed local, not the product");
    expect(destroy_calls == 4 && destroy_flags[0] == 2 && destroy_flags[3] == 2, "four locals destroyed");
    expect(destroyed[0] == mul_dest && destroyed[1] == render_matrix && destroyed[3] == m_copy_dst[0], "destroy m3, m2, m1, m0");
    expect(copy_calls == 3 && m_copy_dst[1] != m_copy_dst[0] && m_copy_dst[2] == m_copy_dst[1], "m1 loaded twice (camera matrix, then model view)");
    expect(order[n - 1] == 12 && order[n - 5] == 11 && order[n - 6] == 10, "Process then Render before destruction");
    // --- flag 2 without a UI sprite
    reset(); ui_answer = -1;
    r = Raster__20VIEffectVolumeSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera_obj, scene_obj, raster_obj, 3u);
    expect(render_calls == 1 && ui_draw_calls == 0 && copy_calls == 2 && destroy_calls == 4, "flag 2 works without the UI draw");
    return failures ? 1 : 0;
}
