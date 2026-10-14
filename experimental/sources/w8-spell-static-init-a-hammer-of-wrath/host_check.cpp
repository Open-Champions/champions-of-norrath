// Host contract for _GLOBAL_$I$__13HammerOfWrathG6Point3sP10GameObjectiP5Skill. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "hammer_of_wrath_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{114, 127, 139, 75, 31, 43, 66, 0}, {0x40c33333u, 0x41000000u, 0x4124ccccu}, {1, 1, 6}},
    {{255, 200, 170, 128, 250, 225, 135, 0}, {0x42180000u, 0x41a00000u, 0x3fb33333u}, {1, 2, 15}},
    {{86, 126, 250, 134, 4, 63, 105, 0}, {0x41a00000u, 0x00000000u, 0x3fb33333u}, {2, 2, 15}},
};
void* const objects[] = {&hammerofwrathParticle1, &hammerofwrathParticle2, &hammerofwrathParticle3};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$__13HammerOfWrathG6Point3sP10GameObjectiP5Skill, expected, objects, 3);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
