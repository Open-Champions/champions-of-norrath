// Observed inventoryStart__Fii at 0x001adcf0, 584 bytes: arguments the player number and a flag,
// no result. Nothing happens unless netPlayerIsLocal__Fi(player) is nonzero. Then:
//  1. With netNetworkMode zero, DLG_SetMutex__Fv() is called and a nonzero result ends the call.
//  2. interfaceOn becomes 1; engineSetBlur__Fi(128), hudClearInfoMessage__Fv() and P_Push__Fv()
//     run; the scene lights are pushed with lightPushSceneLights__FG6Point3iiiiiiiii
//     ((-0.5, -0.5, 0.5), 101, 107, 78, 0, 38, 40, 48, 0, 128).
//  3. gameNoDraw becomes 1, floorDrawEnable and objectDrawEnable become 0.
//  4. The first n records of inventory (116 bytes each; n is 1 in a network game, else 2) get
//     word +24, +28, +32, +36 and +40 = 0, word +52 = -1, word +56 = player and word +76 = 2.
//  5. SetMode__6Camerai(gameCam, 0); msgHudSetPlayerBusy__Fcc(player, 1); the word at offset 23536
//     of the player's 23984-byte record becomes 0; with netNetworkMode zero gamePause becomes 1.
//  6. inventoryUnpack__Fiib(player, 0, true); inventoryDrawDelay becomes 2; the tasks
//     inventoryFrameRun__Fv ("inventoryRun") and inventoryFrameDraw__Fv ("inventoryDraw") are added
//     with engineAddTask__FPFv_viPci at priority 50 and flag 1.
//  7. With a nonzero flag: the animation "game_lvl_complet.anm" is looked up in lump "fx.lmp" and
//     played with soundPlaySequence__FP15AnimationHeaderP10GameObjectf (object 0, volume 1.0); a
//     three-byte message {9, player, byte 89 of the player record} is sent on invMsgHandle; the word
//     at inventory + 80 becomes 5, the byte at statsScreen + 16 becomes 1 and, in the player record
//     at +240, byte +1 becomes the player number and byte +2 becomes 0.
// Inferred: this opens the inventory screen for one player; the first record is the screen of
// player 0.
// Unknown: what the message with code 9 is, and the meaning of the record words that are set.
extern "C" {
extern signed char netNetworkMode;
extern signed char interfaceOn;
extern int gameNoDraw;
extern int gamePause;
extern unsigned char floorDrawEnable;
extern signed char objectDrawEnable;
extern void* gameCam;
extern unsigned char player[];
extern unsigned char inventory[];
extern unsigned char statsScreen[];
extern int invMsgHandle;
extern int inventoryDrawDelay;

struct Point3 { float x, y, z; };
int netPlayerIsLocal__Fi(int player);
int DLG_SetMutex__Fv();
void engineSetBlur__Fi(int amount);
void hudClearInfoMessage__Fv();
void P_Push__Fv();
void lightPushSceneLights__FG6Point3iiiiiiiii(Point3 direction, int a, int b, int c, int d, int e, int f, int g, int h, int i);
void SetMode__6Camerai(void* camera, int mode);
void msgHudSetPlayerBusy__Fcc(char player, char busy);
void inventoryUnpack__Fiib(int player, int a, bool b);
void inventoryFrameRun__Fv();
void inventoryFrameDraw__Fv();
void engineAddTask__FPFv_viPci(void (*task)(), int priority, char* name, int flag);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
int netSendMessage__FiPUci(int handle, unsigned char* bytes, int count);

void inventoryStart__Fii(int playerNumber, int flag) {
    if (!netPlayerIsLocal__Fi(playerNumber)) return;
    if (netNetworkMode == 0 && DLG_SetMutex__Fv() != 0) return;
    interfaceOn = 1;
    engineSetBlur__Fi(128);
    hudClearInfoMessage__Fv();
    P_Push__Fv();
    lightPushSceneLights__FG6Point3iiiiiiiii(Point3{-0.5f, -0.5f, 0.5f}, 101, 107, 78, 0, 38, 40, 48, 0, 128);
    gameNoDraw = 1;
    floorDrawEnable = 0;
    objectDrawEnable = 0;
    const int screens = netNetworkMode != 0 ? 1 : 2;
    for (int i = 0; i < screens; ++i) {
        int* words = reinterpret_cast<int*>(inventory + i * 116);
        words[13] = -1;
        words[14] = playerNumber;
        words[19] = 2;
        words[6] = 0;
        words[7] = 0;
        words[8] = 0;
        words[9] = 0;
        words[10] = 0;
    }
    SetMode__6Camerai(gameCam, 0);
    msgHudSetPlayerBusy__Fcc(static_cast<char>(playerNumber), 1);
    unsigned char* record = player + playerNumber * 23984;
    *reinterpret_cast<int*>(record + 23536) = 0;
    if (netNetworkMode == 0) gamePause = 1;
    inventoryUnpack__Fiib(playerNumber, 0, true);
    inventoryDrawDelay = 2;
    engineAddTask__FPFv_viPci(inventoryFrameRun__Fv, 50, const_cast<char*>("inventoryRun"), 1);
    engineAddTask__FPFv_viPci(inventoryFrameDraw__Fv, 50, const_cast<char*>("inventoryDraw"), 1);
    if (flag == 0) return;
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("game_lvl_complet.anm")), nullptr, 1.0f);
    unsigned char message[3] = {9, static_cast<unsigned char>(playerNumber), record[89]};
    netSendMessage__FiPUci(invMsgHandle, message, 3);
    *reinterpret_cast<int*>(inventory + 80) = 5;
    statsScreen[16] = 1;
    record[240 + 1] = static_cast<unsigned char>(playerNumber);
    record[240 + 2] = 0;
}
}
