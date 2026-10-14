// Observed _$_11SpiderQueen at 0x0030f680, 108 bytes (weak). Stores the 11SpiderQueen table
// pointer (_vt$11SpiderQueen) at object+72, calls listDestroy__FP8ListHead on the list head at
// object+996, calls the Creature destructor (_$_8Creature) with flag 0, then calls blockFree__FPv
// on the object when bit 0 of the incoming flags is set. Other flag bits do not free. The free is
// a tail call in the image.
// Inferred: this is the 11SpiderQueen destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_11SpiderQueen[] __asm__("_vt$11SpiderQueen");
void listDestroy__FP8ListHead(void* head);
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
}

extern "C" void destructor_11SpiderQueen(void* self, int flags) __asm__("_$_11SpiderQueen");
extern "C" void destructor_11SpiderQueen(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_11SpiderQueen));
    listDestroy__FP8ListHead(object + 996);
    creature_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
