// Host contract for _GLOBAL_$I$msg_run__12DiseaseTrail. Includes the source; not an EE input.
#include "disease_trail_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{110, 100, 23, 100, 78, 66, 30, 5}, {0x420c0000u, 0x42340000u, 0x00000000u}, {2, 0, 2}},
    {{12, 12, 0, 78, 8, 40, 16, 5}, {0x41a00000u, 0x41700000u, 0x00000000u}, {3, 0, 8}},
    {{0, 0, 0, 254, 25, 25, 25, 0}, {0x40a00000u, 0x40a00000u, 0x00000000u}, {2, 0, 9}},
};
void* const objects[] = {&diseaseTrailParticle1, &diseaseTrailParticle2, &diseaseTrailParticle3};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$msg_run__12DiseaseTrail, expected, objects, 3);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
