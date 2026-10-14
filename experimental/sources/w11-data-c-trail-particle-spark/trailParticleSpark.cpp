// trailParticleSpark: an 80-byte particle definition (the gap to the next symbol in the pinned symbol table).
// The symbol table lists local BSS symbols of this name at 0x004e2080, 0x00600c80, 0x007cb550 and 0x007d0b20; each is followed by the next symbol 80 bytes later. Which of them an importing caller means is not established here; this definition is one global 80-byte object.
// The file image holds zero here; the particle-definition constructor fills the object in during
// static initialization. The reviewed -G0 profile places it in BSS. Its fields are not
// established here: callers pass its address to P_AddParticle.
extern "C" {
unsigned char trailParticleSpark[80];
}
static_assert(sizeof(trailParticleSpark) == 80u, "trailParticleSpark extent");
