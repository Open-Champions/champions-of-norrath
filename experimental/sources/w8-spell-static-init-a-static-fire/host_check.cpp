// Host contract for _GLOBAL_$I$__10StaticFireG6Point3fif. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "static_fire_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{300, 200, 48, 88, 244, 56, 4, 26}, {0x41d00000u, 0x41300000u, 0x40133333u}, {1, 1, 0}},
    {{136, 131, 92, 136, 391, 46, 67, 0}, {0x41c80000u, 0x40a00000u, 0x3f000000u}, {1, 1, 9}},
    {{375, 267, 92, 128, 55, 55, 4, 0}, {0x40a00000u, 0x41000000u, 0xbebd70a3u}, {2, 2, 0}},
};
void* const objects[] = {&fireParticle, &fireParticleCut, &sparkParticle};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$__10StaticFireG6Point3fif, expected, objects, 3);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
