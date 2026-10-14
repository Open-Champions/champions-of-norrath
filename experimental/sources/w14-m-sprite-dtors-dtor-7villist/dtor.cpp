// Observed _$_7VILList at 0x010727a0, 44 bytes.
// In order: when bit 0 of flags is set, tail-calls __builtin_delete(object).
// Inferred: this is the deleting destructor of the class from its symbol name and the store of its table
// at object+60; the order of the calls is the order in the image. Nothing here was executed on the EE.

extern "C" {
}

extern "C" void free_object(void* pointer) __asm__("__builtin_delete");

extern "C" void destructor_7VILList(void* self, int flags) __asm__("_$_7VILList");
extern "C" void destructor_7VILList(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if ((flags & 1) != 0) free_object(object);
}
