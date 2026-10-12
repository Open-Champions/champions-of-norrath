// Observed invShutDown__Fi at 0x001b1358, 460 bytes. The argument is not read.
// Nothing happens unless the byte interfaceOn (0x00483f45) is nonzero. Then, in
// this order:
//   lightPopSceneLights__Fv, P_Pop__Fv, SetMode__6Camerai(gameCam, 1),
//   engineRemoveTask__FPFv_v(inventoryFrameRun__Fv);
//   while inventoryLoadState (0x00484824) is neither 0 nor 2: cdWait__Fv then
//     inventoryLoad__Fv, until the state equals 2 (the state is re-read after
//     every call);
//   textureReleaseCacheBuffer__Fv; a nonzero inventoryLump (0x00484820) goes to
//     lumpUnload__FPv and is zeroed; invGrid (0x00484544) is zeroed either way;
//   shopGrid (0x0048491c) and inventoryLoadState are zeroed, then
//     coreStartLoading__Fv; engineRemoveTask__FPFv_v(inventoryFrameDraw__Fv);
//   msgHudSetPlayerBusy__Fcc(low byte of netGetLocalPlayerNm__Fi(0), 0).
// Then for the first two 116-byte records of the inventory table (offset 56 of
// each holds a player index; player records are 23984 bytes apart): when
// netPlayerIsLocal__Fi(index) is nonzero a three-byte message is sent on
// invMsgHandle with netSendMessage__FiPUci: 5, the index low byte, and a
// third byte chosen as follows. Let sel be the signed byte at player offset
// 23572 and ptr the word at player offset 21960 + 4 * sel. With ptr zero the
// third byte is 5. Otherwise, when the word at player offset 21900 is zero, it is the
// signed byte at offset 30 of the object pointed to by the first word of *ptr.
// When that word is nonzero, a sel of zero gives 7 and any other sel gives the
// same byte at offset 30. The pointer sizes are 32-bit on the target.
// Finally, when netNetworkMode (0x00483829) is zero DLG_ForceMutexRelease__Fv is
// called. Then gameControlPaused is set to 60, objectDrawEnable to 1, gameNoDraw
// to 0, floorDrawEnable to 1, gamePause to 0, engineSetBlur__Fi(128) is called,
// and interfaceOn is cleared. The meanings of the player offsets are not
// established. All callees and objects are defined elsewhere.
extern "C" {
extern signed char interfaceOn;
extern void* gameCam;
extern int inventoryLoadState;
extern void* inventoryLump;
extern int invGrid;
extern int shopGrid;
extern int invMsgHandle;
extern signed char netNetworkMode;
extern int gameControlPaused;
extern signed char objectDrawEnable;
extern int gameNoDraw;
extern signed char floorDrawEnable;
extern int gamePause;
extern unsigned char inventory[];
extern unsigned char player[];

void lightPopSceneLights__Fv();
void P_Pop__Fv();
void SetMode__6Camerai(void* camera, int mode);
void engineRemoveTask__FPFv_v(void (*task)());
void inventoryFrameRun__Fv();
void inventoryFrameDraw__Fv();
void cdWait__Fv();
void inventoryLoad__Fv();
void textureReleaseCacheBuffer__Fv();
void lumpUnload__FPv(void* lump);
void coreStartLoading__Fv();
int netGetLocalPlayerNm__Fi(int index);
void msgHudSetPlayerBusy__Fcc(char player_number, char busy);
int netPlayerIsLocal__Fi(int index);
void netSendMessage__FiPUci(int handle, unsigned char* bytes, int size);
void DLG_ForceMutexRelease__Fv();
void engineSetBlur__Fi(int amount);
}

extern "C" void invShutDown__Fi(int) {
    unsigned char message[400];
    if (interfaceOn == 0) return;
    lightPopSceneLights__Fv();
    P_Pop__Fv();
    SetMode__6Camerai(gameCam, 1);
    engineRemoveTask__FPFv_v(inventoryFrameRun__Fv);
    if (inventoryLoadState != 0 && inventoryLoadState != 2) {
        do {
            cdWait__Fv();
            inventoryLoad__Fv();
        } while (inventoryLoadState != 2);
    }
    textureReleaseCacheBuffer__Fv();
    if (inventoryLump != 0) {
        lumpUnload__FPv(inventoryLump);
        inventoryLump = 0;
    }
    invGrid = 0;
    shopGrid = 0;
    inventoryLoadState = 0;
    coreStartLoading__Fv();
    engineRemoveTask__FPFv_v(inventoryFrameDraw__Fv);
    msgHudSetPlayerBusy__Fcc(static_cast<char>(netGetLocalPlayerNm__Fi(0)), 0);
    for (int i = 0; i < 2; ++i) {
        const int owner = *reinterpret_cast<int*>(inventory + 116 * i + 56);
        if (netPlayerIsLocal__Fi(owner) == 0) continue;
        unsigned char* record = player + owner * 23984;
        const signed char sel = static_cast<signed char>(record[23572]);
        void* const* entry = reinterpret_cast<void* const*>(record + 21960) + sel;
        const unsigned char* held = static_cast<const unsigned char*>(*entry);
        signed char third = 5;
        if (held != 0) {
            const unsigned char* const* inner = reinterpret_cast<const unsigned char* const*>(held);
            if (*reinterpret_cast<int*>(record + 21900) == 0) {
                third = static_cast<signed char>((*inner)[30]);
            } else {
                third = 7;
                if (sel != 0) third = static_cast<signed char>((*inner)[30]);
            }
        }
        message[0] = 5;
        message[1] = static_cast<unsigned char>(*reinterpret_cast<int*>(inventory + 116 * i + 56));
        message[2] = static_cast<unsigned char>(third);
        netSendMessage__FiPUci(invMsgHandle, message, 3);
    }
    if (netNetworkMode == 0) DLG_ForceMutexRelease__Fv();
    gameControlPaused = 60;
    objectDrawEnable = 1;
    gameNoDraw = 0;
    floorDrawEnable = 1;
    gamePause = 0;
    engineSetBlur__Fi(128);
    interfaceOn = 0;
}
