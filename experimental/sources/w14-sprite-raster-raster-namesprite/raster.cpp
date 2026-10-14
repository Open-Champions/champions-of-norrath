#include "../vslots-b-bytes/bytes.hpp"
// Observed Raster__12VINameSpriteR8VICameraR7VISceneR8VIRasterUi at 0x10a0938,
// 548 bytes. Arguments: the sprite, camera, scene, raster and flags. The
// camera and scene are unused. It always returns 0. The name sprite queues a
// deferred draw record in a global table instead of drawing at once.
// Nothing happens when the signed byte at sprite+68 (the first character of the
// name) is zero, when bit 0 of flags is clear, when the queue already holds 24
// records, or when the sprite is too far away. Distance: the vector
// (0, 0.6, 0, 1), with 0.6 as the single-precision pattern 0x3f199999, is
// multiplied by the raster's current model view (operator* on VIVect4 and
// VIMatrix44, which returns by hidden pointer). The sum of the squares of its
// four components, taken left to right, must be below the square of the float
// at sprite+48.
// A record is 144 bytes at NameDefers + 144 * NumNameDefers:
//   +0   the float z (third component) of the transformed vector
//   +4   the word at sprite+64
//   +8   the string at sprite+68, copied with strcpy (at most 32 bytes fit
//        before the next field; the image does not check the length)
//   +40  the 16 bytes at sprite+100
//   +56  a VIRect, filled by CalcCenteredStringScreenRect(raster, the word at
//        sprite+64, the vector (0, 0.6, 0), the sprite's string, rect)
//   +80  a copy of the raster's model view (sceVu0CopyMatrix)
// The counter is then incremented. The table is 3456 bytes, i.e. 24 records.
// The counter is re-read from memory after each call. Unknown: what the
// sprite words at +48 and +64 mean by name, the VIRect layout, and the user of
// the table.
extern "C" const void* GetModelView__C8VIRaster(const void* raster);
extern "C" void __ml__FRC7VIVect4RC10VIMatrix44(void* result, const void* vector, const void* matrix);
extern "C" char* strcpy(char* destination, const char* source);
extern "C" void sceVu0CopyMatrix(void* dest, const void* source);
extern "C" void CalcCenteredStringScreenRect__8VIRasteriRC7VIVect3PCcR6VIRect(
    void* raster, int word, const void* point, const char* text, void* rect);
extern "C" int _12VINameSprite$NumNameDefers;
extern "C" unsigned char _12VINameSprite$NameDefers[24 * 144];

extern "C" int Raster__12VINameSpriteR8VICameraR7VISceneR8VIRasterUi(
    void* self, void* camera, void* scene, void* raster, unsigned flags) {
    using namespace vslots_b;
    (void)camera;
    (void)scene;
    const Address base = address(self);
    float vector[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    float point[3] = {0.0f, 0.0f, 0.0f};
    float result[4];
    if (static_cast<signed char>(read_u8(base + 68u)) == 0) return 0;
    if ((flags & 1u) == 0u) return 0;
    const float up = __builtin_bit_cast(float, 0x3f199999u);
    vector[3] = 1.0f;
    vector[0] = 0.0f;
    vector[1] = up;
    vector[2] = 0.0f;
    __ml__FRC7VIVect4RC10VIMatrix44(result, vector, GetModelView__C8VIRaster(raster));
    vector[0] = result[0];
    vector[1] = result[1];
    vector[2] = result[2];
    vector[3] = result[3];
    const float limit = read_single(base + 48u);
    const float distance_squared = result[0] * result[0] + result[1] * result[1] + result[2] * result[2] + result[3] * result[3];
    if (limit * limit <= distance_squared) return 0;
    point[0] = 0.0f;
    point[1] = up;
    point[2] = 0.0f;
    if (_12VINameSprite$NumNameDefers >= 24) return 0;
    unsigned char* const table = _12VINameSprite$NameDefers;
    Address record = address(table) + 144u * static_cast<unsigned>(_12VINameSprite$NumNameDefers);
    write_single(record, vector[2]);
    for (unsigned i = 0; i < 16u; ++i) write_u8(record + 40u + i, read_u8(base + 100u + i));
    write_word(record + 4u, read_word(base + 64u));
    strcpy(reinterpret_cast<char*>(record + 8u), reinterpret_cast<const char*>(base + 68u));
    record = address(table) + 144u * static_cast<unsigned>(_12VINameSprite$NumNameDefers);
    sceVu0CopyMatrix(bytes(record + 80u), GetModelView__C8VIRaster(raster));
    record = address(table) + 144u * static_cast<unsigned>(_12VINameSprite$NumNameDefers);
    CalcCenteredStringScreenRect__8VIRasteriRC7VIVect3PCcR6VIRect(
        raster, signed_word(read_word(base + 64u)), point, reinterpret_cast<const char*>(base + 68u), bytes(record + 56u));
    _12VINameSprite$NumNameDefers = _12VINameSprite$NumNameDefers + 1;
    return 0;
}
