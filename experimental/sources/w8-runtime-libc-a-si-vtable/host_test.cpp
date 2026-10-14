// Host comparison only. Not an EE build input. Checks slot order, zero
// adjustments and the zero final pair of the observed table.
#include <cstdio>

#include "si_vtable.cpp"

extern "C" {
void __tf14__si_type_info() {}
void _$_14__si_type_info() {}
void do_upcast__C14__si_type_infoQ216__user_type_info8sub_kindRC9type_infoPvRQ216__user_type_info13upcast_result() {}
void do_dyncast__C14__si_type_infoiQ216__user_type_info8sub_kindRC9type_infoPvT3T4RQ216__user_type_info14dyncast_result() {}
void do_find_public_subobj__C14__si_type_infoiRC9type_infoPvT3() {}
}

int main() {
    const NorrathLegacyTypeInfoSlot* view = norrath_legacy_single_inheritance_type_info_vtable_view;
    void (*expected[5])() = {
        __tf14__si_type_info,
        _$_14__si_type_info,
        do_upcast__C14__si_type_infoQ216__user_type_info8sub_kindRC9type_infoPvRQ216__user_type_info13upcast_result,
        do_dyncast__C14__si_type_infoiQ216__user_type_info8sub_kindRC9type_infoPvT3T4RQ216__user_type_info14dyncast_result,
        do_find_public_subobj__C14__si_type_infoiRC9type_infoPvT3,
    };
    for (int i = 0; i < 5; ++i) {
        if (view[i].adjustment != 0 || view[i].function != expected[i]) { std::printf("slot %d wrong\n", i); return 1; }
    }
    if (view[5].adjustment != 0 || view[5].function != nullptr) { std::printf("final pair wrong\n"); return 2; }
    if (sizeof(norrath_legacy_single_inheritance_type_info_vtable_view) / sizeof(view[0]) != 6) { std::printf("slot count wrong\n"); return 3; }
    std::printf("si type_info vtable cases passed\n");
    return 0;
}
