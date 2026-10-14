// Observed hudShowCompass__FG6Point3PUs at 0x001c7e18, 160 bytes.
// The 12 point bytes are read before the tests. A nonzero hudNoDraw,
// interfaceOn, or coreLoadingScreenRunning byte returns. The word at
// gameCam+52 must be 1. A nonzero gameNoDraw word returns. Otherwise
// drawCompassOn becomes 5, ustrcpy copies the text to drawCompassTitle,
// and the 12 bytes are stored at drawCompassPoint. Those symbols are not
// defined here.
extern "C" signed char hudNoDraw;
extern "C" signed char interfaceOn;
extern "C" unsigned char* gameCam;
extern "C" signed char coreLoadingScreenRunning;
extern "C" unsigned gameNoDraw;
extern "C" signed char drawCompassOn;
extern "C" unsigned short drawCompassTitle[];
extern "C" unsigned char drawCompassPoint[];
extern "C" void ustrcpy__FPUsT0(unsigned short* dest, unsigned short* src);

static unsigned load_word(const unsigned char* bytes) {
    return static_cast<unsigned>(bytes[0]) |
           (static_cast<unsigned>(bytes[1]) << 8) |
           (static_cast<unsigned>(bytes[2]) << 16) |
           (static_cast<unsigned>(bytes[3]) << 24);
}

extern "C" void hudShowCompass__FG6Point3PUs(const unsigned char* point, unsigned short* text) {
    unsigned char local[12];
    for (int i = 0; i < 12; ++i) local[i] = point[i];
    if (hudNoDraw != 0) return;
    if (interfaceOn != 0) return;
    if (load_word(gameCam + 52) != 1u) return;
    if (coreLoadingScreenRunning != 0) return;
    if (gameNoDraw != 0u) return;
    drawCompassOn = 5;
    ustrcpy__FPUsT0(drawCompassTitle, text);
    for (int i = 0; i < 12; ++i) drawCompassPoint[i] = local[i];
}
