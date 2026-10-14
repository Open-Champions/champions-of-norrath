// Observed: MC_CheckPointAge at 0x00485be4 is 4 bytes, initial value 0, in
// .sdata. mcHeartBeatRun adds one. MC_SaveCheckPoint stores 0 after
// updateRamSlot.
// Inferred: nothing.
// Unknown: which read consumes the age.
extern "C" {
int MC_CheckPointAge;
}
