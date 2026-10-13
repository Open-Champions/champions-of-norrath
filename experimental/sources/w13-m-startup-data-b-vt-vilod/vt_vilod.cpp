// Observed _vt$11VILODSprite at 0x01206f40, 96 bytes in .srs (weak object in the image).
// 12 slots of 8 bytes: a zero adjustment word and a function word. The last slot is null.
// The 11 function words, in slot order, are the observed symbol names listed below; each
// name is the image symbol at that address. The bodies are not in this unit.
// Inferred: a constructor of this class stores this table address in the object (another unit).
// Unknown: dispatch order beyond the table itself, and the role of the null last slot.
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VtSlot) == 8, "slot is an adjustment word plus a pointer");
#endif
extern "C" {
void vt_vilod_fn_0() asm("__tf11VILODSprite");
void vt_vilod_fn_1() asm("_$_11VILODSprite");
void vt_vilod_fn_2() asm("GetInstType__C8VISpriteR7VIScene");
void vt_vilod_fn_3() asm("Raster__11VILODSpriteR8VICameraR7VISceneR8VIRasterUi");
void vt_vilod_fn_4() asm("Collide__11VILODSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide");
void vt_vilod_fn_5() asm("Collide__11VILODSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide");
void vt_vilod_fn_6() asm("Pick__11VILODSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster");
void vt_vilod_fn_7() asm("QueryEmitters__8VISpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery");
void vt_vilod_fn_8() asm("QueryIntersection__11VILODSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene");
void vt_vilod_fn_9() asm("Copy__11VILODSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei");
void vt_vilod_fn_10() asm("Release__11VILODSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide");
VtSlot vt_vilod[12] asm("_vt$11VILODSprite") = {
    {0x0u, vt_vilod_fn_0},
    {0x0u, vt_vilod_fn_1},
    {0x0u, vt_vilod_fn_2},
    {0x0u, vt_vilod_fn_3},
    {0x0u, vt_vilod_fn_4},
    {0x0u, vt_vilod_fn_5},
    {0x0u, vt_vilod_fn_6},
    {0x0u, vt_vilod_fn_7},
    {0x0u, vt_vilod_fn_8},
    {0x0u, vt_vilod_fn_9},
    {0x0u, vt_vilod_fn_10},
    {0x0u, nullptr},
};
}
