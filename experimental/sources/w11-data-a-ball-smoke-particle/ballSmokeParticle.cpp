// ballSmokeParticle: an 80-byte particle definition (the size the neighbouring ParticleDef objects share).
// The pinned symbol table lists ballSmokeParticle as local BSS symbols at 0x007d6380 and 0x008d6370; every one is followed by the next symbol 80 bytes later. Which of them an importing caller means is not established here; this definition is one global 80-byte object.
// The file image holds zero here; the particle-definition constructor fills it in during
// static initialization. The reviewed -G0 profile places it in BSS. Its fields are not
// established here: callers pass its address to P_AddParticle.
extern "C" {
unsigned char ballSmokeParticle[80];
}
static_assert(sizeof(ballSmokeParticle) == 80u, "ballSmokeParticle extent");
