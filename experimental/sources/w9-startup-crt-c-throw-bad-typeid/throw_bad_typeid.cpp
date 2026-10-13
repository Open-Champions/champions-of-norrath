// Observed __throw_bad_typeid at 0x002ff3c8, 100 bytes. Straight-line part, in order: allocate 4 bytes
// with __eh_alloc; store the address of _vt$10bad_typeid (0x00453810) into that block's first word;
// call __tf10bad_typeid with no arguments; call __cp_push_exception with the block, the
// __tf10bad_typeid result, and the address of _$_10bad_typeid (0x0031aac8); call __throw. __throw
// is followed by a second region at 0x002ff414 that stores the address of _vt$9exception
// (0x00453930) into a stack slot and calls __rethrow with the data at 0x0047f910. That region is
// reachable only while an exception passes through this function; it is not reproduced here,
// because this profile builds with -fno-exceptions and no unwind-table evidence was used. Inferred
// only: the first word is the object's vtable pointer and the third argument is its destructor,
// from the symbol names. Nothing here was executed on the EE.
extern "C" {
void* eh_alloc(unsigned size) asm("__eh_alloc");
void* tf_bad_typeid() asm("__tf10bad_typeid");
void cp_push_exception(void* object, void* type, void (*destructor)()) asm("__cp_push_exception");
void raise_exception() asm("__throw");
void bad_typeid_destructor() asm("_$_10bad_typeid");
extern unsigned char bad_typeid_vtable[] asm("_vt$10bad_typeid");
}

extern "C" void __throw_bad_typeid() {
    const void** object = static_cast<const void**>(eh_alloc(4));
    *object = bad_typeid_vtable;
    cp_push_exception(object, tf_bad_typeid(), bad_typeid_destructor);
    raise_exception();
}
