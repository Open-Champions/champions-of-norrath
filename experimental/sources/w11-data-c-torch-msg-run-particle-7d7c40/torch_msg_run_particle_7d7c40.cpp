// torch_msg_run_particle_7d7c40: an 80-byte particle definition (the gap to the next symbol in the pinned symbol table).
// The name is an authored caller's placeholder for the local BSS symbol heatParticle at 0x007d7c40, which the pinned symbol table follows with the next symbol 80 bytes later.
// The file image holds zero here; the particle-definition constructor fills the object in during
// static initialization. The reviewed -G0 profile places it in BSS. Its fields are not
// established here: callers pass its address to P_AddParticle.
extern "C" {
unsigned char torch_msg_run_particle_7d7c40[80];
}
static_assert(sizeof(torch_msg_run_particle_7d7c40) == 80u, "torch_msg_run_particle_7d7c40 extent");
