// jumpParticle2: an 80-byte particle definition (the size the neighbouring ParticleDef objects share).
// The pinned symbol table lists jumpParticle2 as a local BSS symbol at 0x008d6090, followed by the next symbol 80 bytes later.
// The file image holds zero here; the particle-definition constructor fills it in during
// static initialization. The reviewed -G0 profile places it in BSS. Its fields are not
// established here: callers pass its address to P_AddParticle.
extern "C" {
unsigned char jumpParticle2[80];
}
static_assert(sizeof(jumpParticle2) == 80u, "jumpParticle2 extent");
