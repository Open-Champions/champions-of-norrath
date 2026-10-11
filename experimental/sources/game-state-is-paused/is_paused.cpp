// Observed gameIsPaused__Fv at 0x001d0e70, 148 bytes.
// netNetworkMode is a byte. A nonzero value returns the gamePause word as it
// stands, including 0 and negative values. cutMode is a byte. A nonzero value
// returns the coreLoading word loaded for that branch. frameNm is compared
// with slti against 60, so a negative word returns 0. After that, a nonzero
// gamePause or coreLoading word returns 1. hudNoController is a byte; 0
// returns 0 without calling DLG_Active. A nonzero DLG_Active result returns 0.
// cutMode is loaded again after that call, and a nonzero byte returns 0.
// Otherwise the result is 1. Nothing here was executed.
extern "C" unsigned char netNetworkMode;
extern "C" int gamePause;
extern "C" unsigned char cutMode;
extern "C" int coreLoading;
extern "C" int frameNm;
extern "C" unsigned char hudNoController;
extern "C" int DLG_Active__Fv();

extern "C" int gameIsPaused__Fv() {
    if (netNetworkMode != 0) return gamePause;
    if (cutMode != 0) return coreLoading;
    if (frameNm < 60) return 0;
    if (gamePause != 0) return 1;
    if (coreLoading != 0) return 1;
    if (hudNoController == 0) return 0;
    if (DLG_Active__Fv() != 0) return 0;
    if (cutMode != 0) return 0;
    return 1;
}
