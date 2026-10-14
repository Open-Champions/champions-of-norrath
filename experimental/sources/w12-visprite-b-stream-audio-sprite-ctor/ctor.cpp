// Observed: __19VIStreamAudioSprite at 0x01018eb8 is 72 bytes in the pinned SLUS-20565 image.
// With the object as this it calls __8VISprite(this), then stores 1 at this+64, then stores the
// address of _vt$19VIStreamAudioSprite at this+60, then stores 12 at this+52, then stores byte 0
// at this+72, then stores 0 at this+68. Nothing is returned.
// Inferred: the word at this+60 receives the class table, as for the other VISprite subclasses.
// Unknown: what the word at this+52 selects, and the meaning of the fields at this+64 and beyond;
// the class table is an import. Nothing here ran on the EE.
extern "C" {
void __8VISprite(void* self);
extern unsigned char _vt$19VIStreamAudioSprite[] __asm__("_vt$19VIStreamAudioSprite");

void fn_19VIStreamAudioSprite(void* self) __asm__("__19VIStreamAudioSprite");
void fn_19VIStreamAudioSprite(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __8VISprite(self);
    *reinterpret_cast<unsigned*>(bytes + 64) = 1;
    *reinterpret_cast<unsigned*>(bytes + 60) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$19VIStreamAudioSprite));
    *reinterpret_cast<unsigned*>(bytes + 52) = 12;
    bytes[72] = 0;
    *reinterpret_cast<unsigned*>(bytes + 68) = 0;
}
}
