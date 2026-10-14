// Observed storage in the pinned SLUS_205.65 image for the network launch path.
// Sizes come from the ELF symbol size where nonzero, otherwise from the distance
// to the next symbol (marked gap). Every initial byte is zero except
// netBroadCastStart and the two frame words in the frame-state unit.
//   netBroadCastStart     0x0048382a  1 byte, initial 1
//   netAttemptLaunch      0x0048382b  1 byte, initial 0
//   isModem               0x00483850  1 byte, initial 0
//   lastPingTime          0x00483884  4 bytes, initial 0
//   compressSize          0x004839f0  4 bytes, initial 0
//   lnet_buf              0x004839e8  4 bytes, initial 0 (a pointer-sized word)
//   launchPeers           0x00333278  1008 bytes, zero; 4 entries of 252 bytes
//                         are indexed by the launch code
//   hostForcedLaunch      0x0048680a  gap of 1
//   hostLaunchMask        0x0048680b  gap of 1
//   hostStartedWithoutMe  0x0048680c  gap of 4 (alignment padding follows)
//   myaddr                0x004f3e80  gap of 16 (passed with length 16 to
//                         sceInetInterfaceControl)
// Inferred: lastPingTime is a float (it is compared with a difference of
// currentTime values); compressSize is a signed count (the launch code stores -1);
// lnet_buf holds a pointer or handle passed to the inet calls as an opaque value.
// Unknown: the element layout of launchPeers beyond the offsets the launch
// routines use; myaddr's address format.
// The image places the nonzero-initialised and several zero-initialised items
// in its data sections (the image symbols are data symbols, not bss), while this
// source lets zero-initialised items fall into bss; section placement is a link
// layout question that this file does not settle.
// netBroadCastStart and netAttemptLaunch are also defined by
// w8-net-play-b-launch-state/launch_state.cpp, which is not a plan input; only
// one of the two may be selected.
extern "C" {
signed char netBroadCastStart = 1;
signed char netAttemptLaunch;
signed char isModem;
float lastPingTime;
int compressSize;
void* lnet_buf;
unsigned char launchPeers[1008];
signed char hostForcedLaunch;
signed char hostLaunchMask;
signed char hostStartedWithoutMe;
unsigned char myaddr[16];
}
