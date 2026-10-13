// Observed storage in the pinned SLUS_205.65 image for the network frame and
// local-input state. Sizes come from the ELF symbol size where nonzero, otherwise
// from the distance to the next symbol (marked gap).
//   localNetFrameNm       0x0048383c  4 bytes, initial 1
//   localNetSentFrameNm   0x00483840  4 bytes, initial 1
//   localPredictedPlayer  0x00483854  4 bytes, initial 0 (a pointer-sized word)
//   logChecksum           0x00483858  4 bytes, initial 0
//   netCameraYaw          0x00486810  gap of 8
//   kickState             0x00486818  gap of 8
//   broadcastConnectionFlags 0x00486820 gap of 4
//   msgBufferSent         0x00486824  gap of 4
//   localPlayerPadState   0x00486828  gap of 8
// All other initial bytes are zero.
// Inferred: netCameraYaw is four halfwords indexed by player (netGetCameraCustomControl
// reads a halfword at twice the player index); the other 8-byte items are byte
// spans, because netInit clears each of them with an 8-byte memset and no
// element type is established. localPredictedPlayer is a pointer (the player
// object lookup returns it as one).
// Unknown: what is stored in logChecksum, and the element types of kickState and
// localPlayerPadState.
// The image keeps the first four in its data sections and the rest in bss; this
// source lets zero-initialised items fall into bss, and section placement is not
// settled here.
extern "C" {
int localNetFrameNm = 1;
int localNetSentFrameNm = 1;
void* localPredictedPlayer;
int logChecksum;
unsigned short netCameraYaw[4];
unsigned char kickState[8];
int broadcastConnectionFlags;
int msgBufferSent;
unsigned char localPlayerPadState[8];
}
