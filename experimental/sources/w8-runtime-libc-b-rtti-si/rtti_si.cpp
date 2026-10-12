// Observed __rtti_si at 0x002fd348, 32 bytes. A null record returns at once.
// Otherwise it writes three words: the name pointer at +0, the address of
// _vt$14__si_type_info (0x004537b0, 48 bytes) at +4, and the base record
// pointer at +8. The vtable view is owned by another package.
extern "C" const unsigned char norrath_legacy_single_inheritance_type_info_vtable_view[];

extern "C" void norrath_rtti_si(void* record, const char* name, void* base) {
    if (record == nullptr) return;
    const void** words = static_cast<const void**>(record);
    words[2] = base;
    words[0] = name;
    words[1] = norrath_legacy_single_inheritance_type_info_vtable_view;
}
