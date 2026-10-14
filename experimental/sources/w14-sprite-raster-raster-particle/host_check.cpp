#define NORRATH_VSLOTS_B_HOST 1
#include <cmath>
#include <cstdio>
#include <cstring>
#include "raster.cpp"
using namespace vslots_b;
static int failures, n, order[64];
static unsigned char sprite[1200], camera_obj[160], scene_obj[4], raster_obj[4];
static float view[16], cam_matrix[16];
static int ui_answer = 3, ui_calls, ui_draw_calls, ui_index;
static unsigned ui_flags;
static void *copy_dst[8], *set_matrix, *destroyed[8], *mul_dest, *space_matrix, *ui_scene, *ui_camera;
static int copy_calls, destroy_calls, destroy_flags[8], set_calls, m33_ctor, m33_dtor, m33_dtor_flag;
static float copied_src[8][16], mul_a_in[16], mul_b_in[16], space_in[16], set_in[16];
static float location_seen[3], dir33[9], dir3[3], mag_in[3], mag_answer = 50.0f;
static int location_calls, dir33_calls, dir3_calls, space_calls, purge_calls, update_calls, mag_calls;
static void *emitter_seen[8], *dir33_ptr, *m33_ptr;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
extern "C" const void* GetModelView__C8VIRaster(const void*) { order[n++] = 1; return view; }
extern "C" void sceVu0CopyMatrix(void* dest, const void* source) {
    order[n++] = 2; copy_dst[copy_calls] = dest; std::memcpy(copied_src[copy_calls], source, 64); ++copy_calls; std::memcpy(dest, source, 64);
}
extern "C" void Transpose__10VIMatrix44RC10VIMatrix44(void* self, const void* source) {
    order[n++] = 3;
    const float* s = static_cast<const float*>(source);
    float* d = static_cast<float*>(self);
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) d[4 * c + r] = s[4 * r + c];
}
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(void* self, const void* a, const void* b) {
    order[n++] = 4; mul_dest = self; std::memcpy(mul_a_in, a, 64); std::memcpy(mul_b_in, b, 64);
    for (int i = 0; i < 16; ++i) static_cast<float*>(self)[i] = static_cast<const float*>(a)[i] + 2.0f * static_cast<const float*>(b)[i];
}
extern "C" void SetLocation__17VIParticleEmitterRC7VIVect3(void* emitter, const void* point) {
    order[n++] = 5; ++location_calls; emitter_seen[0] = emitter; std::memcpy(location_seen, point, 12);
}
extern "C" void SetDirection__17VIParticleEmitterRC10VIMatrix33(void* emitter, const void* matrix) {
    order[n++] = 6; ++dir33_calls; emitter_seen[1] = emitter; dir33_ptr = const_cast<void*>(matrix); std::memcpy(dir33, matrix, 36);
}
extern "C" void SetDirection__17VIParticleEmitterRC7VIVect3(void* emitter, const void* vector) {
    order[n++] = 7; ++dir3_calls; emitter_seen[2] = emitter; std::memcpy(dir3, vector, 12);
}
extern "C" void SetSpace__17VIParticleEmitterRC10VIMatrix44(void* emitter, const void* matrix) {
    order[n++] = 8; ++space_calls; emitter_seen[3] = emitter; space_matrix = const_cast<void*>(matrix); std::memcpy(space_in, matrix, 64);
}
extern "C" void __10VIMatrix33(void* self) { order[n++] = 9; ++m33_ctor; m33_ptr = self; std::memset(self, 0x7f, 36); }
extern "C" void _$_10VIMatrix33(void* self, int flags) { order[n++] = 10; ++m33_dtor; m33_dtor_flag = flags; (void)self; }
extern "C" float Magnitude__C7VIVect3(const void* vector) { order[n++] = 11; ++mag_calls; std::memcpy(mag_in, vector, 12); return mag_answer; }
extern "C" void Purge__17VIParticleEmitter(void* emitter) { order[n++] = 12; ++purge_calls; emitter_seen[4] = emitter; }
extern "C" void Update__17VIParticleEmitter(void* emitter) { order[n++] = 13; ++update_calls; emitter_seen[5] = emitter; }
extern "C" int GetParticleUISprite__7VIScene(void*) { order[n++] = 14; ++ui_calls; return ui_answer < 0 ? -1 : ui_answer + ui_calls; }
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(void*, const void* matrix) { order[n++] = 15; ++set_calls; set_matrix = const_cast<void*>(matrix); std::memcpy(set_in, matrix, 64); }
extern "C" int RasterUISprite__7VISceneR8VICameraiUi(void* scene, void* camera, int index, unsigned flags) {
    order[n++] = 16; ++ui_draw_calls; ui_scene = scene; ui_camera = camera; ui_index = index; ui_flags = flags; return 0;
}
extern "C" void _$_10VIMatrix44(void* self, int flags) { order[n++] = 17; destroyed[destroy_calls] = self; destroy_flags[destroy_calls++] = flags; }
static void reset() {
    n = ui_calls = ui_draw_calls = copy_calls = destroy_calls = set_calls = m33_ctor = m33_dtor = 0;
    location_calls = dir33_calls = dir3_calls = space_calls = purge_calls = update_calls = mag_calls = 0;
    std::memset(sprite, 0, sizeof sprite);
}
static void expected_c(float* c, float* b_out) {
    float b[16];
    for (int r = 0; r < 4; ++r) for (int k = 0; k < 4; ++k) b[4 * k + r] = cam_matrix[4 * r + k];
    float t[3];
    for (int j = 0; j < 3; ++j) t[j] = -(b[3] * b[j] + b[7] * b[4 + j] + b[11] * b[8 + j]);
    for (int j = 0; j < 3; ++j) b[12 + j] = t[j];
    b[3] = b[7] = b[11] = 0.0f;
    for (int i = 0; i < 16; ++i) c[i] = view[i] + 2.0f * b[i];
    if (b_out) std::memcpy(b_out, b, 64);
}
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main() {
    for (int i = 0; i < 16; ++i) view[i] = 1.0f + 0.25f * i * (i % 3 + 1);
    for (int i = 0; i < 16; ++i) cam_matrix[i] = 0.5f * (i + 1) - 0.1f * (i * i % 7);
    std::memcpy(camera_obj + 48, cam_matrix, 64);
    const float campt[3] = {300.0f, -20.0f, 7.0f};
    std::memcpy(camera_obj + 28, campt, 12);
    float c_want[16], b_want[16];
    expected_c(c_want, b_want);
    // ---- mode 0, near, UI sprite present
    reset(); mag_answer = 50.0f; ui_answer = 3;
    int r = Raster__16VIParticleSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera_obj, scene_obj, raster_obj, 0x99u);
    expect(r == 0, "returns 0");
    expect(copy_calls == 3, "three matrix copies: camera matrix, model view, UI model view");
    expect(std::memcmp(copied_src[0], cam_matrix, 64) == 0, "first copy is the camera matrix at +48");
    expect(std::memcmp(copied_src[1], view, 64) == 0 && copy_dst[1] == copy_dst[0], "second copy loads the model view into the same local");
    expect(mul_dest != copy_dst[0] && std::memcmp(mul_a_in, view, 64) == 0, "Mul first operand is the model view local");
    bool same = true;
    for (int i = 0; i < 16; ++i) same = same && near(mul_b_in[i], b_want[i]);
    expect(same, "Mul second operand is the transposed camera matrix with its fourth row replaced");
    expect(location_calls == 1 && near(location_seen[0], c_want[12]) && near(location_seen[1], c_want[13]) && near(location_seen[2], c_want[14]), "location from the product's fourth row");
    expect(dir33_calls == 1 && dir3_calls == 0 && space_calls == 0 && m33_ctor == 1 && m33_dtor == 1 && m33_dtor_flag == 2, "mode 0: matrix33 direction");
    const int idx[9] = {0, 1, 2, 4, 5, 6, 8, 9, 10};
    same = true;
    for (int i = 0; i < 9; ++i) same = same && dir33[i] == c_want[idx[i]];
    expect(same, "3x3 filled from the product's upper-left block");
    expect(emitter_seen[0] == sprite + 80 && emitter_seen[1] == sprite + 80 && emitter_seen[5] == sprite + 80, "emitter at +80");
    expect(near(mag_in[0], 300.0f - c_want[12]) && near(mag_in[1], -20.0f - c_want[13]) && near(mag_in[2], 7.0f - c_want[14]), "camera point minus location");
    expect(update_calls == 1 && purge_calls == 0 && mag_calls == 1, "50 <= 100: Update");
    expect(ui_draw_calls == 1 && ui_index == 5 && ui_scene == scene_obj && ui_camera == camera_obj && ui_flags == 0x99u, "UI draw uses the second answer");
    expect(destroy_calls == 4 && destroyed[0] == copy_dst[2] && destroyed[1] == mul_dest && destroyed[2] != destroyed[1] && destroyed[3] == copy_dst[0], "destroy d, c, b, a");
    for (int i = 0; i < 4; ++i) expect(destroy_flags[i] == 2, "flag 2");
    // order of the whole mode-0 run
    const int want_order[] = {2, 3, 1, 2, 4, 5, 9, 6, 10, 11, 13, 14, 1, 2, 15, 14, 16, 17, 17, 17, 17};
    bool ok = n == 21;
    for (int i = 0; ok && i < 21; ++i) ok = order[i] == want_order[i];
    expect(ok, "call order");
    // ---- distance threshold
    reset(); mag_answer = 100.0f; ui_answer = -1;
    Raster__16VIParticleSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera_obj, scene_obj, raster_obj, 0u);
    expect(update_calls == 1 && purge_calls == 0, "exactly 100: Update");
    expect(ui_draw_calls == 0 && set_calls == 0 && copy_calls == 2 && destroy_calls == 3, "no UI sprite: no d local");
    reset(); mag_answer = 100.5f; ui_answer = -1;
    Raster__16VIParticleSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera_obj, scene_obj, raster_obj, 0u);
    expect(purge_calls == 1 && update_calls == 0 && emitter_seen[4] == sprite + 80, "above 100: Purge");
    // ---- mode non-zero
    reset(); mag_answer = 1.0f; ui_answer = -1;
    write_word(address(sprite) + 1072u, 0xffffffffu);
    Raster__16VIParticleSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera_obj, scene_obj, raster_obj, 0u);
    expect(location_calls == 1 && location_seen[0] == 0.0f && location_seen[1] == 0.0f && location_seen[2] == 0.0f, "mode non-zero: location zero");
    expect(dir3_calls == 1 && dir33_calls == 0 && m33_ctor == 0 && dir3[0] == 0.0f && dir3[1] == 0.0f && dir3[2] == 1.0f, "direction (0,0,1)");
    expect(space_calls == 1, "SetSpace once");
    same = true;
    for (int i = 0; i < 16; ++i) same = same && near(space_in[i], c_want[i]);
    expect(same && space_matrix == mul_dest, "SetSpace gets the product");
    expect(near(mag_in[0], 300.0f - c_want[12]), "distance still uses the product location, not zero");
    // ---- UI clamp rows
    reset(); mag_answer = 1.0f; ui_answer = 2;
    const float rows[16] = {6, 8, 0, 11, 0, 0, 4, 12, 2, 0, 0, 13, 100, 200, 300, 14};
    float saved[16]; std::memcpy(saved, view, 64);
    std::memcpy(view, rows, 64);
    Raster__16VIParticleSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera_obj, scene_obj, raster_obj, 0u);
    expect(near(set_in[0], 2.4f) && near(set_in[1], 3.2f) && set_in[2] == 0.0f && set_in[3] == 11.0f, "row 0 limited to length 4");
    expect(set_in[6] == 4.0f && set_in[8] == 2.0f && std::memcmp(set_in + 12, rows + 12, 16) == 0, "other rows untouched");
    expect(set_matrix == destroyed[0], "d installed then destroyed first");
    std::memcpy(view, saved, 64);
    return failures ? 1 : 0;
}
