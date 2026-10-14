extern "C" {
int inventoryLoadState;
void* inventoryLump;
void* invGrid;
void* shopGrid;
void inventoryUnload__Fv();
}

static int log_[32], n;
static int stepsToTwo;       // number of inventoryLoad calls until state reaches 2
static int stateAtCoreStart = -1;
static void* unloaded;
static void* releasedLumpSeen;

extern "C" void cdWait__Fv() { log_[n++] = 1; }
extern "C" void inventoryLoad__Fv() {
    log_[n++] = 2;
    if (--stepsToTwo <= 0) inventoryLoadState = 2; else inventoryLoadState = 1;
}
extern "C" void textureReleaseCacheBuffer__Fv() { log_[n++] = 3; releasedLumpSeen = inventoryLump; }
extern "C" void lumpUnload__FPv(void* p) { log_[n++] = 4; unloaded = p; }
extern "C" void coreStartLoading__Fv() { log_[n++] = 5; stateAtCoreStart = inventoryLoadState; }

static void setup(int state, void* lump, int steps) {
    inventoryLoadState = state; inventoryLump = lump; invGrid = (void*)1; shopGrid = (void*)1;
    n = 0; stepsToTwo = steps; stateAtCoreStart = -1; unloaded = nullptr;
}

int main() {
    int dummy;
    // state 0: no waiting, lump unloaded
    setup(0, &dummy, 0);
    inventoryUnload__Fv();
    if (n != 3 || log_[0] != 3 || log_[1] != 4 || log_[2] != 5) return 1;
    if (unloaded != &dummy || releasedLumpSeen != &dummy) return 2;
    if (inventoryLump || invGrid || shopGrid || inventoryLoadState != 0 || stateAtCoreStart != 0) return 3;
    // state 2: same
    setup(2, &dummy, 0);
    inventoryUnload__Fv();
    if (n != 3) return 4;
    // state 1: loop cdWait/inventoryLoad until it reads 2 (three iterations here)
    setup(1, &dummy, 3);
    inventoryUnload__Fv();
    if (n != 3 * 2 + 3) return 5;
    for (int i = 0; i < 6; ++i) if (log_[i] != (i % 2 ? 2 : 1)) return 6;
    if (log_[6] != 3 || log_[7] != 4 || log_[8] != 5) return 7;
    // state 3 also loops (anything but 0 or 2), once here
    setup(3, &dummy, 1);
    inventoryUnload__Fv();
    if (n != 5 || log_[0] != 1 || log_[1] != 2) return 8;
    // no lump: lumpUnload not called, grid words still cleared
    setup(0, nullptr, 0);
    inventoryUnload__Fv();
    if (n != 2 || log_[0] != 3 || log_[1] != 5 || invGrid || shopGrid || inventoryLump) return 9;
    return 0;
}
