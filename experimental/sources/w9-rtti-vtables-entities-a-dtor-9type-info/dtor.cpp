// Observed _$_9type_info at 0x002fd2a8, 44 bytes. Stores the 9type_info table
// address (0x00453850, _vt$9type_info) into word 1 of the object, at +4. Then,
// when bit 0 of the incoming flag is set, calls __builtin_delete on the object (a
// tail call). Other flag bits do not delete. The first word of the object (the
// name pointer) is not touched.
// Inferred: word 1 is the table pointer, as __rtti_si and __rtti_user use it.

extern "C" {
void __builtin_delete(void* object);
extern const unsigned char norrath_rtti_entities_a_view_453850[];
}

extern "C" void dtor_9type_info(void* object, int flags) asm("_$_9type_info");
extern "C" void dtor_9type_info(void* object, int flags) {
    reinterpret_cast<const void**>(object)[1] = norrath_rtti_entities_a_view_453850;
    if ((flags & 1) != 0) __builtin_delete(object);
}
