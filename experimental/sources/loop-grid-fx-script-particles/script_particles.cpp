// Observed scriptParticles at 0x333920 (.data): three pointers followed by a
// zero word, 16 bytes up to the next symbol nativeList. The pointers are the
// addresses of scriptParticle0 (0x4f3f20), scriptParticle1 (0x4f3f70) and
// scriptParticle2 (0x4f3fc0), each 80 bytes of BSS (the size of a particle
// definition as copied by P_UpdateParticleType). Their contents start as
// zero; the definitions are written at run time.
extern "C" {
unsigned char scriptParticle0[80];
unsigned char scriptParticle1[80];
unsigned char scriptParticle2[80];
unsigned char* scriptParticles[4] = {scriptParticle0, scriptParticle1, scriptParticle2, nullptr};
}
