// Observed lavaGoopParticle at 0x008d56f8 and lavaSteamParticle at 0x008d5748, both zero-size
// symbols in .bss whose extents to the next symbol are 0x50 (80) bytes each (slideDustParticleBigDark
// precedes, nullp follows 0x50 after the second). Both are passed as the first argument of
// P_AddParticle by the authored vehicle code, so each is a particle definition. The bytes are
// zero in the file and filled at start-up by code outside this package.
// Unknown: the particle definition fields.
extern "C" {
alignas(4) unsigned char lavaGoopParticle[80];
alignas(4) unsigned char lavaSteamParticle[80];
}
