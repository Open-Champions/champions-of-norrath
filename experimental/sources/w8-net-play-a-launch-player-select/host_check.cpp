#include "launch_player_select.cpp"
#include <cstring>
unsigned char camera[16];
unsigned char* gameCam = camera;
int frontEndRunning;
unsigned char norrath_frontend_byte_484ba8;
unsigned char playerSelectNextName[64];
unsigned char playerSelectPrevName[64];
unsigned char frontEndSuspend;
signed char hudNoDraw;
unsigned short* playerSelectExitMessage;
unsigned char attemptLoadGame;
unsigned char playerSelectCount;
unsigned char playerSelectStartPlayer;
unsigned char playerSelectEnforce;
unsigned char norrath_player_select_level_bytes_484c40[8] = {'k', 'e', 'l', 's', 'e', 'l', '2', 0};
unsigned char levelName[322];
static unsigned short messageText[2];
static char order[16];
static int n;
static void log(char c) { order[n++] = c; }
extern "C" {
void cutStop__Fv() { log('c'); }
void runFrontEnd__Fv() {}
static void (*removed)();
void engineRemoveTask__FPFv_v(void (*cb)()) { removed = cb; log('r'); }
void SetMode__6Camerai(unsigned char* cam, int mode) {
    if (cam == camera && mode == 1) log('m');
}
unsigned short* textGetString__FPc(char* key) {
    if (std::strcmp(key, "exitToMenuMsg") == 0) log('t');
    return messageText;
}
void scriptSetGameState__FPci(char* name, int value) {
    if (std::strcmp(name, "needPlayers") == 0 && value == 1) log('s');
}
void engineSetDisplayDisable__Fi(int v) { if (v == 10) log('d'); }
void engineSyncVBlank__Fv() { log('v'); }
}
int main() {
    frontEndRunning = 1;
    norrath_frontend_byte_484ba8 = 0x5a;
    hudNoDraw = 1;
    attemptLoadGame = 1;
    playerSelectCount = 4;
    playerSelectStartPlayer = 3;
    playerSelectEnforce = 1;
    levelName[8] = 'x';
    launchPlayerSelect__Fv();
    if (std::strcmp(order, "crmtsdv") != 0) return 1;
    if (removed != runFrontEnd__Fv) return 2;
    if (frontEndRunning != 0 || frontEndSuspend != 1 || hudNoDraw != 0) return 3;
    if (playerSelectNextName[0] != 0x5a || playerSelectPrevName[0] != 0x5a) return 4;
    if (playerSelectExitMessage != messageText) return 5;
    if (attemptLoadGame != 0 || playerSelectCount != 1 || playerSelectStartPlayer != 0 || playerSelectEnforce != 0) return 6;
    if (std::memcmp(levelName, "kelsel2", 8) != 0 || levelName[8] != 'x' || levelName[80] != 0xff) return 7;
    return 0;
}
