extern "C" char DLG_FireUpShopOnExit = 0;
extern "C" int nmPlayersAlive = 0;
extern "C" int playersAlive[4] = {};
static int dialog;
static int local_calls;
static int local_index;
static int shop_calls;
static int shop_index;
static int shop_second;
static int shop_third;
extern "C" int DLG_Active__Fv() { return dialog; }
extern "C" int netPlayerIsLocal__Fi(int index) {
    ++local_calls;
    local_index = index;
    return index == 4 ? 1 : 0;
}
extern "C" void shopStart__Fiii(int index, int second, int third) {
    ++shop_calls;
    shop_index = index;
    shop_second = second;
    shop_third = third;
}
extern "C" int amxActivateShop__FP5__amxPi(void*, int*);
int main() {
    dialog = 1;
    DLG_FireUpShopOnExit = 0;
    nmPlayersAlive = 2;
    if (amxActivateShop__FP5__amxPi(0, 0) != 0) return 1;
    if (DLG_FireUpShopOnExit != 1 || local_calls != 0 || shop_calls != 0) return 2;
    dialog = 0;
    DLG_FireUpShopOnExit = 3;
    playersAlive[0] = 3;
    playersAlive[1] = 4;
    nmPlayersAlive = 2;
    if (amxActivateShop__FP5__amxPi(0, 0) != 0) return 3;
    if (DLG_FireUpShopOnExit != 3 || local_calls != 2 || shop_calls != 1) return 4;
    if (shop_index != 4 || shop_second != 0 || shop_third != 1) return 5;
    nmPlayersAlive = 0;
    int shops = shop_calls;
    if (amxActivateShop__FP5__amxPi(0, 0) != 0 || shop_calls != shops) return 6;
    nmPlayersAlive = -1;
    if (amxActivateShop__FP5__amxPi(0, 0) != 0 || shop_calls != shops) return 7;
    return 0;
}
