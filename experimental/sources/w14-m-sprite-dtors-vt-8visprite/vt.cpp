// Observed _vt$8VISprite at 0x011f4d10, 96 bytes (12 slots of 8 bytes) in the image data.
// Each slot is a zero adjustment word and a function word. The function words, in slot order, are the
// image symbols named below; the last slot is null. The bodies of these functions are not in this unit.
// Inferred: the first function word is the type-info function of the class, then the virtual functions in
// the order the image lists them. Unknown: the owner of each named function beyond the names in this unit.
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VtSlot) == 8, "slot is an adjustment word plus a pointer");
#endif
extern "C" {
void vt_fn_0() asm("__tf8VISprite");
void vt_fn_1() asm("_$_8VISprite");
void vt_fn_2() asm("GetInstType__C8VISpriteR7VIScene");
void vt_fn_3() asm("Raster__8VISpriteR8VICameraR7VISceneR8VIRasterUi");
void vt_fn_4() asm("Collide__8VISpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide");
void vt_fn_5() asm("Collide__8VISpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide");
void vt_fn_6() asm("Pick__8VISpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster");
void vt_fn_7() asm("QueryEmitters__8VISpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery");
void vt_fn_8() asm("QueryIntersection__8VISpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene");
void vt_fn_9() asm("Copy__8VISpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei");
void vt_fn_10() asm("Release__8VISpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide");
VtSlot vt_8VISprite[12] asm("_vt$8VISprite") = {
    {0x0u, vt_fn_0},
    {0x0u, vt_fn_1},
    {0x0u, vt_fn_2},
    {0x0u, vt_fn_3},
    {0x0u, vt_fn_4},
    {0x0u, vt_fn_5},
    {0x0u, vt_fn_6},
    {0x0u, vt_fn_7},
    {0x0u, vt_fn_8},
    {0x0u, vt_fn_9},
    {0x0u, vt_fn_10},
    {0x0u, nullptr},
};
}
