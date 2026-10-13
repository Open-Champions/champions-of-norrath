// Observed: __13VISoundSprite at 0x0117c1d8 is 100 bytes in the pinned SLUS-20565 image. With the
// object as this it calls __8VISprite(this), then stores 9 at this+52, then stores the float 1.0
// at this+84, then stores the address of _vt$13VISoundSprite at this+60, then stores -1 at
// this+64, then stores 268435456 at this+72, then stores 1 at this+80, then stores -1 at this+68,
// then stores 0 at this+76. Nothing is returned.
// Inferred: the word at this+60 receives the class table, as for the other VISprite subclasses.
// Unknown: what the word at this+52 selects, and the meaning of the fields at this+64 and beyond;
// the class table is an import. Nothing here ran on the EE.
extern "C" {
void __8VISprite(void* self);
extern unsigned char _vt$13VISoundSprite[] __asm__("_vt$13VISoundSprite");

void fn_13VISoundSprite(void* self) __asm__("__13VISoundSprite");
void fn_13VISoundSprite(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __8VISprite(self);
    *reinterpret_cast<unsigned*>(bytes + 52) = 9;
    *reinterpret_cast<float*>(bytes + 84) = 1.0f;
    *reinterpret_cast<unsigned*>(bytes + 60) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$13VISoundSprite));
    *reinterpret_cast<unsigned*>(bytes + 64) = static_cast<unsigned>(-1);
    *reinterpret_cast<unsigned*>(bytes + 72) = 268435456;
    *reinterpret_cast<unsigned*>(bytes + 80) = 1;
    *reinterpret_cast<unsigned*>(bytes + 68) = static_cast<unsigned>(-1);
    *reinterpret_cast<unsigned*>(bytes + 76) = 0;
}
}
