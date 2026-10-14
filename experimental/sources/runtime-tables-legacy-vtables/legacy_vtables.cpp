// Observed _vt$14__si_type_info at 0x4537b0 and _vt$16__user_type_info at
// 0x4537e0, 48 bytes each in .data. Each holds six slots; every slot is a
// zero adjustment word plus a function word. The function words, in order,
// are the __tf constructor, the _$_ destructor, do_upcast, do_dyncast, and
// do_find_public_subobj of that legacy class, then a null slot. The bodies
// behind those words are not in this package, so the slots reference the
// observed names and stay unresolved for the typeinfo work.
// Inferred: they are norrath_legacy_single_inheritance_type_info_vtable_view
// and norrath_legacy_user_type_info_vtable_view, read by address only from
// the energy-rtti initializers.
// Unknown: dispatch through these tables and the bodies of the slot
// functions.
struct LegacySlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(LegacySlot) == 8, "legacy slot is an adjustment word plus a pointer");
#endif
extern "C" {
void legacy_si_fn_0() asm("__tf14__si_type_info");
void legacy_si_fn_1() asm("_$_14__si_type_info");
void legacy_si_fn_2() asm("do_upcast__C14__si_type_infoQ216__user_type_info8sub_kindRC9type_infoPvRQ216__user_type_info13upcast_result");
void legacy_si_fn_3() asm("do_dyncast__C14__si_type_infoiQ216__user_type_info8sub_kindRC9type_infoPvT3T4RQ216__user_type_info14dyncast_result");
void legacy_si_fn_4() asm("do_find_public_subobj__C14__si_type_infoiRC9type_infoPvT3");
void legacy_user_fn_0() asm("__tf16__user_type_info");
void legacy_user_fn_1() asm("_$_16__user_type_info");
void legacy_user_fn_2() asm("do_upcast__C16__user_type_infoQ216__user_type_info8sub_kindRC9type_infoPvRQ216__user_type_info13upcast_result");
void legacy_user_fn_3() asm("do_dyncast__C16__user_type_infoiQ216__user_type_info8sub_kindRC9type_infoPvT3T4RQ216__user_type_info14dyncast_result");
void legacy_user_fn_4() asm("do_find_public_subobj__C16__user_type_infoiRC9type_infoPvT3");
LegacySlot norrath_legacy_single_inheritance_type_info_vtable_view[6] = {
    {0x0u, legacy_si_fn_0},
    {0x0u, legacy_si_fn_1},
    {0x0u, legacy_si_fn_2},
    {0x0u, legacy_si_fn_3},
    {0x0u, legacy_si_fn_4},
    {0x0u, nullptr},
};
LegacySlot norrath_legacy_user_type_info_vtable_view[6] = {
    {0x0u, legacy_user_fn_0},
    {0x0u, legacy_user_fn_1},
    {0x0u, legacy_user_fn_2},
    {0x0u, legacy_user_fn_3},
    {0x0u, legacy_user_fn_4},
    {0x0u, nullptr},
};
}
