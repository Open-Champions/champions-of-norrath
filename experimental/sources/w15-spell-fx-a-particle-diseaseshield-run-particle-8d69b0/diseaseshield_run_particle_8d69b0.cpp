// diseaseshield_run_particle_8d69b0: an 80-byte particle definition (the gap to the next symbol in the pinned symbol table).
// The symbol table lists the local BSS symbol diseaseshieldParticle1 (followed by diseaseshieldParticle2) at 0x008d69b0. The name used by importing callers is diseaseshield_run_particle_8d69b0. Which object a
// caller means is taken from that name; this definition is one global 80-byte object.
// The file image holds zero here; the particle-definition constructor fills the object in during static initialization. The
// reviewed -G0 profile places it in BSS. Its fields are not established here: callers pass its address to P_AddParticle.
extern "C" {
unsigned char diseaseshield_run_particle_8d69b0[80];
}
static_assert(sizeof(diseaseshield_run_particle_8d69b0) == 80u, "diseaseshield_run_particle_8d69b0 extent");
