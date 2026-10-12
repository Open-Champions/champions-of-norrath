// Observed shopKick__Fv at 0x001bbb48, 168 bytes.
// Does nothing unless the word shopRunning (0x00483f2c) is nonzero. Then it:
//  - sets the byte objectDrawEnable (0x00483ac8) and the byte floorDrawEnable
//    (0x004844e0) to 1;
//  - zeroes the two words of shopMouseDecay (0x00484a18) and the four words
//    starting at sellMode (0x007d03c8);
//  - clears shopRunning and the word gameNoDraw (0x00483f18);
//  - stores 60 in the word gameControlPaused (0x00483f1c) and clears the byte
//    interfaceOn (0x00483f45);
//  - calls Pop on the camera held in gameCam (0x00484f50), inventoryUnload, and
//    engineRemoveTask with shopFrame (0x001b2248);
//  - when the signed byte netNetworkMode (0x00483829) is nonzero it ends by
//    tail-calling hudNetWaitMsgReady, otherwise it clears the word gamePause
//    (0x00483f14).
extern "C" int shopRunning;
extern "C" unsigned char objectDrawEnable;
extern "C" unsigned char floorDrawEnable;
extern "C" int shopMouseDecay[2];
extern "C" int norrath_shop_sell_mode_words[4];
extern "C" int gameNoDraw;
extern "C" int gameControlPaused;
extern "C" signed char interfaceOn;
extern "C" void* gameCam;
extern "C" signed char netNetworkMode;
extern "C" int gamePause;

extern "C" void Pop__6Camera(void* camera);
extern "C" void inventoryUnload__Fv();
extern "C" void shopFrame__Fv();
extern "C" void engineRemoveTask__FPFv_v(void (*task)());
extern "C" void hudNetWaitMsgReady__Fv();

extern "C" void shopKick__Fv() {
    if (shopRunning == 0) return;
    objectDrawEnable = 1;
    shopMouseDecay[1] = 0;
    shopRunning = 0;
    gameNoDraw = 0;
    floorDrawEnable = 1;
    shopMouseDecay[0] = 0;
    for (int i = 3; i >= 0; i--) norrath_shop_sell_mode_words[i] = 0;
    gameControlPaused = 60;
    interfaceOn = 0;
    Pop__6Camera(gameCam);
    inventoryUnload__Fv();
    engineRemoveTask__FPFv_v(shopFrame__Fv);
    if (netNetworkMode != 0) {
        hudNetWaitMsgReady__Fv();
        return;
    }
    gamePause = 0;
}
