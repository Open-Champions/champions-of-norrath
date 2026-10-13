// vaporParticle: an 80-byte particle definition (the gap to the next symbol in the pinned symbol table).
// The symbol table lists the local BSS symbol vaporParticle at 0x007d67e0; each is followed by the next symbol 80 bytes later. Which of them an importing caller means is not established here; this definition is one global 80-byte object.
// The file image holds zero here; the particle-definition constructor fills the object in during
// static initialization. The reviewed -G0 profile places it in BSS. Its fields are not
// established here: callers pass its address to P_AddParticle.
extern "C" {
unsigned char vaporParticle[80];
}
static_assert(sizeof(vaporParticle) == 80u, "vaporParticle extent");
