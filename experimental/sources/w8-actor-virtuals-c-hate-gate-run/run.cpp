#include "../w8-actor-virtuals-c-hate-run-common/hate_run.hpp"
// Observed msg_run__8HateGate at 0x225ae8, 472 bytes. See hate_run.hpp for the
// sequence: frame mask 7, index step 8, and the particle definition stored at
// 0x7d7a90 (the image names it HateGateParticle; the same 80-byte object is
// constructed by the AntTrap static initializer and is imported here under the
// name that unit's source gives it).
extern "C" unsigned char norrath_anttrap_msg_run_HateGateParticle_bytes[80];

extern "C" void msg_run__8HateGate(void* self) {
    w8c::hate_emit_particles(self, 7u, 8, norrath_anttrap_msg_run_HateGateParticle_bytes);
}
