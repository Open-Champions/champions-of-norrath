// dustcloud_msg_run_particle_7d69c0: an 80-byte particle definition (the gap to the next symbol in the pinned symbol table).
// The symbol table lists the local BSS symbol hitParticleDust2 (one of two symbols with that name; followed by froststormParticle1) at 0x007d69c0. The name used by importing callers is dustcloud_msg_run_particle_7d69c0. Which object a
// caller means is taken from that name; this definition is one global 80-byte object.
// The file image holds zero here; the particle-definition constructor fills the object in during static initialization. The
// reviewed -G0 profile places it in BSS. Its fields are not established here: callers pass its address to P_AddParticle.
extern "C" {
unsigned char dustcloud_msg_run_particle_7d69c0[80];
}
static_assert(sizeof(dustcloud_msg_run_particle_7d69c0) == 80u, "dustcloud_msg_run_particle_7d69c0 extent");
