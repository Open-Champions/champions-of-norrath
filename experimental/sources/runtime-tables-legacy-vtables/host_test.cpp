// Host comparison only. Not an EE build input. Links against
// legacy_vtables.cpp compiled for the host.
#include <cstdio>

const char* hit = "";
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
extern "C" void legacy_si_fn_0() asm("__tf14__si_type_info");
extern "C" void legacy_si_fn_0() { hit = "__tf14__si_type_info"; }
extern "C" void legacy_si_fn_1() asm("_$_14__si_type_info");
extern "C" void legacy_si_fn_1() { hit = "_$_14__si_type_info"; }
extern "C" void legacy_si_fn_2() asm("do_upcast__C14__si_type_infoQ216__user_type_info8sub_kindRC9type_infoPvRQ216__user_type_info13upcast_result");
extern "C" void legacy_si_fn_2() {
    hit = "do_upcast__C14__si_type_info";
}
extern "C" void legacy_si_fn_3() asm("do_dyncast__C14__si_type_infoiQ216__user_type_info8sub_kindRC9type_infoPvT3T4RQ216__user_type_info14dyncast_result");
extern "C" void legacy_si_fn_3() {
    hit = "do_dyncast__C14__si_type_info";
}
extern "C" void legacy_si_fn_4() asm("do_find_public_subobj__C14__si_type_infoiRC9type_infoPvT3");
extern "C" void legacy_si_fn_4() {
    hit = "do_find_public_subobj__C14__si_type_info";
}
extern "C" void legacy_user_fn_0() asm("__tf16__user_type_info");
extern "C" void legacy_user_fn_0() { hit = "__tf16__user_type_info"; }
extern "C" void legacy_user_fn_1() asm("_$_16__user_type_info");
extern "C" void legacy_user_fn_1() { hit = "_$_16__user_type_info"; }
extern "C" void legacy_user_fn_2() asm("do_upcast__C16__user_type_infoQ216__user_type_info8sub_kindRC9type_infoPvRQ216__user_type_info13upcast_result");
extern "C" void legacy_user_fn_2() {
    hit = "do_upcast__C16__user_type_info";
}
extern "C" void legacy_user_fn_3() asm("do_dyncast__C16__user_type_infoiQ216__user_type_info8sub_kindRC9type_infoPvT3T4RQ216__user_type_info14dyncast_result");
extern "C" void legacy_user_fn_3() {
    hit = "do_dyncast__C16__user_type_info";
}
extern "C" void legacy_user_fn_4() asm("do_find_public_subobj__C16__user_type_infoiRC9type_infoPvT3");
extern "C" void legacy_user_fn_4() {
    hit = "do_find_public_subobj__C16__user_type_info";
}
struct LegacySlot {
    unsigned adjustment;
    void (*function)();
};
extern "C" LegacySlot norrath_legacy_single_inheritance_type_info_vtable_view[6];
extern "C" LegacySlot norrath_legacy_user_type_info_vtable_view[6];
static int check_slot(LegacySlot* table, int slot, const char* want, const char* what) {
    if (table[slot].adjustment != 0x0u) return 1;
    hit = "";
    table[slot].function();
    if (!same(hit, want)) {
        std::printf("%s slot %d hit %s want %s\n", what, slot, hit, want);
        return 1;
    }
    return 0;
}
int main() {
    // Total sizes are an EE property (48 bytes each: six 8-byte slots) and
    // are checked on the EE object, not here: host pointers are wider.
    if (sizeof(LegacySlot) < 8u) return 1;
    if (check_slot(norrath_legacy_single_inheritance_type_info_vtable_view, 0, "__tf14__si_type_info", "si"))
        return 3;
    if (check_slot(norrath_legacy_single_inheritance_type_info_vtable_view, 1, "_$_14__si_type_info", "si"))
        return 4;
    if (check_slot(norrath_legacy_single_inheritance_type_info_vtable_view, 2, "do_upcast__C14__si_type_info",
                   "si"))
        return 5;
    if (check_slot(norrath_legacy_single_inheritance_type_info_vtable_view, 3, "do_dyncast__C14__si_type_info",
                   "si"))
        return 6;
    if (check_slot(norrath_legacy_single_inheritance_type_info_vtable_view, 4,
                   "do_find_public_subobj__C14__si_type_info", "si"))
        return 7;
    if (check_slot(norrath_legacy_user_type_info_vtable_view, 0, "__tf16__user_type_info", "user")) return 8;
    if (check_slot(norrath_legacy_user_type_info_vtable_view, 1, "_$_16__user_type_info", "user")) return 9;
    if (check_slot(norrath_legacy_user_type_info_vtable_view, 2, "do_upcast__C16__user_type_info", "user"))
        return 10;
    if (check_slot(norrath_legacy_user_type_info_vtable_view, 3, "do_dyncast__C16__user_type_info", "user"))
        return 11;
    if (check_slot(norrath_legacy_user_type_info_vtable_view, 4, "do_find_public_subobj__C16__user_type_info",
                   "user"))
        return 12;
    if (norrath_legacy_single_inheritance_type_info_vtable_view[5].adjustment != 0x0u ||
        norrath_legacy_single_inheritance_type_info_vtable_view[5].function != nullptr)
        return 13;
    if (norrath_legacy_user_type_info_vtable_view[5].adjustment != 0x0u ||
        norrath_legacy_user_type_info_vtable_view[5].function != nullptr)
        return 14;
    return 0;
}
