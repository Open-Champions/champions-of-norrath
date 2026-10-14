// Storage for four single-byte net flags, with addresses from the private image's symbol table
// (SLUS_205.65). The first three are consecutive, preceded by netBroadCastStart/netAttemptLaunch and
// followed by netForceLocalUpdate, which are not part of this unit.
//   netTrueStep                0x0048382c  1 byte, initial 0 (global .data in the image)
//   netPrediction              0x0048382d  1 byte, initial 0
//   netPredictionFastForward   0x0048382e  1 byte, initial 0
//   netLogDisable              0x00483839  1 byte, initial 0 (local .data symbol; the preceding
//                                          byte at 0x483838 is netKickedOut)
// Observed use: advanceRealState sets netTrueStep to 1 while it re-simulates a confirmed frame and
// back to 0, and sets netLogDisable and netPrediction to 1 while the predicted local player is
// rebuilt; the prediction runner reads and writes netPredictionFastForward. They are read with
// signed byte loads in the sources that use them. Unknown: the meaning of values other than 0 and 1.
// The image keeps these in file-backed .data; here the zero values land in BSS under the reviewed
// -G0 profile. netLogDisable is a local symbol in the image and is exported here because other
// units refer to it by name. Nothing here ran on the EE.
extern "C" {
signed char netTrueStep = 0;
signed char netPrediction = 0;
signed char netPredictionFastForward = 0;
signed char netLogDisable = 0;
}
