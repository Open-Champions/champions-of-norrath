// Observed front-end data words in the pinned SLUS-20565 image.
// frontMenu at 0x004286e0 is 80 bytes: four 16-byte records whose first word
// points at menu_startnewgame, menu_loadsavedgame, menu_options and
// menu_credits, then one all-zero record. creditMenu at 0x004289c0 is 16 zero
// bytes. frontEndRunning (0x00483f30) and engineHoldMpeg (0x00484424) are
// zero words. frontEndSuspend (0x00484bdd) and frontEndStartInCredits
// (0x00484bde) are zero bytes. creditsPosY (0x00484be4) starts at 448 and
// autodemoTimer (0x00484be8) and lineShift (0x00484bf4) start at zero.
// frontendControllerConfigRunning (0x00484c6c) is a zero word.
// attemptLoadGame (0x00486af0) and frontEndWasRunning (0x00486cba) are bytes
// in .sbss and expWarnAlpha (0x00486af8) is a word there.
//
// Inferred: the four-word record shape from the 16-byte stride that
// runFrontEnd uses. Unknown: what the last three words of each record hold.
// The neighbours endTimer, endInit and curFocus are not defined here.
struct norrath_frontend_record {
    const char* label;
    unsigned second;
    unsigned third;
    unsigned fourth;
};

extern "C" {
norrath_frontend_record frontMenu[5] = {
    {"menu_startnewgame", 0u, 0u, 0u},
    {"menu_loadsavedgame", 0u, 0u, 0u},
    {"menu_options", 0u, 0u, 0u},
    {"menu_credits", 0u, 0u, 0u},
    {0, 0u, 0u, 0u},
};
norrath_frontend_record creditMenu[1] = {{0, 0u, 0u, 0u}};
int frontEndRunning = 0;
int engineHoldMpeg = 0;
unsigned char frontEndSuspend = 0;
unsigned char frontEndStartInCredits = 0;
int creditsPosY = 448;
int autodemoTimer = 0;
int lineShift = 0;
int frontendControllerConfigRunning = 0;
unsigned char attemptLoadGame;
float expWarnAlpha;
unsigned char frontEndWasRunning;
}
