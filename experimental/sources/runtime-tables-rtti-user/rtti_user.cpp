// Observed __rtti_user at 0x2fd368, 28 bytes. When the typeinfo pointer is
// null it stores nothing. Otherwise word 0 receives the name pointer and
// word 1 receives the legacy user-type-info vtable, which the image holds
// as the absolute address 0x4537e0 (_vt$16__user_type_info).
// Inferred: this reconstruction name is that behavior; the stored vtable
// word is the address of norrath_legacy_user_type_info_vtable_view, the
// separately authored view of that table, since the partial link carries no
// absolute game addresses.
// Unknown: the object at the stored address is only taken by address here;
// dispatch through it is not established.
namespace {
unsigned address_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
void write_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
} // namespace

extern "C" {
extern const unsigned char norrath_legacy_user_type_info_vtable_view[];

void norrath_rtti_user(void* typeinfo, const char* name) {
    if (typeinfo == nullptr) return;
    auto* words = static_cast<unsigned char*>(typeinfo);
    write_word(words, address_of(name));
    write_word(words + 4, address_of(norrath_legacy_user_type_info_vtable_view));
}
}
