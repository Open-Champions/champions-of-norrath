// Observed: stopCredits__Fv at 0x001ca878 is 84 bytes. It fades stream 0,
// resets the credits scroll (creditsPosY 448, endTimer 1, endInit 0,
// lineShift 0, frontEndStartInCredits 0), starts the global music file
// "mx_mainmenu_l.vag" with mode 1, then returns the front end to menu
// number 0 by clearing menuNm and pointing menu at frontMenu.
// Inferred: the mode 1 is the same looping mode runFrontEnd passes for the
// credits track; the pointer is a menu record array.
// Unknown: the exact meaning of the music mode values.
extern "C" {
void soundFadeStream__Fi(int stream);
void soundStartGlobalMusic__FPci(char* name, int mode);
extern int creditsPosY;
extern int endTimer;
extern int endInit;
extern int lineShift;
extern unsigned char frontEndStartInCredits;
extern int menuNm;
extern unsigned char* menu;
extern unsigned char frontMenu[];
}

extern "C" void stopCredits__Fv() {
    soundFadeStream__Fi(0);
    creditsPosY = 448;
    endTimer = 1;
    endInit = 0;
    lineShift = 0;
    frontEndStartInCredits = 0;
    soundStartGlobalMusic__FPci(const_cast<char*>("mx_mainmenu_l.vag"), 1);
    menuNm = 0;
    menu = frontMenu;
}
