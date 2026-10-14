// hitParticleDust2: an 80-byte particle definition (the gap to the next symbol in the pinned symbol table).
// The symbol table lists the local BSS symbol hitParticleDust2; two symbols carry this name, at 0x007d69c0 and 0x007d70c0, and the import name without a suffix does not say which one at 0x007d70c0. The name used by importing callers is hitParticleDust2. Which object a
// caller means is taken from that name; this definition is one global 80-byte object.
// The file image holds zero here; the particle-definition constructor fills the object in during static initialization. The
// reviewed -G0 profile places it in BSS. Its fields are not established here: callers pass its address to P_AddParticle.
extern "C" {
unsigned char hitParticleDust2[80];
}
static_assert(sizeof(hitParticleDust2) == 80u, "hitParticleDust2 extent");
