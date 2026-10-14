// planarsteel_run_particle_8d8510: 80-byte particle definition record, one of the 22 record storages of the w14-m-data-b group.
// Observed: w12-actor-spell-c-run-22spellplanarsteeleffect/run.cpp pass its address to P_AddParticle__FP11ParticleDefP6Point3T1f.
// Inferred: 80 bytes and 16-byte alignment, from the particle-definition constructor record contract in
// w8-spell-static-init-a-common/particle_def.hpp and the store range of setParticleDef (byte offsets 0 to 79).
// Unknown: the field values. This definition is zeroed storage; the static initializer that owns the record
// fills it in another package. Nothing here establishes the contents.
extern "C" {
alignas(16) unsigned char planarsteel_run_particle_8d8510[80];
}
static_assert(sizeof planarsteel_run_particle_8d8510 == 80u, "planarsteel_run_particle_8d8510 extent");
