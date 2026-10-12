// Observed: MC_ChangePlayers__FiPUsiii at 0x0025b298 is 112 bytes. It returns
// at once unless disableMC is nonzero or (asyncMode is -1 and errorCondition is
// zero). Otherwise it stores its first argument as the byte whichPad, its fifth
// as the byte MC_FrontEndImport, its second (a pointer) in pcTitle, its third
// and fourth as the bytes pcNmPlayers and pcLaunch, and zero in the byte
// MC_LastModeSuccessful. It then calls stopCardQuery__Fv, newMode__Fii(7, 0)
// and pushPause__Fv.
// Inferred: nothing.
// Unknown: the unit of the second argument beyond its 16-bit element type in the
// mangled name; the meaning of the third and fourth arguments.

extern "C" int stopCardQuery__Fv();
extern "C" int newMode__Fii(int mode, int keep);
extern "C" void pushPause__Fv();
extern "C" int disableMC;
extern "C" int asyncMode;
extern "C" int errorCondition;
extern "C" signed char whichPad;
extern "C" signed char MC_FrontEndImport;
extern "C" unsigned short* pcTitle;
extern "C" signed char pcNmPlayers;
extern "C" signed char pcLaunch;
extern "C" unsigned char MC_LastModeSuccessful;

extern "C" void MC_ChangePlayers__FiPUsiii(int pad, unsigned short* title, int players, int launch, int frontEnd) {
    if (disableMC == 0 && (asyncMode != -1 || errorCondition != 0)) {
        return;
    }
    whichPad = pad;
    MC_FrontEndImport = frontEnd;
    pcTitle = title;
    pcNmPlayers = players;
    pcLaunch = launch;
    MC_LastModeSuccessful = 0;
    stopCardQuery__Fv();
    newMode__Fii(7, 0);
    pushPause__Fv();
}
