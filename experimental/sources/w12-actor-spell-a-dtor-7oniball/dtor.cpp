// Observed _$_7OniBall at 0x00310d18, 132 bytes (weak).
// In order it stores the table _vt$7OniBall at object+72, calls the Light destructor (_$_5Light) on object+120 with flag 2, stores the table _vt$10Projectile at object+72, calls listDestroy__FP8ListHead on object+76, then calls the GameObject destructor (_$_10GameObject) with flag 0.
// Finally, when bit 0 of the incoming flags is set, it calls blockFree__FPv on the object (a tail call in the image).
// Other flag bits do not free. Inferred: this is the destructor of the class from its symbol name; the table name stored
// is the observed one even where it is a base class's. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_7OniBall[] __asm__("_vt$7OniBall");
extern unsigned char vt_10Projectile[] __asm__("_vt$10Projectile");
void game_object_destructor(void* self, int flags) __asm__("_$_10GameObject");
void light_destructor(void* self, int flags) __asm__("_$_5Light");
void listDestroy__FP8ListHead(void* head);
void blockFree__FPv(void* block);
}

static void store_table(unsigned char* object, int offset, const unsigned char* table) {
    *reinterpret_cast<unsigned*>(object + offset) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(table));
}

extern "C" void destructor_7OniBall(void* self, int flags) __asm__("_$_7OniBall");
extern "C" void destructor_7OniBall(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    store_table(object, 72, vt_7OniBall);
    light_destructor(object + 120, 2);
    store_table(object, 72, vt_10Projectile);
    listDestroy__FP8ListHead(object + 76);
    game_object_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
