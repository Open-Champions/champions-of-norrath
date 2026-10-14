// Observed hudKick__Fv at 0x001c8008, 104 bytes.
// For each index below nmPlayers (read once) it clears the byte
// multiHelpMessageOn[i] (0x00483f38), sets the byte multiRelease[i] to 1
// (0x00486ae0), clears the word yesNoPos[i] (0x007d08d0) and stores 3 in the
// word activeDelay[i] (0x004282a0). Afterwards, whether or not the loop ran, it
// clears the byte helpMessageOn (0x00483f55). Array extents are not established;
// the loop bound is nmPlayers. The six objects are not defined here.
extern "C" int nmPlayers;
extern "C" signed char multiHelpMessageOn[];
extern "C" signed char multiRelease[];
extern "C" int yesNoPos[];
extern "C" int activeDelay[];
extern "C" signed char helpMessageOn;

extern "C" void hudKick__Fv() {
    const int count = nmPlayers;
    for (int i = 0; i < count; ++i) {
        multiHelpMessageOn[i] = 0;
        multiRelease[i] = 1;
        yesNoPos[i] = 0;
        activeDelay[i] = 3;
    }
    helpMessageOn = 0;
}
