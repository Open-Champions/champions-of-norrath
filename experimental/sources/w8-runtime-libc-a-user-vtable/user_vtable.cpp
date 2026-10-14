// Observed _vt$16__user_type_info at 0x004537e0, 48 bytes of initialized data
// (the next object, _vt$10bad_typeid, starts at 0x00453810). Same shape as the
// single-inheritance table: six 8-byte pairs of a zero adjustment word and a
// function address, with a zero final pair. Matched symbols, in order:
//   0x0031ab68 __tf16__user_type_info
//   0x0031ab00 _$_16__user_type_info
//   0x002fd548 do_upcast__C16__user_type_info...
//   0x002fd5a8 do_dyncast__C16__user_type_info...
//   0x002fd640 do_find_public_subobj__C16__user_type_info...
// __rtti_user stores the address of this object into word 1 of a user type_info.
// Inferred: slot meanings follow the symbol names only. Unknown: none of the
// five targets is authored yet; they stay imports.
extern "C" {
void __tf16__user_type_info();
void _$_16__user_type_info();
void do_upcast__C16__user_type_infoQ216__user_type_info8sub_kindRC9type_infoPvRQ216__user_type_info13upcast_result();
void do_dyncast__C16__user_type_infoiQ216__user_type_info8sub_kindRC9type_infoPvT3T4RQ216__user_type_info14dyncast_result();
void do_find_public_subobj__C16__user_type_infoiRC9type_infoPvT3();
}

struct NorrathLegacyTypeInfoSlot {
    int adjustment;
    void (*function)();
};

#if defined(__mips__)
static_assert(sizeof(NorrathLegacyTypeInfoSlot) == 8, "offset and 32-bit code address");
#endif

extern "C" const NorrathLegacyTypeInfoSlot norrath_legacy_user_type_info_vtable_view[6] = {
    {0, __tf16__user_type_info},
    {0, _$_16__user_type_info},
    {0, do_upcast__C16__user_type_infoQ216__user_type_info8sub_kindRC9type_infoPvRQ216__user_type_info13upcast_result},
    {0, do_dyncast__C16__user_type_infoiQ216__user_type_info8sub_kindRC9type_infoPvT3T4RQ216__user_type_info14dyncast_result},
    {0, do_find_public_subobj__C16__user_type_infoiRC9type_infoPvT3},
    {0, nullptr},
};
