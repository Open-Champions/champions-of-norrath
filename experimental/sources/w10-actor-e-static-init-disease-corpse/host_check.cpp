// Host contract for _GLOBAL_$I$msg_run__13DiseaseCorpse. Includes the source; not an EE input.
#include "disease_corpse_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{98, 121, 66, 50, 63, 85, 30, 0}, {0x41700000u, 0x41a00000u, 0xc0000000u}, {2, 0, 2}},
    {{125, 179, 15, 50, 65, 116, 0, 0}, {0x41700000u, 0x41a00000u, 0xc0000000u}, {2, 0, 2}},
    {{0, 0, 0, 54, 25, 25, 25, 0}, {0x41200000u, 0x41200000u, 0x00000000u}, {2, 0, 9}},
};
void* const objects[] = {&diseaseCorpseParticle1, &diseaseCorpseParticle2, &diseaseCorpseParticle3};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$msg_run__13DiseaseCorpse, expected, objects, 3);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
