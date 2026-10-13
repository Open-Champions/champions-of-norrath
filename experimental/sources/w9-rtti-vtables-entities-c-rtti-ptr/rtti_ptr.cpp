// Observed __rtti_ptr at 0x2fe658, 32 bytes, emitted here as norrath_rtti_ptr (the
// name its callers import). Takes a 12-byte type_info object,
// a name pointer and a target type_info pointer. A null object does nothing.
// Otherwise it stores the name at +0, the table address 0x4538f8
// (_vt$19__pointer_type_info) at +4 and the target at +8. Nothing is returned.
// Inferred: the object is the legacy pointer type_info form, with the third word
// pointing at the pointee's type_info. Unknown: whether other readers expect the
// name word to be unique.
extern "C" {
extern const unsigned char norrath_legacy_pointer_type_info_vtable_view[];
}

extern "C" void norrath_rtti_ptr(void* object, const char* name, void* target);
extern "C" void norrath_rtti_ptr(void* object, const char* name, void* target) {
    if (object == nullptr) return;
    const void** words = static_cast<const void**>(object);
    words[2] = target;
    words[0] = name;
    words[1] = norrath_legacy_pointer_type_info_vtable_view;
}
