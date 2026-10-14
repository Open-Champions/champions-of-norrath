// Host contract for _GLOBAL_$I$__6SwitchG6Point3PPc. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "switch_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{231, 87, 100, 88, 264, 63, 4, 0}, {0x41f33333u, 0x409eb851u, 0x3f6e147au}, {2, 1, 0}},
    {{42, 87, 100, 88, -11, 171, 328, 0}, {0x41f33333u, 0x409eb851u, 0x3f6e147au}, {2, 1, 0}},
    {{62, 80, 280, 108, 34, 87, 193, 0}, {0x41000000u, 0x41700000u, 0x3e6147aeu}, {3, 1, 6}},
    {{78, 139, 136, 97, 103, 103, 206, 0}, {0x41066666u, 0x41c00000u, 0x40600000u}, {2, 0, 9}},
    {{20, 20, 40, 0, 20, 20, 40, 175}, {0x409ad0e5u, 0x00000000u, 0x00000000u}, {1, 2, 4}},
    {{50, 50, 100, 100, 170, 200, 250, 0}, {0x3f800000u, 0x41200000u, 0x00000000u}, {1, 1, 7}},
    {{200, 200, 200, 0, 200, 200, 200, 80}, {0x40a00000u, 0x3fccccccu, 0x00000000u}, {2, 1, 6}},
    {{2, 83, 74, 224, 31, 43, 31, 214}, {0x42a4570au, 0x42956d91u, 0x00000000u}, {3, 3, 2}},
    {{72, 51, 6, 25, 43, 36, 32, 5}, {0x404ae147u, 0x42916d91u, 0x00000000u}, {3, 0, 6}},
    {{255, 255, 255, 255, 0, 0, 0, 0}, {0x41d00000u, 0x41200000u, 0xc1700000u}, {2, 1, 0}},
    {{255, 240, 119, 128, 0, 0, 0, -159}, {0x420d5604u, 0x3fcb851eu, 0x00000000u}, {1, 1, 8}},
    {{72, 31, 19, 128, 0, 0, 0, 0}, {0x41efced9u, 0x426553f7u, 0x00000000u}, {1, 1, 8}},
    {{5, 128, 128, 402, 128, 128, 128, -68}, {0x42c80000u, 0x42c80000u, 0x00000000u}, {2, 3, 0}},
    {{250, 126, 86, 255, 0, 0, 0, 0}, {0x42700000u, 0x41200000u, 0xc1700000u}, {1, 1, 9}},
    {{90, 50, 40, 70, 2, 5, 5, 0}, {0x422c0000u, 0x42c80000u, 0x40000000u}, {1, 0, 6}},
    {{100, 210, 238, 90, 4, 63, 105, 0}, {0x41300000u, 0x42480000u, 0x00000000u}, {1, 1, 4}},
    {{255, 255, 255, 255, 0, 0, 0, 0}, {0x41000000u, 0x40400000u, 0xc0a00000u}, {2, 1, 0}},
    {{100, 44, 0, 23, -77, -54, 0, 0}, {0x41d00000u, 0x41a80000u, 0xc07851ebu}, {2, 0, 5}},
    {{96, 210, 103, 85, 70, 242, 74, 0}, {0x3f424dd2u, 0x41e478d4u, 0x00000000u}, {1, 1, 15}},
    {{254, 30, 27, 128, 250, 35, 34, 0}, {0x42187ef9u, 0x3fa16872u, 0x00000000u}, {2, 1, 9}},
    {{30, 254, 27, 128, 35, 250, 34, 0}, {0x41a0fdf3u, 0x3fa16872u, 0x00000000u}, {2, 1, 9}},
    {{27, 30, 254, 128, 34, 35, 250, 0}, {0x42187ef9u, 0x3fa16872u, 0x00000000u}, {2, 1, 9}},
    {{250, 100, 250, 103, 255, 103, 238, 32}, {0x42798624u, 0x42158624u, 0x4002e147u}, {1, 2, 6}},
};
void* const objects[] = {&fireTrailParticle, &trackfireTrailParticle, &saveParticle, &saveParticle2, &rainParticle, &rainSplashParticle, &snowParticle, &teleportParticle1, &teleportParticle2, &sparkParticle, &trailParticle, &fireSmokeParticle, &heatParticle, &fireHitParticle, &fireSmoke, &iceBallParticle1, &fuseParticle, &smokeParticle, &rootParticle, &oniRedParticle, &oniGreenParticle, &oniBlueParticle, &unholyAuraParticle};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$__6SwitchG6Point3PPc, expected, objects, 23);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
