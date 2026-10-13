// smokeParticle: an 80-byte particle definition (the gap to the next symbol in the pinned symbol table).
// The symbol table lists local BSS symbols of this name at 0x007cb690, 0x007d68d0, 0x007d7300, 0x007d7770, 0x007d79a0, 0x007d7f20 and 0x008d6550; each is followed by the next symbol 80 bytes later. Which of them an importing caller means is not established here; this definition is one global 80-byte object.
// The file image holds zero here; the particle-definition constructor fills the object in during
// static initialization. The reviewed -G0 profile places it in BSS. Its fields are not
// established here: callers pass its address to P_AddParticle.
extern "C" {
unsigned char smokeParticle[80];
}
static_assert(sizeof(smokeParticle) == 80u, "smokeParticle extent");
