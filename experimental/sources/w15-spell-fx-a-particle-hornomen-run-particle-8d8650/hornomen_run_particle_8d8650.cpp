// hornomen_run_particle_8d8650: an 80-byte particle definition (the gap to the next symbol in the pinned symbol table).
// The symbol table lists the local BSS symbol hornomenParticle1 (followed by vik) at 0x008d8650. The name used by importing callers is hornomen_run_particle_8d8650. Which object a
// caller means is taken from that name; this definition is one global 80-byte object.
// The file image holds zero here; the particle-definition constructor fills the object in during static initialization. The
// reviewed -G0 profile places it in BSS. Its fields are not established here: callers pass its address to P_AddParticle.
extern "C" {
unsigned char hornomen_run_particle_8d8650[80];
}
static_assert(sizeof(hornomen_run_particle_8d8650) == 80u, "hornomen_run_particle_8d8650 extent");
