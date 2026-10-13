// Observed storage in the pinned SLUS_205.65 image. Initial bytes are zero.
//   timeout    0x00486650  4 bytes (D)
//   readcount  0x00486668  4 bytes (D)
//   writecount 0x0048666c  4 bytes (D)
// Inferred: signed 32-bit values (types follow the authored comm code that
// reads them). Unknown: units and meaning beyond the symbol names.
extern "C" {
int timeout;
int readcount;
int writecount;
}
