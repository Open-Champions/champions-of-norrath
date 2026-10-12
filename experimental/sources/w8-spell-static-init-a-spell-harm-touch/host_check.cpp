// Host contract for _GLOBAL_$I$attack__14SpellHarmTouchP10GameObjectPiT2. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "spell_harm_touch_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{136, 198, 107, 98, 54, 118, 33, 34}, {0x42480000u, 0x3dccccccu, 0x3d4cccccu}, {1, 1, 8}},
    {{163, 127, 92, 135, 59, 7, 3, 58}, {0x41200000u, 0x40000000u, 0x40a00000u}, {2, 1, 6}},
};
void* const objects[] = {&harm1, &harm2};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$attack__14SpellHarmTouchP10GameObjectPiT2, expected, objects, 2);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
