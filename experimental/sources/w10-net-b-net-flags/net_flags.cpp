// Observed storage in the pinned SLUS_205.65 image. All initial bytes are zero.
//   netTrouble          0x00483834  4 bytes
//   netKickedOut        0x00483838  1 byte
//   totallyDisconnected 0x00483870  1 byte
// netTrouble is also defined by w8-net-play-b-launch-state, which is not a plan
// input; select only one definition. Inferred: byte flags are tested for
// nonzero by the authored callers. Unknown: which routines set netKickedOut.
extern "C" {
int netTrouble;
unsigned char netKickedOut;
unsigned char totallyDisconnected;
}
