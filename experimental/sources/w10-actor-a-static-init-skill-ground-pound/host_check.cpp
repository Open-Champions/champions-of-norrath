// Host contract for _GLOBAL_$I$getRankDescription__16SkillGroundPoundPUsi. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "skill_ground_pound_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{74, 80, 141, 119, 44, 55, 297, 0}, {0x41a80000u, 0x3e4cccccu, 0xc0c00000u}, {3, 1, 9}},
    {{283, 164, 79, 127, 148, 100, 52, 0}, {0x41b00000u, 0x3fa66666u, 0xc0c00000u}, {3, 1, 8}},
    {{87, 58, 31, 191, 9, 9, 9, 0}, {0x41d00000u, 0x425c0000u, 0x3fc00000u}, {3, 0, 6}},
    {{138, 163, 141, 93, 111, 161, 150, 0}, {0x41880000u, 0x3dccccccu, 0xc0dcccccu}, {3, 0, 6}},
    {{255, 0, 0, 255, 255, 0, 0, 255}, {0x42a00000u, 0x41f00000u, 0x00000000u}, {0, 3, 6}},
    {{255, 0, 0, 255, 255, 0, 0, 255}, {0x42a00000u, 0x41f00000u, 0x00000000u}, {1, 3, 6}},
    {{255, 0, 0, 255, 255, 0, 0, 255}, {0x42a00000u, 0x41f00000u, 0x00000000u}, {2, 3, 6}},
};
void* const objects[] = {&dustParticle, &sparkParticle, &smokeParticle, &splashParticle, &groundpoundParticle0, &groundpoundParticle1, &groundpoundParticle2};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$getRankDescription__16SkillGroundPoundPUsi, expected, objects, 7);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
