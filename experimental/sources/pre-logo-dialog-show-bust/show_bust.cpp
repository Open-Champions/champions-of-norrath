// Observed hudShowBust__FPcPUsii at 0x001c7eb8, 172 bytes.
// A nonzero hudNoDraw, interfaceOn, or coreLoadingScreenRunning byte
// returns. The word at gameCam+52 must be 1. A nonzero gameNoDraw word
// returns. Otherwise drawBustOn becomes 5 before strcpy copies the name
// into drawBustTexName. ustrcpy then copies the text into drawBustTitle,
// and the two integers are stored in drawBustCur and drawBustMax. Those
// symbols, strcpy, and ustrcpy are not defined here.
extern "C" signed char hudNoDraw;
extern "C" signed char interfaceOn;
extern "C" unsigned char* gameCam;
extern "C" signed char coreLoadingScreenRunning;
extern "C" unsigned gameNoDraw;
extern "C" signed char drawBustOn;
extern "C" char drawBustTexName[];
extern "C" unsigned short drawBustTitle[];
extern "C" int drawBustCur;
extern "C" int drawBustMax;
extern "C" char* strcpy(char* dest, const char* src);
extern "C" void ustrcpy__FPUsT0(unsigned short* dest, unsigned short* src);

static unsigned load_word(const unsigned char* bytes) {
    return static_cast<unsigned>(bytes[0]) |
           (static_cast<unsigned>(bytes[1]) << 8) |
           (static_cast<unsigned>(bytes[2]) << 16) |
           (static_cast<unsigned>(bytes[3]) << 24);
}

extern "C" void hudShowBust__FPcPUsii(char* name, unsigned short* text, int column, int row) {
    if (hudNoDraw != 0) return;
    if (interfaceOn != 0) return;
    if (load_word(gameCam + 52) != 1u) return;
    if (coreLoadingScreenRunning != 0) return;
    if (gameNoDraw != 0u) return;
    drawBustOn = 5;
    strcpy(drawBustTexName, name);
    ustrcpy__FPUsT0(drawBustTitle, text);
    drawBustCur = column;
    drawBustMax = row;
}
