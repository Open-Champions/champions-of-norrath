// Observed: gameDeath__Fv at 0x001cd7c0 is 212 bytes with an 80-byte frame. It
// switches on the signed byte deathState:
//   0: calls hudFadeScreen__Fi(4); only when it returns exactly 128 does it add
//      1 to deathState (read and stored as an unsigned byte).
//   1: if the byte deathNetHack is nonzero it stores 1 in engineDisplayDisable
//      (a word) and in netNetworkMode (a byte); then it calls MC_LoadRamSlot__Fv
//      and adds 1 to deathState.
//   2: if the first byte of levelName is zero, it fills a local 64-byte buffer
//      with gameGetFullLevelName__FPc, compares it with MC_GetLoadName__Fv()
//      using strcmp, and on equality calls engineRemoveTask__FPFv_v with this
//      function's own address.
//   any other value: nothing.
// Inferred: this is a task function, from the self-removal; each state is one
// step of a death sequence (fade, reload, wait for the reloaded level).
// Unknown: what the hudFadeScreen__Fi result 128 means, why deathNetHack forces
// network mode on, and the declared size of levelName.

extern "C" int hudFadeScreen__Fi(int mode);
extern "C" void MC_LoadRamSlot__Fv();
extern "C" void gameGetFullLevelName__FPc(char* out);
extern "C" char* MC_GetLoadName__Fv();
extern "C" int strcmp(const char* a, const char* b);
extern "C" void engineRemoveTask__FPFv_v(void (*task)());
extern "C" signed char deathState;
extern "C" signed char deathNetHack;
extern "C" int engineDisplayDisable;
extern "C" signed char netNetworkMode;
extern "C" char levelName[];

extern "C" void gameDeath__Fv() {
    switch (deathState) {
    case 0:
        if (hudFadeScreen__Fi(4) == 128) deathState = static_cast<signed char>(
            static_cast<unsigned char>(deathState) + 1);
        break;
    case 1:
        if (deathNetHack != 0) {
            engineDisplayDisable = 1;
            netNetworkMode = 1;
        }
        MC_LoadRamSlot__Fv();
        deathState = static_cast<signed char>(static_cast<unsigned char>(deathState) + 1);
        break;
    case 2:
        if (levelName[0] == 0) {
            char name[64];
            gameGetFullLevelName__FPc(name);
            if (strcmp(name, MC_GetLoadName__Fv()) == 0) engineRemoveTask__FPFv_v(gameDeath__Fv);
        }
        break;
    default:
        break;
    }
}
