// Observed __rtti_si at 0x002fd348, 24 bytes. With a null object it does
// nothing. Otherwise it stores, into a 12-byte type_info object: word 0 the
// name pointer (second argument), word 1 the address of the single-inheritance
// type_info table at 0x004537b0 (_vt$14__si_type_info), word 2 the base
// type_info pointer (third argument). Nothing is returned.
// Inferred: word 0 is the name because __eq__C9type_infoRC9type_info reads it
// as strcmp text; word 2 is the base because callers pass the parent's
// __ti object there (see the light and prop typeinfo builders).
extern "C" {
extern const unsigned char norrath_legacy_single_inheritance_type_info_vtable_view[];

void __rtti_si(void* object, const char* name, void* base) {
    if (object == nullptr) return;
    void** words = static_cast<void**>(object);
    words[2] = base;
    words[0] = const_cast<char*>(name);
    words[1] = const_cast<unsigned char*>(norrath_legacy_single_inheritance_type_info_vtable_view);
}
}
