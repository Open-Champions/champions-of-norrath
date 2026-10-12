// Host contract for _GLOBAL_$I$__8FireBombG6Point3sPPc. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "fire_bomb_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{295, 195, 111, 149, 226, 11, 14, 24}, {0x40800000u, 0x40c00000u, 0xc0533333u}, {2, 1, 0}},
};
void* const objects[] = {&sparkParticle};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$__8FireBombG6Point3sPPc, expected, objects, 1);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
