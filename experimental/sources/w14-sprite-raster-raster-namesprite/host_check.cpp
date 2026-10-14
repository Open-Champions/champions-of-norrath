#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#define strcpy w14_strcpy
#include "raster.cpp"
#undef strcpy
using namespace vslots_b;
int _12VINameSprite$NumNameDefers;
unsigned char _12VINameSprite$NameDefers[24 * 144];
static int failures, n, order[16], copy_calls, rect_calls, ml_calls, strcpy_calls;
static float view[16], ml_in[4], ml_out[4] = {1.0f, 2.0f, 3.0f, 4.0f};
static const void *ml_matrix, *copy_src;
static void *copy_dst, *rect_raster, *rect_out, *strcpy_dst;
static int rect_word;
static float rect_point[3];
static const char* rect_text;
static const void* strcpy_src;
static int counter_at_strcpy, counter_at_copy, counter_at_rect;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
extern "C" const void* GetModelView__C8VIRaster(const void*) { order[n++] = 1; return view; }
extern "C" void __ml__FRC7VIVect4RC10VIMatrix44(void* result, const void* vector, const void* matrix) {
    order[n++] = 2; ++ml_calls; std::memcpy(ml_in, vector, 16); ml_matrix = matrix; std::memcpy(result, ml_out, 16);
}
extern "C" char* w14_strcpy(char* dst, const char* src) {
    order[n++] = 3; ++strcpy_calls; strcpy_dst = dst; strcpy_src = src; counter_at_strcpy = _12VINameSprite$NumNameDefers;
    return std::strcpy(dst, src);
}
extern "C" void sceVu0CopyMatrix(void* dest, const void* source) {
    order[n++] = 4; ++copy_calls; copy_dst = dest; copy_src = source; counter_at_copy = _12VINameSprite$NumNameDefers;
    std::memcpy(dest, source, 64);
}
extern "C" void CalcCenteredStringScreenRect__8VIRasteriRC7VIVect3PCcR6VIRect(void* raster, int word, const void* point, const char* text, void* rect) {
    order[n++] = 5; ++rect_calls; rect_raster = raster; rect_word = word; std::memcpy(rect_point, point, 12); rect_text = text; rect_out = rect;
    counter_at_rect = _12VINameSprite$NumNameDefers; std::memset(rect, 0xab, 24);
}
static unsigned char sprite[128], camera[4], scene[4], raster[4];
static void reset() {
    n = ml_calls = copy_calls = rect_calls = strcpy_calls = 0;
}
static void prime(float limit) {
    std::memset(sprite, 0, sizeof sprite);
    sprite[68] = 'A'; sprite[69] = 'b'; sprite[70] = 0;
    write_single(address(sprite) + 48u, limit);
    write_word(address(sprite) + 64u, 0xfffffff9u);
    for (int i = 0; i < 16; ++i) sprite[100 + i] = static_cast<unsigned char>(0x30 + i);
    reset();
}
int main() {
    for (int i = 0; i < 16; ++i) view[i] = 10.0f + i;
    // distance^2 = 1 + 4 + 9 + 16 = 30
    prime(6.0f);  // 36 > 30
    _12VINameSprite$NumNameDefers = 2;
    std::memset(_12VINameSprite$NameDefers, 0, sizeof _12VINameSprite$NameDefers);
    int r = Raster__12VINameSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 1u);
    expect(r == 0, "returns 0");
    expect(ml_calls == 1 && ml_matrix == view, "vector times the model view");
    expect(ml_in[0] == 0.0f && ml_in[2] == 0.0f && ml_in[3] == 1.0f, "vector (0, ., 0, 1)");
    unsigned bits; std::memcpy(&bits, &ml_in[1], 4);
    expect(bits == 0x3f199999u, "0.6 pattern 0x3f199999");
    expect(_12VINameSprite$NumNameDefers == 3, "counter incremented");
    const unsigned char* rec = _12VINameSprite$NameDefers + 2 * 144;
    float z; std::memcpy(&z, rec, 4);
    expect(z == 3.0f, "record +0 is the transformed z");
    expect(read_word(address(rec) + 4u) == 0xfffffff9u, "record +4 is sprite word +64");
    expect(std::strcmp(reinterpret_cast<const char*>(rec + 8), "Ab") == 0 && strcpy_src == sprite + 68 && strcpy_dst == rec + 8, "record +8 holds the name");
    expect(std::memcmp(rec + 40, sprite + 100, 16) == 0, "record +40 holds sprite bytes 100..115");
    expect(copy_dst == rec + 80 && copy_src == view && std::memcmp(rec + 80, view, 64) == 0, "record +80 holds the model view");
    expect(rect_out == rec + 56 && rect_raster == raster && rect_word == -7 && rect_text == reinterpret_cast<const char*>(sprite + 68), "rect call arguments");
    unsigned pb[3]; std::memcpy(pb, rect_point, 12);
    expect(pb[0] == 0 && pb[1] == 0x3f199999u && pb[2] == 0, "rect point (0, 0.6, 0)");
    expect(counter_at_strcpy == 2 && counter_at_copy == 2 && counter_at_rect == 2, "counter not yet incremented in the calls");
    expect(order[0] == 1 && order[1] == 2 && order[2] == 3 && order[3] == 1 && order[4] == 4 && order[5] == 5 && n == 6, "order");
    expect(rec[56] == 0xab && rec[79] == 0xab && rec[80] != 0xab && rec[55] == 0x3f, "rect is 24 bytes at +56");
    expect(_12VINameSprite$NameDefers[0] == 0 && _12VINameSprite$NameDefers[144] == 0, "other records untouched");
    // too far: limit^2 <= 30
    prime(5.5f);  // 30.25 > 30 -> queued
    r = Raster__12VINameSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 1u);
    expect(_12VINameSprite$NumNameDefers == 4, "30.25 above 30 queues");
    prime(5.0f);  // 25 <= 30
    r = Raster__12VINameSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 1u);
    expect(r == 0 && _12VINameSprite$NumNameDefers == 4 && ml_calls == 1 && copy_calls == 0 && rect_calls == 0, "too far: nothing queued");
    ml_out[3] = 5.0f;  // w component counts: 1+4+9+25 = 39
    prime(6.0f);       // 36 <= 39
    r = Raster__12VINameSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 1u);
    expect(_12VINameSprite$NumNameDefers == 4 && copy_calls == 0, "w counted in the distance");
    ml_out[3] = 4.0f;
    // empty name
    prime(6.0f); sprite[68] = 0;
    r = Raster__12VINameSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 1u);
    expect(r == 0 && ml_calls == 0 && n == 0 && _12VINameSprite$NumNameDefers == 4, "empty name: nothing");
    prime(6.0f); sprite[68] = 0x80;  // signed byte, nonzero
    Raster__12VINameSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 1u);
    expect(_12VINameSprite$NumNameDefers == 5, "nonzero byte 0x80 is a name");
    // flags bit 0
    prime(6.0f);
    Raster__12VINameSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 0xfffffffeu);
    expect(ml_calls == 0 && _12VINameSprite$NumNameDefers == 5, "bit 0 clear: nothing");
    Raster__12VINameSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 3u);
    expect(ml_calls == 1 && _12VINameSprite$NumNameDefers == 6, "bit 0 set among others");
    // full
    _12VINameSprite$NumNameDefers = 24;
    prime(6.0f);
    r = Raster__12VINameSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 1u);
    expect(r == 0 && ml_calls == 1 && copy_calls == 0 && _12VINameSprite$NumNameDefers == 24, "full queue: nothing added");
    _12VINameSprite$NumNameDefers = 23;
    prime(6.0f);
    Raster__12VINameSpriteR8VICameraR7VISceneR8VIRasterUi(sprite, camera, scene, raster, 1u);
    expect(_12VINameSprite$NumNameDefers == 24 && std::strcmp(reinterpret_cast<const char*>(_12VINameSprite$NameDefers + 23 * 144 + 8), "Ab") == 0, "last record 23");
    return failures ? 1 : 0;
}
