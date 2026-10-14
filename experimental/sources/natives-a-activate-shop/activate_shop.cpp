// amxActivateShop is the 136-byte native at 0x0013b640. When DLG_Active is
// nonzero it stores 1 in the byte DLG_FireUpShopOnExit at 0x00485078 and
// returns zero. Otherwise it walks nmPlayersAlive indexes in playersAlive at
// 0x0042a3e8. Each local index is passed to shopStart with 0 and 1.
extern "C" int DLG_Active__Fv();
extern "C" char DLG_FireUpShopOnExit;
extern "C" int nmPlayersAlive;
extern "C" int playersAlive[];
extern "C" int netPlayerIsLocal__Fi(int index);
extern "C" void shopStart__Fiii(int index, int second, int third);
extern "C" int amxActivateShop__FP5__amxPi(void* amx, int* params) {
    (void)amx;
    (void)params;
    if (DLG_Active__Fv() != 0) {
        DLG_FireUpShopOnExit = 1;
        return 0;
    }
    int cursor = 0;
    while (cursor < nmPlayersAlive) {
        int index = playersAlive[cursor];
        if (netPlayerIsLocal__Fi(index) != 0) shopStart__Fiii(index, 0, 1);
        cursor += 1;
    }
    return 0;
}
