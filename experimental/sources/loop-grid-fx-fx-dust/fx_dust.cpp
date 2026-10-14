// Observed fxDust at 0x42ad70 (.data), 80 bytes up to snowParticle2. The file
// bytes are all zero. It is a particle definition that fxUpdateDust hands to
// P_UpdateParticleType, which fills it in at run time.
extern "C" {
unsigned char fxDust[80];
}
static_assert(sizeof fxDust == 80u, "fxDust extent");
