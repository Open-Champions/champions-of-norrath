// Observed _$_9MummyKing at 0x00315f58, 228 bytes (weak).
// Stores the MummyKing table pointer (_vt$9MummyKing) at object+72, then destroys three list members
// with listDestroy__FP8ListHead, last to first: the four heads at +1136, +1140, +1144, +1148
// (destroyed from +1148 down to +1136), the single head at +1124, then the four heads at +1108.. +1120
// (destroyed from +1120 down to +1108). Then the Creature destructor (_$_8Creature) runs with flag 0
// and, when bit 0 of the incoming flags is set, blockFree__FPv(object) frees the object (a tail call
// in the image). The image also compares the object address against -1136 and -1108 before the
// array loops, the usual guard for a null object; it is not reproduced. Each head is one word.
// Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_MummyKing[] __asm__("_vt$9MummyKing");
void listDestroy__FP8ListHead(void* head);
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
}

extern "C" void destructor_MummyKing(void* self, int flags) __asm__("_$_9MummyKing");
extern "C" void destructor_MummyKing(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_MummyKing));
    for (int i = 3; i >= 0; --i) listDestroy__FP8ListHead(object + 1136 + 4 * i);
    listDestroy__FP8ListHead(object + 1124);
    for (int i = 3; i >= 0; --i) listDestroy__FP8ListHead(object + 1108 + 4 * i);
    creature_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
