// Observed __rtti_user at 0x002fd368, 28 bytes. A null record returns at once. Otherwise word 0
// of the record receives the name pointer and word 1 the address of _vt$16__user_type_info
// (0x004537e0, 48 bytes of data); nothing else is touched. Nothing is returned.
// This unit carries the original symbol name so that the type-info routines that import it
// resolve. Earlier packages authored the same body under the name norrath_rtti_user; the
// vtable is taken through the existing authored view of that table.
// Unknown: what the record's later words hold; dispatch through the stored table.
// Nothing was executed on the EE.
extern "C" {
extern const unsigned char norrath_legacy_user_type_info_vtable_view[];

void __rtti_user(const void** record, const char* name) {
    if (record == nullptr) return;
    record[0] = name;
    record[1] = norrath_legacy_user_type_info_vtable_view;
}
}
