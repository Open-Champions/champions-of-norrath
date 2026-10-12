// Host contract for _GLOBAL_$I$__11DiseaseBoltsiG6Point3P10GameObjectP5Skill. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "disease_bolts_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{110, 100, 23, 241, 8, 40, 16, 5}, {0x420c0000u, 0x41800000u, 0x00000000u}, {0, 0, 6}},
    {{78, 80, 30, 50, 78, 66, 30, 5}, {0x41e00000u, 0x42480000u, 0xc0000000u}, {2, 0, 9}},
    {{0, 0, 0, 72, 36, 25, 20, 125}, {0x41400000u, 0x3f800000u, 0x00000000u}, {2, 0, 9}},
};
void* const objects[] = {&diseaseParticle1, &diseaseParticle2, &diseaseParticle3};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$__11DiseaseBoltsiG6Point3P10GameObjectP5Skill, expected, objects, 3);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
