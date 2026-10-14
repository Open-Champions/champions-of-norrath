// Observed storage used by the change-players state machine and the
// memory-card change-players call in the pinned SLUS_205.65 image. Every
// object is zero in the file. Sizes come from the distance to the next symbol
// and from access widths in the authored callers.
//   pcNmPlayers         0x00486cb8  1 byte, signed byte accesses
//   pcLaunch            0x00486cb9  1 byte, signed byte accesses
//   pcTitle             0x00486ccc  4 bytes, a pointer to an
//                                   unsigned-short string (copied with ustrcpy)
//   playerChangeOldYaw  0x00486ca8  8 bytes, four unsigned halfwords
//   playerChangeOldPos  0x007e05b0  48 bytes, four 12-byte positions
//   playerStatus        0x008d1540  16 bytes, four words
//   playerRace          0x008d1550  16 bytes, four words (a different object
//                                   from the initialized local playerRace.1193
//                                   at 0x00444818)
// Inferred: one entry per player slot (four), from the sizes. Unknown: the
// value ranges; pcNmPlayers/pcLaunch/pcTitle are the players-and-launch request
// and heading text of the change-players call only by their use there.
extern "C" {
signed char pcNmPlayers;
signed char pcLaunch;
unsigned short* pcTitle;
unsigned short playerChangeOldYaw[4];
float playerChangeOldPos[4][3];
int playerStatus[4];
int playerRace[4];
}
