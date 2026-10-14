// Observed __rtti_user at 0x002fd368, 28 bytes. A null record returns at
// once. Otherwise it writes the name pointer at +0 and the address of
// _vt$16__user_type_info (0x004537e0, 48 bytes) at +4. Nothing else is
// touched. The vtable view is owned by another package.
extern "C" const unsigned char norrath_legacy_user_type_info_vtable_view[];

extern "C" void norrath_rtti_user(void* record, const char* name) {
    if (record == nullptr) return;
    const void** words = static_cast<const void**>(record);
    words[0] = name;
    words[1] = norrath_legacy_user_type_info_vtable_view;
}
