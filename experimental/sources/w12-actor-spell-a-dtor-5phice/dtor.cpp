// Observed _$_5PhIce at 0x0030ddd0, 100 bytes (weak).
// In order it stores the table _vt$4Prop at object+72, then calls the GameObject destructor (_$_10GameObject) with flag 0.
// Finally, when bit 0 of the incoming flags is set, it calls blockFree__FPv on the object (a tail call in the image).
// Other flag bits do not free. Inferred: this is the destructor of the class from its symbol name; the table name stored
// is the observed one even where it is a base class's. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_4Prop[] __asm__("_vt$4Prop");
void game_object_destructor(void* self, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* block);
}

static void store_table(unsigned char* object, int offset, const unsigned char* table) {
    *reinterpret_cast<unsigned*>(object + offset) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(table));
}

extern "C" void destructor_5PhIce(void* self, int flags) __asm__("_$_5PhIce");
extern "C" void destructor_5PhIce(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    store_table(object, 72, vt_4Prop);
    game_object_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
