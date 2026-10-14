// Observed _$_13HammerOfWrath at 0x002b5610, 148 bytes (global symbol in the private executable).
// In order it stores the table _vt$13HammerOfWrath at object+72, updates the byte wrathHammerCount as described below, calls listDestroy__FP8ListHead on object+80, calls listDestroy__FP8ListHead on object+76, and calls the GameObject destructor _$_10GameObject with flag 0; then, when bit 0 of the incoming flags is set, it calls blockFree__FPv on the object
// (a tail call in the image). Other flag bits do not free.
// The wrath step reads the byte wrathHammerCount, then stores that value minus one when bit 7 of the
// difference is clear, and zero otherwise. Observed from the image instructions; the meaning of the counter is inferred.
// Inferred: destructor of the class named by the symbol; the table names are the observed ones, including base
// tables stored after the derived one. Unknown: the meaning of the list heads and the flag-bit uses beyond bit 0.
// Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_13HammerOfWrath[] __asm__("_vt$13HammerOfWrath");
void game_object_destructor(void* self, int flags) __asm__("_$_10GameObject");
void listDestroy__FP8ListHead(void* head);
extern unsigned char wrathHammerCount __asm__("wrathHammerCount");
void blockFree__FPv(void* block);
}

static void store_table(unsigned char* object, int offset, const unsigned char* table) {
    *reinterpret_cast<unsigned*>(object + offset) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(table));
}

extern "C" void destructor_13HammerOfWrath(void* self, int flags) __asm__("_$_13HammerOfWrath");
extern "C" void destructor_13HammerOfWrath(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, vt_13HammerOfWrath);
    const int next = static_cast<int>(wrathHammerCount) - 1;
    wrathHammerCount = static_cast<unsigned char>((next & 0x80) == 0 ? next : 0);
    listDestroy__FP8ListHead(object + 80);
    listDestroy__FP8ListHead(object + 76);
    game_object_destructor(object, 0);
    if (release) blockFree__FPv(object);
}
