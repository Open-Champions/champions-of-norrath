// Host contract for _GLOBAL_$I$__5TorchG6Point3PPci. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "torch_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{132, 83, 45, 59, 108, 46, 13, 94}, {0x42480000u, 0x40a00000u, 0x3fa66666u}, {2, 1, 9}},
    {{90, 74, 60, 48, 60, 50, 45, 0}, {0x41f00000u, 0x42700000u, 0x40000000u}, {2, 0, 6}},
    {{5, 128, 128, 402, 128, 128, 128, -68}, {0x42180000u, 0x42080000u, 0x00000000u}, {1, 3, 0}},
    {{402, 295, 391, 485, 126, 58, 27, 0}, {0x40199999u, 0x41e13333u, 0xc1680000u}, {1, 2, 0}},
    {{128, 96, 26, 152, 119, 55, 1, 47}, {0x40266666u, 0x3ef0a3d7u, 0x3e0f5c28u}, {1, 1, 4}},
    {{128, 96, 26, 152, 119, 55, 1, 47}, {0x40666666u, 0x3ef0a3d7u, 0x3e0f5c28u}, {1, 1, 4}},
    {{164, 120, 10, 790, 154, 55, 47, 0}, {0x41ccc8b4u, 0x3e3020c4u, 0x00000000u}, {2, 1, 9}},
};
void* const objects[] = {&torchParticle, &torchSmokeParticle, &heatParticle, &fireSparkParticle, &candleParticle, &candle2Particle, &lampParticle};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$__5TorchG6Point3PPci, expected, objects, 7);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
