// norrath_w10_actor_n_view_8d6460: an 80-byte particle definition (the gap to the next symbol in the pinned symbol table).
// The name is an authored caller's placeholder for the local BSS symbol sparkParticle at 0x008d6460, which the pinned symbol table follows with the next symbol 80 bytes later.
// The file image holds zero here; the particle-definition constructor fills the object in during
// static initialization. The reviewed -G0 profile places it in BSS. Its fields are not
// established here: callers pass its address to P_AddParticle.
extern "C" {
unsigned char norrath_w10_actor_n_view_8d6460[80];
}
static_assert(sizeof(norrath_w10_actor_n_view_8d6460) == 80u, "norrath_w10_actor_n_view_8d6460 extent");
