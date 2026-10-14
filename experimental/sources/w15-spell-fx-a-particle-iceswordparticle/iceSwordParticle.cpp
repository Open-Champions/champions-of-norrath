// iceSwordParticle: an 80-byte particle definition (the gap to the next symbol in the pinned symbol table).
// The symbol table lists the local BSS symbol iceSwordParticle (followed by newIceSwordParticle1) at 0x007d6560. The name used by importing callers is iceSwordParticle. Which object a
// caller means is taken from that name; this definition is one global 80-byte object.
// The file image holds zero here; the particle-definition constructor fills the object in during static initialization. The
// reviewed -G0 profile places it in BSS. Its fields are not established here: callers pass its address to P_AddParticle.
extern "C" {
unsigned char iceSwordParticle[80];
}
static_assert(sizeof(iceSwordParticle) == 80u, "iceSwordParticle extent");
