// Host contract for _GLOBAL_$I$getRankDescription__9SkillBashPUsi. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "skill_bash_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{190, 174, 91, 101, 2, 6, 38, 67}, {0xc1200000u, 0x41400000u, 0x00000000u}, {0, 2, 8}},
};
void* const objects[] = {&effectParticle};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$getRankDescription__9SkillBashPUsi, expected, objects, 1);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
