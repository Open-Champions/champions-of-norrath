// Observed frontEndInit at 0x001c96b0, 372 bytes. It returns.
// The four text arguments have no ELF symbol. Their bytes are the sequences
// at 0x0046c2c0, 0x00484c20, 0x00484c38 and 0x0046bf10.
// menuSetPos receives the address of frontMenu. The credits path stores the
// address of creditMenu. engineAddTask receives runFrontEnd.
struct Menu;

extern "C" {
void MC_GetReadyForMainMenu__Fv();
void MC_LoadGameBrowser__Fii(int first, int second);
void menuSetPos__FP4Menui(Menu* target, int position);
void padInitPadIndexMap__Fv();
void runFrontEnd__Fv();
void engineAddTask__FPFv_viPci(void (*callback)(), int second, char* third, int fourth);
void lumpLoad__FPc(char* name);

extern unsigned char frontEndSuspend;
extern unsigned char hudNoDraw;
extern unsigned char netNetworkMode;
extern int nmPlayers;
extern int musicPlaying;
extern unsigned char attemptLoadGame;
extern unsigned char MC_LastModeSuccessful;
extern int menuNm;
extern Menu* menu;
extern Menu frontMenu;
extern Menu creditMenu;
extern int autodemoTimer;
extern unsigned char levelName[];
extern unsigned char frontEndStartInCredits;
extern int frontendControllerConfigRunning;
extern int frontEndRunning;
extern int lineShift;
extern int expWarnAlpha;
extern int gamePause;
extern int gameNoDraw;
extern unsigned char floorDrawEnable;
extern unsigned char objectDrawEnable;
extern int creditsPosY;
extern int engineHoldMpeg;
extern int emergencyMusicWaitAbort;

char norrath_frontend_task_name[] = "frontend";
char norrath_frontend_fx_lump[] = "fx.lmp";
char norrath_frontend_intro_level[] = "intro";
char norrath_frontend_lump[] = "frontend.lmp";

void frontEndInit__Fi(int value) {
    MC_GetReadyForMainMenu__Fv();
    hudNoDraw = 1;
    netNetworkMode = 0;
    nmPlayers = 1;
    musicPlaying = 0;
    if (frontEndSuspend == 0) {
        // The original loads MC_LastModeSuccessful in the delay slot of the
        // attemptLoadGame test, including when that byte is zero.
        unsigned char last_mode = MC_LastModeSuccessful;
        if (attemptLoadGame != 0 && last_mode == 0) {
            if (menuNm != 0) menuNm = 15;
            attemptLoadGame = 0;
            MC_LoadGameBrowser__Fii(0, 1);
        } else {
            menu = &frontMenu;
            menuSetPos__FP4Menui(&frontMenu, 0);
            menuNm = 0;
        }
    }
    padInitPadIndexMap__Fv();
    autodemoTimer = 0;
    volatile unsigned char* copied = levelName;
    const volatile unsigned char* source =
        reinterpret_cast<const volatile unsigned char*>(norrath_frontend_intro_level);
    for (int byte = 0; byte < 6; ++byte) copied[byte] = source[byte];
    copied[80] = 0xffu;
    frontEndStartInCredits = static_cast<unsigned char>(value);
    frontendControllerConfigRunning = 0;
    frontEndRunning = 1;
    lineShift = 0;
    expWarnAlpha = 0;
    gamePause = 0;
    gameNoDraw = 0;
    floorDrawEnable = 1;
    objectDrawEnable = 1;
    frontEndSuspend = 0;
    engineAddTask__FPFv_viPci(runFrontEnd__Fv, 25, norrath_frontend_task_name, 0);
    if (frontEndStartInCredits != 0) {
        creditsPosY = 448;
        menu = &creditMenu;
        menuNm = 60;
    } else {
        engineHoldMpeg = 1;
        emergencyMusicWaitAbort = 0;
    }
    lumpLoad__FPc(norrath_frontend_fx_lump);
    lumpLoad__FPc(norrath_frontend_lump);
}
}
