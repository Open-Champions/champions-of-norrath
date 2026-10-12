// Host contract for _GLOBAL_$I$__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "cold_arrow_projectile_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{65, 65, 144, 64, 28, 39, 49, 0}, {0x41b80000u, 0x42906666u, 0xc0b00000u}, {1, 1, 6}},
    {{77, 89, 115, 129, 88, 74, 115, 128}, {0x40c00000u, 0x400cccccu, 0xc0a33333u}, {3, 0, 5}},
    {{111, 119, 215, 128, 76, 86, 210, 50}, {0x40a00000u, 0x3d4cccccu, 0xbdccccccu}, {3, 0, 4}},
};
void* const objects[] = {&coldSmoke, &coldSpeck, &pack};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill, expected, objects, 3);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
