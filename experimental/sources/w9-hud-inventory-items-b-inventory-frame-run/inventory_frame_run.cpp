// Observed inventoryFrameRun__Fv at 0x001b1848, 120 bytes.
// Does nothing unless the lump pointer inventoryLump (0x00484820) is nonzero.
// While the word inventoryDrawDelay (0x00484830) is positive it is decremented
// and nothing else happens that frame. Afterwards it looks at the word
// localNmPlayersAlive (0x00484f48):
//   exactly 1   -> inventoryRun__Fi(0)
//   2 or more   -> inventoryRun__Fi(1), then inventoryRun__Fi(0) if the byte
//                  interfaceOn (0x00483f45) is nonzero
//   0 or negative -> nothing
// What the argument of inventoryRun__Fi selects (a player slot or a screen half)
// is not established. None of the four objects or inventoryRun__Fi is defined here.
extern "C" {
extern void* inventoryLump;
extern int inventoryDrawDelay;
extern int localNmPlayersAlive;
extern signed char interfaceOn;
void inventoryRun__Fi(int);
}

extern "C" void inventoryFrameRun__Fv() {
    if (inventoryLump == nullptr) return;
    if (inventoryDrawDelay > 0) {
        inventoryDrawDelay = inventoryDrawDelay - 1;
        return;
    }
    int alive = localNmPlayersAlive;
    if (alive == 1) {
        inventoryRun__Fi(0);
    } else if (alive >= 2) {
        inventoryRun__Fi(1);
        if (interfaceOn != 0) inventoryRun__Fi(0);
    }
}
