// Observed _$_6Minion at 0x00315af0, 120 bytes (weak).
// Stores the Minion table pointer (the .data object at 0x44f5b8, _vt$6Minion) at object+72, destroys
// the lists at object+968 and then object+960 with listDestroy__FP8ListHead, calls the Creature
// destructor (_$_8Creature) on the object with flag 0, then calls blockFree__FPv on the object when
// bit 0 of the incoming flags is set. The free is a tail call in the image.
// Inferred: this is the Minion destructor from its symbol name; the two lists are heads (the type
// of the callee's parameter). Nothing here ran on the EE.
extern "C" {
extern unsigned char norrath_ctors_lava_wraith_view_44f5b8[];
void listDestroy__FP8ListHead(void* list);
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
}

extern "C" void minion_destructor(void* self, int flags) __asm__("_$_6Minion");
extern "C" void minion_destructor(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_ctors_lava_wraith_view_44f5b8));
    listDestroy__FP8ListHead(object + 968);
    listDestroy__FP8ListHead(object + 960);
    creature_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
