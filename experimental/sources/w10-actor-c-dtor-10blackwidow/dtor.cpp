// Observed _$_10BlackWidow at 0x0030ea70, 108 bytes.
// Stores the 10BlackWidow vtable (0x44c260) at object+72, destroys the list head at object+1004
// (listDestroy), calls the Creature destructor with flag 0, then blockFree on the object when
// bit 0 of the incoming flag is set. Other flag bits do not free.
// Inferred: this is the class's destructor, from the symbol name; the +1004 field is a ListHead
// from the callee's declared parameter. Not executed on the EE.

extern "C" {
void listDestroy__FP8ListHead(void* head);
void creature_destructor(void* object, int flags) asm("_$_8Creature");
void norrath_block_free(void* object);
extern const unsigned char norrath_ctors_ant_lantern_view_44c260[];
}

namespace {
unsigned low_pointer(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
}

extern "C" void dtor_10BlackWidow(void* object_storage, int flags) asm("_$_10BlackWidow");
extern "C" void dtor_10BlackWidow(void* object_storage, int flags) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    write_word(object + 72u, low_pointer(norrath_ctors_ant_lantern_view_44c260));
    listDestroy__FP8ListHead(object + 1004u);
    creature_destructor(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
