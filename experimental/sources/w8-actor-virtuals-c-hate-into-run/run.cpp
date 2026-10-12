#include "../w8-actor-virtuals-c-hate-run-common/hate_run.hpp"
// Observed msg_run__8HateInto at 0x225910, 472 bytes. See hate_run.hpp for the
// sequence: frame mask 1, index step 2, and the particle definition stored at
// 0x7d7a40 (the image names it FireParticle; the same 80-byte object is
// constructed by the AntTrap static initializer and is imported here under the
// name that unit's source gives it).
extern "C" unsigned char norrath_anttrap_msg_run_FireParticle_bytes[80];

extern "C" void msg_run__8HateInto(void* self) {
    w8c::hate_emit_particles(self, 1u, 2, norrath_anttrap_msg_run_FireParticle_bytes);
}
