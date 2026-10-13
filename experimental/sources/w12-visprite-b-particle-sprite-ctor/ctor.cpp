// Observed: __16VIParticleSprite at 0x010aa660 is 76 bytes in the pinned SLUS-20565 image. With
// the object as this it calls __8VISprite(this), then stores the address of
// _vt$16VIParticleSprite at this+60, then calls __17VIParticleEmitter(this+80), then stores 10 at
// this+52, then stores -1 at this+64, then stores 0 at this+1072. Nothing is returned.
// Inferred: the word at this+60 receives the class table, as for the other VISprite subclasses.
// Unknown: what the word at this+52 selects, and the meaning of the fields at this+64 and beyond;
// the class table is an import. Nothing here ran on the EE.
extern "C" {
void __8VISprite(void* self);
void __17VIParticleEmitter(void* self);
extern unsigned char _vt$16VIParticleSprite[] __asm__("_vt$16VIParticleSprite");

void fn_16VIParticleSprite(void* self) __asm__("__16VIParticleSprite");
void fn_16VIParticleSprite(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __8VISprite(self);
    *reinterpret_cast<unsigned*>(bytes + 60) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$16VIParticleSprite));
    __17VIParticleEmitter(bytes + 80);
    *reinterpret_cast<unsigned*>(bytes + 52) = 10;
    *reinterpret_cast<unsigned*>(bytes + 64) = static_cast<unsigned>(-1);
    *reinterpret_cast<unsigned*>(bytes + 1072) = 0;
}
}
