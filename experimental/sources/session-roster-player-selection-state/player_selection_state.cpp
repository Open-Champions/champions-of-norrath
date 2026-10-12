// Observed player-selection storage. Both are zero in the file.
//   playerSelRunning       0x00485ec4  4 bytes (file-local in the image; global here)
//   playerSelectSoundLump  0x008d5a98  32 bytes (gap to the next symbol); the
//                          selection start copies one of three 11-byte lump
//                          names into it.
extern "C" {
int playerSelRunning;
char playerSelectSoundLump[32];
}
