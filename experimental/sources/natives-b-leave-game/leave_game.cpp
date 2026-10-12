// Observed amxLeaveGame at 0x0013bee8, 28 bytes. A zero first cell
// stores 1 in gameBailFromGameLoop at 0x00483f28. Any other cell
// stores 101. The native returns zero.
extern "C" int gameBailFromGameLoop;

extern "C" int amxLeaveGame__FP5__amxPi(void*, int* params) {
    gameBailFromGameLoop = params[1] == 0 ? 1 : 101;
    return 0;
}
