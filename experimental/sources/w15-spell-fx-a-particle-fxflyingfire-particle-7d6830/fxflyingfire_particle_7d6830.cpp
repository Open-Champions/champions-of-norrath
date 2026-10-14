// fxflyingfire_particle_7d6830: an 80-byte particle definition (the gap to the next symbol in the pinned symbol table).
// The symbol table lists the local BSS symbol sparkParticle (followed by flameParticle) at 0x007d6830. The name used by importing callers is fxflyingfire_particle_7d6830. Which object a
// caller means is taken from that name; this definition is one global 80-byte object.
// The file image holds zero here; the particle-definition constructor fills the object in during static initialization. The
// reviewed -G0 profile places it in BSS. Its fields are not established here: callers pass its address to P_AddParticle.
extern "C" {
unsigned char fxflyingfire_particle_7d6830[80];
}
static_assert(sizeof(fxflyingfire_particle_7d6830) == 80u, "fxflyingfire_particle_7d6830 extent");
