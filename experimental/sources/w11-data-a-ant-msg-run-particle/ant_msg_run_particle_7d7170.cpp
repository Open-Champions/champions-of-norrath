// ant_msg_run_particle_7d7170 is the importing 3Ant msg_run source's own label (not a symbol of the
// image) for the 80-byte particle definition at 0x007d7170, which the symbol table lists as the local
// BSS symbol ectopileParticle1 (80 bytes to the next symbol). The file image holds zero; the
// particle-definition constructor fills it during static initialization.
extern "C" {
unsigned char ant_msg_run_particle_7d7170[80];
}
static_assert(sizeof(ant_msg_run_particle_7d7170) == 80u, "particle definition extent");
