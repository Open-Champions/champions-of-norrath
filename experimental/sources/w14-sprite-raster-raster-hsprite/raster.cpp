#include "../vslots-b-bytes/bytes.hpp"
// Observed Raster__9VIHSpriteR8VICameraR7VISceneR8VIRasterUi at 0x1060548,
// 632 bytes. Arguments: the sprite, camera, scene, raster and flags. Two
// 64-byte 16-aligned VIMatrix44 locals are zero-filled, "first" and "second".
//  1. CalcLODLevelIndex(sprite, raster's current model view) gives a level;
//     Process(sprite, level, scene) is called with it. A negative answer ends
//     the function with -1 (both locals destroyed, second then first, with
//     their destructor flag 2).
//  2. CalcTransAboutCenter(sprite, a fresh GetModelView, scene, first) fills the
//     first local; SetBlendMatrices(sprite, raster) is called, and a negative
//     answer again returns -1.
//  3. The pool embedded at sprite+96 is walked with Begin, then at each step
//     the entry at (word at pool+8) + 16*position is used: its word +4 is an
//     attachment number. If it is -1 the model view is set to the first local;
//     otherwise the second local is made by Mul(second, M, first) where M is
//     the matrix at offset 48 of the 256-byte record number "attachment" in
//     the table whose base is the word at (sprite+64)+8, and the model view is
//     set to the second local. Either way RasterSprite(scene, camera, the
//     entry's word +0, flags) is called, and the result of the whole function
//     becomes 1 once any RasterSprite answers non-zero (it is never reset to
//     0). The position advances with Next(pool, position) and the walk ends
//     when it equals End(pool); End is asked afresh each step.
//  4. Both locals are destroyed and the 0/1 flag is returned.
// Unknown: what the pool entries, the attachment table and the "H" in the name
// mean, and what the Process and SetBlendMatrices answers describe.
extern "C" int CalcLODLevelIndex__9VIHSpriteRC10VIMatrix44(void* self, const void* matrix);
extern "C" int Process__9VIHSpriteiR7VIScene(void* self, int level, void* scene);
extern "C" const void* GetModelView__C8VIRaster(const void* raster);
extern "C" void CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(
    void* self, const void* matrix, void* scene, void* out);
extern "C" int SetBlendMatrices__9VIHSpriteR8VIRaster(void* self, void* raster);
extern "C" int Begin__Ct6VIPool1Z19VIHSpriteAttachment(void* pool);
extern "C" int Next__Ct6VIPool1Z19VIHSpriteAttachmenti(void* pool, int position);
extern "C" int End__Ct6VIPool1Z19VIHSpriteAttachment(void* pool);
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(void* self, const void* a, const void* b);
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(void* raster, const void* matrix);
extern "C" int RasterSprite__7VISceneR8VICameraiUi(void* scene, void* camera, int index, unsigned flags);
extern "C" void _$_10VIMatrix44(void* self, int flags);

extern "C" int Raster__9VIHSpriteR8VICameraR7VISceneR8VIRasterUi(
    void* self, void* camera, void* scene, void* raster, unsigned flags) {
    using namespace vslots_b;
    alignas(16) float first[16];
    alignas(16) float second[16];
    for (int i = 0; i < 16; ++i) first[i] = 0.0f;
    for (int i = 0; i < 16; ++i) second[i] = 0.0f;
    int result = 0;
    const int level = CalcLODLevelIndex__9VIHSpriteRC10VIMatrix44(self, GetModelView__C8VIRaster(raster));
    if (Process__9VIHSpriteiR7VIScene(self, level, scene) < 0) {
        _$_10VIMatrix44(second, 2);
        _$_10VIMatrix44(first, 2);
        return -1;
    }
    CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(self, GetModelView__C8VIRaster(raster), scene, first);
    if (SetBlendMatrices__9VIHSpriteR8VIRaster(self, raster) < 0) {
        _$_10VIMatrix44(second, 2);
        _$_10VIMatrix44(first, 2);
        return -1;
    }
    const Address attachments = address(self) + 64u;
    void* const pool = static_cast<unsigned char*>(self) + 96;
    int position = Begin__Ct6VIPool1Z19VIHSpriteAttachment(pool);
    while (position != End__Ct6VIPool1Z19VIHSpriteAttachment(pool)) {
        const Address entry = address(pointer(read_word(address(pool) + 8u))) + (static_cast<unsigned>(position) << 4);
        const int attachment = signed_word(read_word(entry + 4u));
        if (attachment == -1) {
            SetModelView__8VIRasterRC10VIMatrix44(raster, first);
        } else {
            const Address record = address(pointer(read_word(attachments + 8u))) + (static_cast<unsigned>(attachment) << 8);
            Mul__10VIMatrix44RC10VIMatrix44T1(second, pointer(record + 48u), first);
            SetModelView__8VIRasterRC10VIMatrix44(raster, second);
        }
        const Address again = address(pointer(read_word(address(pool) + 8u))) + (static_cast<unsigned>(position) << 4);
        if (RasterSprite__7VISceneR8VICameraiUi(scene, camera, signed_word(read_word(again)), flags) != 0) result = 1;
        position = Next__Ct6VIPool1Z19VIHSpriteAttachmenti(pool, position);
    }
    _$_10VIMatrix44(second, 2);
    _$_10VIMatrix44(first, 2);
    return result;
}
