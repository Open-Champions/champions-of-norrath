// Observed storage in the pinned SLUS_205.65 image. Sizes come from the ELF
// symbol size where nonzero, otherwise from the distance to the next symbol
// (marked gap). All initial bytes are zero except netBroadCastStart.
//   netBroadCastStart  0x0048382a  1 byte, initial 1
//   netAttemptLaunch   0x0048382b  1 byte, initial 0
//   netTrouble         0x00483834  4 bytes, initial 0
//   multiHelpMessageOn 0x00483f38  4 bytes, initial 0
//   localNmPlayersAlive 0x00484f48 4 bytes, initial 0
//   nmPlayersAlive     0x00484f44  4 bytes, initial 0 (read as a signed count)
//   nmBatchCommands    0x00485954  4 bytes, initial 0 (a local symbol in the image)
//   multiRelease       0x00486ae0  gap of 4
//   multiTitle         0x007d06e8  gap of 16
//   multiBody          0x007d06f8  gap of 16
//   multiSide          0x007d0708  gap of 16
// Inferred: none. The element types of the multi* objects are not established
// here; they are byte spans only.
// Unknown: which routines read each value, beyond the names the image gives.
// nmPlayersAlive is also defined by session-roster-player-state/player_state.cpp,
// which is not a plan input because it duplicates other packages' names; only
// one of the two may be selected.
extern "C" {
signed char netBroadCastStart = 1;
signed char netAttemptLaunch;
int netTrouble;
int multiHelpMessageOn;
int nmPlayersAlive;
int localNmPlayersAlive;
int nmBatchCommands;
unsigned char multiRelease[4];
unsigned char multiTitle[16];
unsigned char multiBody[16];
unsigned char multiSide[16];
}
