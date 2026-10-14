// Observed _$_10bad_typeid at 0x0031aac8, 52 bytes. It stores the table address of _vt$9exception (0x00453930)
// into word 0 of the object (the store is in the branch delay slot, so it runs for every flag value). When bit 0
// of the incoming flag is set it tail-calls __builtin_delete on the object. Other flag bits do not delete.
// Inferred: the destructor of a class derived from exception, with the base table installed first.
// Unknown: the layout of the rest of the object, which this destructor does not touch.
extern "C" {
void __builtin_delete(void* object);
extern const unsigned char vt_9exception[] __asm__("_vt$9exception");
}

extern "C" void dtor_10bad_typeid(void* object, int flags) __asm__("_$_10bad_typeid");
extern "C" void dtor_10bad_typeid(void* object, int flags) {
    reinterpret_cast<const void**>(object)[0] = vt_9exception;
    if ((flags & 1) != 0) __builtin_delete(object);
}
