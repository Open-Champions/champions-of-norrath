// Host contract for _GLOBAL_$I$msg_draw__10FrostShard. Includes the source; not an EE input.
#include "static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{100, 210, 235, 140, 198, 199, 244, 140}, {0x40800000u, 0x00000000u, 0xc0000000u}, {1, 1, 5}},
    {{100, 139, 255, 255, 198, 199, 244, 0}, {0x40e00000u, 0x40a00000u, 0xc104ccccu}, {1, 0, 5}},
    {{255, 255, 255, 255, 150, 150, 150, 0}, {0x40e00000u, 0x40a00000u, 0xc104ccccu}, {1, 1, 5}},
};
void* const objects[] = {&shockoffrostParticle1, &shockoffrostParticle2, &shockoffrostParticle3};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$msg_draw__10FrostShard, expected, objects, 3);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
