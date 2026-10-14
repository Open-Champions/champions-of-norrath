// redSpark: 80-byte particle definition record, one of the 22 record storages of the w14-m-data-b group.
// Observed: w12-hud-loop-b-player-block-sparks/player_block_sparks.cpp pass its address to P_AddParticle__FP11ParticleDefP6Point3T1f.
// Inferred: 80 bytes and 16-byte alignment, from the particle-definition constructor record contract in
// w8-spell-static-init-a-common/particle_def.hpp and the store range of setParticleDef (byte offsets 0 to 79).
// Unknown: the field values. This definition is zeroed storage; the static initializer that owns the record
// fills it in another package. Nothing here establishes the contents.
// Note: w8-net-play-a-global-init-nm-players/global_init.cpp declares a file-local record of the same name and constructs that copy, so this global is not the one it fills.
extern "C" {
alignas(16) unsigned char redSpark[80];
}
static_assert(sizeof redSpark == 80u, "redSpark extent");
