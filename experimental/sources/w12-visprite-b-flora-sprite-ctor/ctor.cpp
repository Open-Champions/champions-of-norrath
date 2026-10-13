// Observed: __13VIFloraSprite at 0x010061d8 is 68 bytes in the pinned SLUS-20565 image. With the
// object as this it calls __8VISprite(this), then stores 13 at this+52, then stores -1 at
// this+68, then stores the address of _vt$13VIFloraSprite at this+60, then stores -1 at this+64.
// Nothing is returned.
// Inferred: the word at this+60 receives the class table, as for the other VISprite subclasses.
// Unknown: what the word at this+52 selects, and the meaning of the fields at this+64 and beyond;
// the class table is an import. Nothing here ran on the EE.
extern "C" {
void __8VISprite(void* self);
extern unsigned char _vt$13VIFloraSprite[] __asm__("_vt$13VIFloraSprite");

void fn_13VIFloraSprite(void* self) __asm__("__13VIFloraSprite");
void fn_13VIFloraSprite(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __8VISprite(self);
    *reinterpret_cast<unsigned*>(bytes + 52) = 13;
    *reinterpret_cast<unsigned*>(bytes + 68) = static_cast<unsigned>(-1);
    *reinterpret_cast<unsigned*>(bytes + 60) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$13VIFloraSprite));
    *reinterpret_cast<unsigned*>(bytes + 64) = static_cast<unsigned>(-1);
}
}
