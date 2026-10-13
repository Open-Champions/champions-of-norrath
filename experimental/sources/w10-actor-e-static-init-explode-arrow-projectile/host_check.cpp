// Host contract for _GLOBAL_$I$msg_run__22ExplodeArrowProjectile. Includes the source; not an EE input.
#include "explode_arrow_projectile_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{193, 112, 39, 61, 54, 51, 33, 14}, {0x41800000u, 0x3e999999u, 0xbeccccccu}, {1, 2, 2}},
};
void* const objects[] = {&explodeArrowParticle};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$msg_run__22ExplodeArrowProjectile, expected, objects, 1);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
