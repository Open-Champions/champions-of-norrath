// Observed session storage in the pinned SLUS_205.65 image. Sizes come from the
// ELF symbol size where it is nonzero, and otherwise from the distance to the
// next symbol (marked gap). Initial bytes are all zero in the file, so every
// object here is plain zero-initialized storage. No field layout is claimed.
//   playersAlive      0x0042a3e8  16 bytes
//   startEntranceName 0x0042a3f8  32 bytes
//   syncRandState     0x00483e5c  4 bytes
//   activePlayer      0x0048453c  4 bytes (read as a signed index)
//   nmPlayersAlive    0x00484f44  4 bytes
//   noMonsters        0x00486840  gap of 8; every reader loads one signed byte
//   nmSingle          0x004869f8  gap of 4
//   nmLines           0x004869fc  gap of 4
//   invisible         0x006f5a80  gap of 80
extern "C" {
unsigned char playersAlive[16];
unsigned char startEntranceName[32];
unsigned syncRandState;
int activePlayer;
int nmPlayersAlive;
signed char noMonsters;
int nmSingle;
int nmLines;
unsigned char invisible[80];
}
