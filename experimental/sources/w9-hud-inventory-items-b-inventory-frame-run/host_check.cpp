extern "C" {
void* inventoryLump;
int inventoryDrawDelay;
int localNmPlayersAlive;
signed char interfaceOn;
void inventoryFrameRun__Fv();
}

static int calls[8];
static int ncalls;
extern "C" void inventoryRun__Fi(int a) { if (ncalls < 8) calls[ncalls++] = a; }

static int run(void* lump, int delay, int alive, int ui) {
    inventoryLump = lump; inventoryDrawDelay = delay; localNmPlayersAlive = alive; interfaceOn = (signed char)ui;
    ncalls = 0;
    inventoryFrameRun__Fv();
    return ncalls;
}

int main() {
    int dummy;
    if (run(0, 0, 1, 1) != 0 || inventoryDrawDelay != 0) return 1;        // no lump
    if (run(&dummy, 3, 1, 1) != 0 || inventoryDrawDelay != 2) return 2;   // delay counts down
    if (run(&dummy, 1, 2, 1) != 0 || inventoryDrawDelay != 0) return 3;
    if (run(&dummy, -1, 1, 0) != 1 || calls[0] != 0 || inventoryDrawDelay != -1) return 4; // negative delay is not positive
    if (run(&dummy, 0, 1, 1) != 1 || calls[0] != 0) return 5;             // one player ignores interfaceOn
    if (run(&dummy, 0, 2, 0) != 1 || calls[0] != 1) return 6;
    if (run(&dummy, 0, 2, 1) != 2 || calls[0] != 1 || calls[1] != 0) return 7;
    if (run(&dummy, 0, 4, 5) != 2 || calls[0] != 1 || calls[1] != 0) return 8;
    if (run(&dummy, 0, 0, 1) != 0) return 9;
    if (run(&dummy, 0, -1, 1) != 0) return 10;
    return 0;
}
