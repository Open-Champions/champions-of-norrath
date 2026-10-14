// Observed amxSaveGame__FP5__amxPi at 0x0013c030, 40 bytes. It asks for
// local player 0 and passes that result to MC_SaveGameBrowser. The native
// returns 0.
extern "C" int netGetLocalPlayerNm__Fi(int player);
extern "C" void MC_SaveGameBrowser__Fi(int player);

extern "C" int amxSaveGame__FP5__amxPi(void*, int*) {
    MC_SaveGameBrowser__Fi(netGetLocalPlayerNm__Fi(0));
    return 0;
}
