// Observed _vt$14__si_type_info at 0x004537b0, 48 bytes of initialized data
// (the next object, _vt$16__user_type_info, starts at 0x004537e0). It is six
// 8-byte pairs of a zero adjustment word and a function address, with a zero
// final pair. The addresses were matched to symbols, in order:
//   0x0031acf0 __tf14__si_type_info
//   0x0031ac88 _$_14__si_type_info
//   0x002fd658 do_upcast__C14__si_type_info... (single-inheritance upcast)
//   0x002fd708 do_dyncast__C14__si_type_info...
//   0x002fd828 do_find_public_subobj__C14__si_type_info...
// __rtti_si stores the address of this object into word 1 of a class
// type_info it initializes. Inferred: slot meanings follow the symbol names
// only. Unknown: none of the five targets is authored yet; they stay imports.
extern "C" {
void __tf14__si_type_info();
void _$_14__si_type_info();
void do_upcast__C14__si_type_infoQ216__user_type_info8sub_kindRC9type_infoPvRQ216__user_type_info13upcast_result();
void do_dyncast__C14__si_type_infoiQ216__user_type_info8sub_kindRC9type_infoPvT3T4RQ216__user_type_info14dyncast_result();
void do_find_public_subobj__C14__si_type_infoiRC9type_infoPvT3();
}

struct NorrathLegacyTypeInfoSlot {
    int adjustment;
    void (*function)();
};

#if defined(__mips__)
static_assert(sizeof(NorrathLegacyTypeInfoSlot) == 8, "offset and 32-bit code address");
#endif

extern "C" const NorrathLegacyTypeInfoSlot norrath_legacy_single_inheritance_type_info_vtable_view[6] = {
    {0, __tf14__si_type_info},
    {0, _$_14__si_type_info},
    {0, do_upcast__C14__si_type_infoQ216__user_type_info8sub_kindRC9type_infoPvRQ216__user_type_info13upcast_result},
    {0, do_dyncast__C14__si_type_infoiQ216__user_type_info8sub_kindRC9type_infoPvT3T4RQ216__user_type_info14dyncast_result},
    {0, do_find_public_subobj__C14__si_type_infoiRC9type_infoPvT3},
    {0, nullptr},
};
