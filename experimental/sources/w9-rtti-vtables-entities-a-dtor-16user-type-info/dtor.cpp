// Observed _$_16__user_type_info at 0x0031ab00, 100 bytes. Stores the 16__user_type_info table address
// (0x004537e0, _vt$16__user_type_info) into word 1 of the object, at +4, then calls
// the 9type_info destructor on the object with flag 0, then, when bit 0 of the
// incoming flag is set, calls __builtin_delete on the object (a tail call). Other
// flag bits do not delete, and the incoming flag is not passed on.
// Observed: the single-inheritance form stores the same user table in its
// destructor, not its own; the two destructors have the same body.
// Inferred: word 1 is the table pointer, as __rtti_si and __rtti_user use it.

extern "C" {
void type_info_destructor(void* object, int flags) asm("_$_9type_info");
void __builtin_delete(void* object);
extern const unsigned char norrath_legacy_user_type_info_vtable_view[];
}

extern "C" void dtor_16__user_type_info(void* object, int flags) asm("_$_16__user_type_info");
extern "C" void dtor_16__user_type_info(void* object, int flags) {
    reinterpret_cast<const void**>(object)[1] = norrath_legacy_user_type_info_vtable_view;
    type_info_destructor(object, 0);
    if ((flags & 1) != 0) __builtin_delete(object);
}
