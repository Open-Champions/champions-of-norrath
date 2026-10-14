// Observed: MC_SpawnPoint at 0x0042e2a0 is 48 bytes, 48 bytes of zero in the executable
// image. The defined object here has that extent and is zero-initialised.
// Inferred: the savegame-load code copies 12-byte position triples into it (four of them).
// Unknown: whether the triples are floats and which player each belongs to.

extern "C" {
unsigned char MC_SpawnPoint[48];
}
