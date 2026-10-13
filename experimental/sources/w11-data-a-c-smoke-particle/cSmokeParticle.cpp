// cSmokeParticle: an 80-byte particle definition (the size the neighbouring ParticleDef objects share).
// The pinned symbol table lists cSmokeParticle as a local BSS symbol at 0x007d61f0, followed by the next symbol 80 bytes later.
// The file image holds zero here; the particle-definition constructor fills it in during
// static initialization. The reviewed -G0 profile places it in BSS. Its fields are not
// established here: callers pass its address to P_AddParticle.
extern "C" {
unsigned char cSmokeParticle[80];
}
static_assert(sizeof(cSmokeParticle) == 80u, "cSmokeParticle extent");
