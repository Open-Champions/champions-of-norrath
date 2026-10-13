// Observed: __13VISceneFilter at 0x011382f0 is 32 bytes in the pinned SLUS-20565 image. With the object as this it stores the
// address of _vt$13VISceneFilter at +16 and then jumps to Init__13VISceneFilter with the object unchanged, so whatever that
// routine returns is what the constructor returns. It reads nothing from the object.
// Inferred: the word at +16 is the class table; the words at +0..+12 are set to -1 by Init (seen in that routine's own body,
// not authored here).
// Unknown: what Init__13VISceneFilter does beyond that, and the object size. Init is an import. Nothing here ran on the EE.
extern "C" {
void Init__13VISceneFilter(void* self);
extern unsigned char _vt$13VISceneFilter[] __asm__("_vt$13VISceneFilter");

void __13VISceneFilter(void* self) {
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 16) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$13VISceneFilter));
    Init__13VISceneFilter(self);
}
}
