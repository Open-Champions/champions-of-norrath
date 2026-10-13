// Observed _$_5Rondo at 0x00315978, 108 bytes (weak). Stores the 5Rondo table pointer (the data
// object at 0x44f3d8, _vt$5Rondo) at object+72, then destroys the list at object+960 with
// listDestroy__FP8ListHead; then calls the destructor _$_8Creature on object+0 with flag 0; finally
// calls blockFree__FPv on the object when bit 0 of the incoming flags is set (a tail call in the
// image). Inferred: this is the 5Rondo destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char norrath_ctors_lava_wraith_view_44f3d8[];
void listDestroy__FP8ListHead(void* list);
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
}

extern "C" void destructor_5rondo(void* self, int flags) __asm__("_$_5Rondo");
extern "C" void destructor_5rondo(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_ctors_lava_wraith_view_44f3d8));
    listDestroy__FP8ListHead(object + 960);
    creature_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
