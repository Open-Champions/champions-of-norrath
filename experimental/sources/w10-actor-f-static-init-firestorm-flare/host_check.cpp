// Host contract for _GLOBAL_$I$msg_run__14FireStormFlare. Includes the source; not an EE input.
#include "static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{295, 195, 111, 413, 226, 11, 14, -11}, {0x40800000u, 0x40c00000u, 0x00000000u}, {2, 1, 0}},
    {{130, 116, 76, 87, 194, 120, 74, 0}, {0x42000000u, 0x40a00000u, 0x00000000u}, {1, 1, 2}},
    {{0, 0, 0, 64, 34, 30, 21, 0}, {0x41f00000u, 0x42480000u, 0x40400000u}, {1, 0, 6}},
};
void* const objects[] = {&firestormParticle1, &firestormParticle2, &firestormParticle3};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$msg_run__14FireStormFlare, expected, objects, 3);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
