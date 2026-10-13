// Observed: __11VILODSprite at 0x01076768 is 60 bytes in the pinned SLUS-20565 image. With the
// object as this it calls __8VISprite(this), then stores 7 at this+52, then stores the address of
// _vt$11VILODSprite at this+60, then stores 0 at this+64. Nothing is returned.
// Inferred: the word at this+60 receives the class table, as for the other VISprite subclasses.
// Unknown: what the word at this+52 selects, and the meaning of the fields at this+64 and beyond;
// the class table is an import. Nothing here ran on the EE.
extern "C" {
void __8VISprite(void* self);
extern unsigned char _vt$11VILODSprite[] __asm__("_vt$11VILODSprite");

void fn_11VILODSprite(void* self) __asm__("__11VILODSprite");
void fn_11VILODSprite(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __8VISprite(self);
    *reinterpret_cast<unsigned*>(bytes + 52) = 7;
    *reinterpret_cast<unsigned*>(bytes + 60) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$11VILODSprite));
    *reinterpret_cast<unsigned*>(bytes + 64) = 0;
}
}
