// multifire_msg_run_particle_8d7040: an 80-byte particle definition (the gap to the next symbol in the pinned symbol table).
// The symbol table lists the local BSS symbol trail (one of two at 0x008d7040 and 0x008d7090; followed by the other) at 0x008d7040. The name used by importing callers is multifire_msg_run_particle_8d7040. Which object a
// caller means is taken from that name; this definition is one global 80-byte object.
// The file image holds zero here; the particle-definition constructor fills the object in during static initialization. The
// reviewed -G0 profile places it in BSS. Its fields are not established here: callers pass its address to P_AddParticle.
extern "C" {
unsigned char multifire_msg_run_particle_8d7040[80];
}
static_assert(sizeof(multifire_msg_run_particle_8d7040) == 80u, "multifire_msg_run_particle_8d7040 extent");
