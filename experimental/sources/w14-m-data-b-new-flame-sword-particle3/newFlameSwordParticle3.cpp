// newFlameSwordParticle3: 80-byte particle definition record, one of the 22 record storages of the w14-m-data-b group.
// Observed: w11-rend-a-fx-run-weapon/fx_run_weapon.cpp pass its address to P_AddParticle__FP11ParticleDefP6Point3T1f.
// Inferred: 80 bytes and 16-byte alignment, from the particle-definition constructor record contract in
// w8-spell-static-init-a-common/particle_def.hpp and the store range of setParticleDef (byte offsets 0 to 79).
// Unknown: the field values. This definition is zeroed storage; the static initializer that owns the record
// fills it in another package. Nothing here establishes the contents.
// Note: w8-fx-particles-init-fx/static_init.cpp declares a file-local record of the same name and constructs that copy, so this global is not the one it fills.
extern "C" {
alignas(16) unsigned char newFlameSwordParticle3[80];
}
static_assert(sizeof newFlameSwordParticle3 == 80u, "newFlameSwordParticle3 extent");
