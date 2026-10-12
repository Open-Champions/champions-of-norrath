// endTimer.1059 at 0x004842e8, 4 bytes, in SLUS_205.65.
// Observed: local word in .data with file value 0, accessed by pauseMenuFrameRun with word loads and stores. Its name carries the .1059 suffix in the symbol table, so it is bound with an assembler label. It is a different object from endTimer at 0x00484bec.
extern "C" {
int endTimerPause asm("endTimer.1059") = 0;
}
