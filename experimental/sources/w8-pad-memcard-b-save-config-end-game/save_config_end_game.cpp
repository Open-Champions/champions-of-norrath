// Observed: saveConfigEndGame is one byte at 0x00486cc7, in zero-filled storage
// between keepPlayerPositions (0x00486cc6) and killPlayer (0x00486cc8). The
// authored silent config save stores a byte into it.
// Inferred: none.
// Unknown: who reads it.
extern "C" unsigned char saveConfigEndGame = 0;
