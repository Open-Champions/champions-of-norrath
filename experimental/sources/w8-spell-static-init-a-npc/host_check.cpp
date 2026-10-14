// Host contract for _GLOBAL_$I$__3NPCG6Point3sPcN33ifPPc. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "npc_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{114, 108, 26, 118, 133, 73, 97, 1}, {0x40c00000u, 0x40a00000u, 0x3d4cccccu}, {3, 1, 9}},
};
void* const objects[] = {&kingParticle};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$__3NPCG6Point3sPcN33ifPPc, expected, objects, 1);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
