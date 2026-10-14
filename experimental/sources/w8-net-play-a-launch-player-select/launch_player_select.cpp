// Observed launchPlayerSelect__Fv at 0x001c9828, 200 bytes. In order: cutStop();
// engineRemoveTask(runFrontEnd); gameCam->SetMode(1); frontEndRunning = 0; the
// byte at 0x484ba8 is copied to the first byte of playerSelectNextName and of
// playerSelectPrevName; frontEndSuspend = 1; hudNoDraw = 0;
// playerSelectExitMessage = textGetString("exitToMenuMsg"); attemptLoadGame = 0;
// playerSelectCount = 1; playerSelectStartPlayer = 0; playerSelectEnforce = 0;
// scriptSetGameState("needPlayers", 1); engineSetDisplayDisable(10);
// engineSyncVBlank(). Finally the 8 bytes at 0x484c40 (observed text "kelsel2"
// plus its terminator) are copied over the start of levelName and levelName[80]
// is set to 0xff. No result is produced.
extern "C" {
extern unsigned char* gameCam;
extern int frontEndRunning;
extern unsigned char norrath_frontend_byte_484ba8;
extern unsigned char playerSelectNextName[];
extern unsigned char playerSelectPrevName[];
extern unsigned char frontEndSuspend;
extern signed char hudNoDraw;
extern unsigned short* playerSelectExitMessage;
extern unsigned char attemptLoadGame;
extern unsigned char playerSelectCount;
extern unsigned char playerSelectStartPlayer;
extern unsigned char playerSelectEnforce;
extern unsigned char norrath_player_select_level_bytes_484c40[8];
extern unsigned char levelName[];
void cutStop__Fv();
void runFrontEnd__Fv();
void engineRemoveTask__FPFv_v(void (*callback)());
void SetMode__6Camerai(unsigned char* camera, int mode);
unsigned short* textGetString__FPc(char* key);
void scriptSetGameState__FPci(char* name, int value);
void engineSetDisplayDisable__Fi(int value);
void engineSyncVBlank__Fv();

void launchPlayerSelect__Fv() {
    cutStop__Fv();
    engineRemoveTask__FPFv_v(runFrontEnd__Fv);
    SetMode__6Camerai(gameCam, 1);
    frontEndRunning = 0;
    unsigned char carried = norrath_frontend_byte_484ba8;
    playerSelectNextName[0] = carried;
    playerSelectPrevName[0] = carried;
    frontEndSuspend = 1;
    hudNoDraw = 0;
    playerSelectExitMessage = textGetString__FPc(const_cast<char*>("exitToMenuMsg"));
    attemptLoadGame = 0;
    playerSelectCount = 1;
    playerSelectStartPlayer = 0;
    playerSelectEnforce = 0;
    scriptSetGameState__FPci(const_cast<char*>("needPlayers"), 1);
    engineSetDisplayDisable__Fi(10);
    engineSyncVBlank__Fv();
    for (int i = 0; i < 8; ++i) levelName[i] = norrath_player_select_level_bytes_484c40[i];
    levelName[80] = 0xff;
}
}
