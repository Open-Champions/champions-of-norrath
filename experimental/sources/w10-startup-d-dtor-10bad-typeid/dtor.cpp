// Observed _$_10bad_typeid at 0x0031aac8, 52 bytes (weak). Stores the address of _vt$9exception
// (0x00453930, the base class table) into word 0 of the object. Then, when bit 0 of the
// incoming flag is set, calls __builtin_delete on the object as a tail call; other flag bits
// do not delete.
// Inferred: word 0 is the table pointer, as the stored symbol and the type_info accessor
// suggest; the class has no members of its own here.
// Nothing here was executed on the EE.
extern "C" {
void __builtin_delete(void* object);
extern unsigned char _vt$9exception[];

void _$_10bad_typeid(void** object, int flags) {
    object[0] = _vt$9exception;
    if ((flags & 1) != 0) __builtin_delete(object);
}
}
