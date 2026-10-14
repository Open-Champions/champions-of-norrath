// oniball_msg_run_particle_7d78b0: 80-byte particle definition record, one of the 22 record storages of the w14-m-data-b group.
// Observed: w12-actor-spell-c-msg-run-7oniball/msg_run.cpp pass its address to P_AddParticle__FP11ParticleDefP6Point3T1f.
// Inferred: 80 bytes and 16-byte alignment, from the particle-definition constructor record contract in
// w8-spell-static-init-a-common/particle_def.hpp and the store range of setParticleDef (byte offsets 0 to 79).
// Unknown: the field values. This definition is zeroed storage; the static initializer that owns the record
// fills it in another package. Nothing here establishes the contents.
extern "C" {
alignas(16) unsigned char oniball_msg_run_particle_7d78b0[80];
}
static_assert(sizeof oniball_msg_run_particle_7d78b0 == 80u, "oniball_msg_run_particle_7d78b0 extent");
