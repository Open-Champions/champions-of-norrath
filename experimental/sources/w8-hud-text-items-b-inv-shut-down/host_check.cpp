// Pointer-sized table slots are scaled by the host pointer width, so the host
// fixture places them with sizeof(void*) rather than 4.
#include <cstring>

extern "C" {
signed char interfaceOn;
void* gameCam;
int inventoryLoadState;
void* inventoryLump;
int invGrid;
int shopGrid;
int invMsgHandle;
signed char netNetworkMode;
int gameControlPaused;
signed char objectDrawEnable;
int gameNoDraw;
signed char floorDrawEnable;
int gamePause;
alignas(8) unsigned char inventory[464];
alignas(8) unsigned char player[4 * 23984];
void invShutDown__Fi(int);
}

static int log_[64];
static int logn;
static void ev(int code) { log_[logn++] = code; }
static int load_calls_until_ready;
static int busy_args[2];
static int sent;
static unsigned char sent_bytes[8][3];
static int sent_handle;
static int local_mask;
static int local_nm = 3;
static void* seen_cam;
static int seen_cam_mode;
static void* seen_lump;
static int blur;

enum { LIGHT = 1, POP, SETMODE, RM_RUN, CDWAIT, LOAD, TEXREL, LUMPUN, CORE, RM_DRAW, BUSY, SEND, MUTEX, BLUR, LOCALNM };

extern "C" {
void lightPopSceneLights__Fv() { ev(LIGHT); }
void P_Pop__Fv() { ev(POP); }
void SetMode__6Camerai(void* camera, int mode) { ev(SETMODE); seen_cam = camera; seen_cam_mode = mode; }
void inventoryFrameRun__Fv() {}
void inventoryFrameDraw__Fv() {}
void engineRemoveTask__FPFv_v(void (*task)()) {
    if (task == inventoryFrameRun__Fv) ev(RM_RUN);
    else if (task == inventoryFrameDraw__Fv) ev(RM_DRAW);
    else ev(-99);
}
void cdWait__Fv() { ev(CDWAIT); }
void inventoryLoad__Fv() {
    ev(LOAD);
    if (--load_calls_until_ready <= 0) inventoryLoadState = 2;
}
void textureReleaseCacheBuffer__Fv() { ev(TEXREL); }
void lumpUnload__FPv(void* lump) { ev(LUMPUN); seen_lump = lump; }
void coreStartLoading__Fv() {
    ev(CORE);
    // The state and the grids are already cleared when loading starts.
    if (inventoryLoadState != 0 || shopGrid != 0) ev(-98);
}
int netGetLocalPlayerNm__Fi(int index) { ev(LOCALNM); return index == 0 ? local_nm + 0x100 : -1; }
void msgHudSetPlayerBusy__Fcc(char player_number, char busy) {
    ev(BUSY);
    busy_args[0] = player_number;
    busy_args[1] = busy;
}
int netPlayerIsLocal__Fi(int index) { return (local_mask >> index) & 1; }
void netSendMessage__FiPUci(int handle, unsigned char* bytes, int size) {
    ev(SEND);
    sent_handle = handle;
    if (size != 3) ev(-97);
    std::memcpy(sent_bytes[sent++], bytes, 3);
}
void DLG_ForceMutexRelease__Fv() { ev(MUTEX); }
void engineSetBlur__Fi(int amount) {
    ev(BLUR);
    blur = amount;
    if (gamePause != 0 || gameControlPaused != 60) ev(-96);
}
}

static void reset() {
    std::memset(inventory, 0, sizeof inventory);
    std::memset(player, 0, sizeof player);
    interfaceOn = 1;
    static int cam_marker;
    gameCam = &cam_marker;
    inventoryLoadState = 0;
    inventoryLump = nullptr;
    invGrid = 11;
    shopGrid = 12;
    invMsgHandle = 99;
    netNetworkMode = 1;
    gameControlPaused = 0;
    objectDrawEnable = 0;
    gameNoDraw = 1;
    floorDrawEnable = 0;
    gamePause = 1;
    logn = 0;
    sent = 0;
    local_mask = 0;
    seen_cam = nullptr;
    seen_lump = nullptr;
    blur = 0;
}

static void owner(int rec, int index) { std::memcpy(inventory + 116 * rec + 56, &index, 4); }

static void* slot(int pl, int sel) {
    return player + pl * 23984 + 21960 + sel * sizeof(void*);
}

static unsigned char target_bytes[4][40];
static unsigned char held_object[4][16];

static void hold(int pl, int sel, unsigned char value_at_30) {
    unsigned char* target = target_bytes[pl];
    target[30] = value_at_30;
    std::memcpy(held_object[pl], &target, sizeof target);
    void* p = held_object[pl];
    std::memcpy(slot(pl, sel), &p, sizeof p);
    player[pl * 23984 + 23572] = static_cast<unsigned char>(sel);
}

int main() {
    reset();
    interfaceOn = 0;
    invShutDown__Fi(0);
    if (logn != 0 || invGrid != 11 || gamePause != 1) return 1;

    // Basic path: no load pending, no lump, no local players, no network.
    reset();
    netNetworkMode = 0;
    invShutDown__Fi(0);
    static const int basic[] = {LIGHT, POP, SETMODE, RM_RUN, TEXREL, CORE, RM_DRAW, LOCALNM, BUSY, MUTEX, BLUR};
    if (logn != 11) return 2;
    for (int i = 0; i < 11; ++i) if (log_[i] != basic[i]) return 3;
    if (seen_cam_mode != 1 || seen_cam != gameCam) return 4;
    if (busy_args[0] != 3 || busy_args[1] != 0) return 5;
    if (invGrid != 0 || shopGrid != 0 || inventoryLoadState != 0 || sent != 0) return 6;
    if (gameControlPaused != 60 || objectDrawEnable != 1 || gameNoDraw != 0 || floorDrawEnable != 1 || gamePause != 0) return 7;
    if (blur != 128 || interfaceOn != 0) return 8;

    // Pending load, then a lump, with the network on (no mutex release).
    reset();
    inventoryLoadState = 1;
    load_calls_until_ready = 3;
    static int lump_marker;
    inventoryLump = &lump_marker;
    invShutDown__Fi(7);
    int loads = 0, waits = 0, unloads = 0, mutexes = 0;
    for (int i = 0; i < logn; ++i) {
        if (log_[i] == LOAD) ++loads;
        if (log_[i] == CDWAIT) ++waits;
        if (log_[i] == LUMPUN) ++unloads;
        if (log_[i] == MUTEX) ++mutexes;
        if (log_[i] < 0) return 9;
    }
    if (loads != 3 || waits != 3 || unloads != 1 || mutexes != 0) return 10;
    if (seen_lump != &lump_marker || inventoryLump != nullptr) return 11;
    // State 2 already: no loop.
    reset();
    inventoryLoadState = 2;
    invShutDown__Fi(0);
    for (int i = 0; i < logn; ++i) if (log_[i] == LOAD || log_[i] == CDWAIT) return 12;

    // Messages. Record 0 -> player 2 (local), record 1 -> player 1 (not local).
    reset();
    owner(0, 2);
    owner(1, 1);
    local_mask = 1 << 2;
    // No held object: third byte 5.
    player[2 * 23984 + 23572] = 0;
    invShutDown__Fi(0);
    if (sent != 1 || sent_handle != 99) return 13;
    if (sent_bytes[0][0] != 5 || sent_bytes[0][1] != 2 || sent_bytes[0][2] != 5) return 14;

    // Held object, word at 21900 zero: byte 30 of the pointed object, any sel.
    reset();
    owner(0, 1);
    owner(1, 3);
    local_mask = (1 << 1) | (1 << 3);
    hold(1, 0, 0x82);
    hold(3, 2, 0x11);
    invShutDown__Fi(0);
    if (sent != 2) return 15;
    if (sent_bytes[0][1] != 1 || sent_bytes[0][2] != 0x82) return 16;
    if (sent_bytes[1][1] != 3 || sent_bytes[1][2] != 0x11) return 17;

    // Held object with the word at 21900 nonzero: sel 0 gives 7, others the byte.
    reset();
    owner(0, 1);
    owner(1, 3);
    local_mask = (1 << 1) | (1 << 3);
    hold(1, 0, 0x44);
    hold(3, 1, 0x55);
    int one = 1;
    std::memcpy(player + 1 * 23984 + 21900, &one, 4);
    std::memcpy(player + 3 * 23984 + 21900, &one, 4);
    invShutDown__Fi(0);
    if (sent_bytes[0][2] != 7 || sent_bytes[1][2] != 0x55) return 18;
    return 0;
}
