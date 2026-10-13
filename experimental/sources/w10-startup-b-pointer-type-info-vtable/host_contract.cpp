// Host check. Not an EE build input. Checks slot order, zero adjustments and the zero
// final pair.
#include <cstdio>
#include "pointer_type_info_vtable.cpp"
extern "C" {
void __tf19__pointer_type_info() {}
void _$_19__pointer_type_info() {}
}
int main() {
    const NorrathLegacyPointerTypeInfoSlot* view = norrath_legacy_pointer_type_info_vtable_view;
    if (view[0].adjustment != 0 || view[0].function != __tf19__pointer_type_info) { std::puts("slot 0 wrong"); return 1; }
    if (view[1].adjustment != 0 || view[1].function != _$_19__pointer_type_info) { std::puts("slot 1 wrong"); return 1; }
    if (view[2].adjustment != 0 || view[2].function != nullptr) { std::puts("final pair wrong"); return 1; }
    if (sizeof(norrath_legacy_pointer_type_info_vtable_view) / sizeof(view[0]) != 3) { std::puts("slot count wrong"); return 1; }
    std::puts("pointer type_info vtable cases passed");
    return 0;
}
