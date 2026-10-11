// Observed frontEndStartInNetMenu at 0x001ca9b8, 68 bytes.
// It stores 1 in isNetGame, 2 in menuNm, and the address of
// menuNetClientServer in menu, then calls memset on netGameLaunchInfo with
// 0 and 328. The menu store is the delay slot of that call, so it is visible
// to memset. menu and menuNm here are the words at 0x00484ba4 and 0x00484ba0.
extern "C" int isNetGame;
extern "C" int menuNm;
extern "C" unsigned char* menu;
extern "C" unsigned char menuNetClientServer[];
extern "C" unsigned char netGameLaunchInfo[];
extern "C" void* memset(void* destination, int value, unsigned count);

extern "C" void frontEndStartInNetMenu__Fv() {
    isNetGame = 1;
    menuNm = 2;
    menu = menuNetClientServer;
    memset(netGameLaunchInfo, 0, 328u);
}
