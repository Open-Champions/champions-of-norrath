// norrath_w10_actor_n_view_8d63c0: an 80-byte particle definition (the gap to the next symbol in the pinned symbol table).
// The name is an authored caller's placeholder for the local BSS symbol fireParticle at 0x008d63c0, which the pinned symbol table follows with the next symbol 80 bytes later.
// The file image holds zero here; the particle-definition constructor fills the object in during
// static initialization. The reviewed -G0 profile places it in BSS. Its fields are not
// established here: callers pass its address to P_AddParticle.
extern "C" {
unsigned char norrath_w10_actor_n_view_8d63c0[80];
}
static_assert(sizeof(norrath_w10_actor_n_view_8d63c0) == 80u, "norrath_w10_actor_n_view_8d63c0 extent");
