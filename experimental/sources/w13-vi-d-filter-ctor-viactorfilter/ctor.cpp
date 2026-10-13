// Observed: __13VIActorFilter at 0x0105ebe8 is 32 bytes in the pinned SLUS-20565 image. With the object as this it stores the
// address of _vt$13VIActorFilter at +4 and then jumps to Init__13VIActorFilter with the object unchanged, so whatever that
// routine returns is what the constructor returns. It reads nothing from the object.
// Inferred: the word at +4 is the class table, as for the other VI classes with a table; the first word (+0) is set to -1
// by Init (seen in that routine's own body, not authored here).
// Unknown: what Init__13VIActorFilter does beyond that, and the object size. Init is an import. Nothing here ran on the EE.
extern "C" {
void Init__13VIActorFilter(void* self);
extern unsigned char _vt$13VIActorFilter[] __asm__("_vt$13VIActorFilter");

void __13VIActorFilter(void* self) {
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 4) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$13VIActorFilter));
    Init__13VIActorFilter(self);
}
}
