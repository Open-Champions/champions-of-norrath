#define NORRATH_HOST_CONTRACT
#include "draw_dialog.cpp"
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstring>

extern "C" {
unsigned long curDlg;
int swooping, flaredIn, flaredOut;
float coreInterfaceFadeUpVal;
void* mediumFont;
int letterAtATime, textBodyChars;
unsigned short textBody[64];
int scrollY;
unsigned DLG_ShowDialogText;
int textPageDone, showOptions, showMore, optionCount;
signed char waitForSound;
int cancelStreamWait;
int optionIndex, nextIndex;
unsigned optTbl[8];
int optionLinePos[8];
float cursorTween;
int nameChars;
unsigned short dlgName[64];
int activePlayer;
unsigned char player[4 * 23984];
void* haloTexture7;
int padAcceptButtonFontChar;
int loadSampleRate;
}

static int fades, starts, restricts, wraps, colors, prints, printus, ends, draws;
static int centeredus, centeredis, statusCalls, hermites, ustrcpys, texts;
static unsigned startColor;
static int restrictArg, scissorArgs[4], wrapArgs[7], colorArg;
static void* wrapText;
static int printArgs[3], printuArgs[3], centeredArgs[3], textureArgs[4];
static void* textureTex, *centeredText, *printText;
static void* textIn;
static unsigned textureColor;
static char glyphText[64], whoText[64];
static int scissorCalls = 0;
static int statusResult = 0, localResult = 0, localArg = -1;
static float hermiteResult = 0;
static float hermiteIn[5];
static char textKey[32];

static void reset_stubs() {
    fades = starts = restricts = wraps = colors = prints = printus = ends = draws = 0;
    centeredus = centeredis = statusCalls = hermites = ustrcpys = texts = 0;
    startColor = 0; restrictArg = 0; colorArg = 0; wrapText = nullptr;
    memset(scissorArgs, 0, sizeof scissorArgs); memset(wrapArgs, 0, sizeof wrapArgs);
    memset(printArgs, 0, sizeof printArgs); memset(printuArgs, 0, sizeof printuArgs);
    memset(centeredArgs, 0, sizeof centeredArgs); memset(textureArgs, 0, sizeof textureArgs);
    textureTex = centeredText = printText = nullptr; textureColor = 0;
    textIn = nullptr;
    memset(glyphText, 0, sizeof glyphText); memset(whoText, 0, sizeof whoText);
    memset(hermiteIn, 0, sizeof hermiteIn); memset(textKey, 0, sizeof textKey);
}
static void reset_state() {
    curDlg = 0; swooping = flaredIn = flaredOut = 0;
    coreInterfaceFadeUpVal = 0; letterAtATime = textBodyChars = 0;
    scrollY = 0; DLG_ShowDialogText = 0;
    textPageDone = showOptions = showMore = optionCount = 0;
    waitForSound = 0; cancelStreamWait = 0;
    optionIndex = nextIndex = 0; cursorTween = 0; nameChars = 0;
    activePlayer = 0; padAcceptButtonFontChar = 0; loadSampleRate = 0;
    memset(optTbl, 0, sizeof optTbl); memset(optionLinePos, 0, sizeof optionLinePos);
    memset(dlgName, 0, sizeof dlgName); memset(textBody, 0, sizeof textBody);
    memset(player, 0, sizeof player);
    statusResult = 0; localResult = 0; localArg = -1; hermiteResult = 0;
    scissorCalls = 0;
    reset_stubs();
}

extern "C" {
int coreInterfaceFadeUp__Fi(int f) { ++fades; return f; }
void fontStart__FP4FontiUii(void* f, int s, unsigned c, int g) {
    ++starts; startColor = (starts == 1) ? c : startColor; (void)f; (void)s; (void)g;
}
void fontRestrict__Fi(int c) { ++restricts; restrictArg = c; }
int lineToY__Fi(int l) { return l * 10; }
void fontScissor__Fiiii(int x0, int y0, int x1, int y1) {
    if (scissorCalls++ == 0) {
        scissorArgs[0] = x0; scissorArgs[1] = y0; scissorArgs[2] = x1; scissorArgs[3] = y1;
    }
}
int fontPrintWrapu__FiiPUsiiii(int x, int y, unsigned short* t, int w, int lo, int hi, int st) {
    ++wraps; wrapArgs[0] = x; wrapArgs[1] = y; wrapText = t;
    wrapArgs[2] = w; wrapArgs[3] = lo; wrapArgs[4] = hi; wrapArgs[5] = st;
    return 0;
}
void fontColor__FUiii(unsigned c, int f, int e) { ++colors; colorArg = (int)c; (void)f; (void)e; }
int soundGetStreamPlayStatus__Fi(int) { ++statusCalls; return statusResult; }
unsigned short* textGetString__FPc(char* k) {
    ++texts; textIn = k; strncpy(textKey, k, 31);
    static unsigned short s[] = {'K', 0}; return s;
}
int fontPrintu__FiiPUsi(int x, int y, unsigned short*, int f) {
    ++printus; printuArgs[0] = x; printuArgs[1] = y; printuArgs[2] = f; return 0;
}
float evalHermite__Ffffff(float a, float b, float c, float d, float e) {
    ++hermites; hermiteIn[0] = a; hermiteIn[1] = b; hermiteIn[2] = c;
    hermiteIn[3] = d; hermiteIn[4] = e; return hermiteResult;
}
int sprintf(char* d, const char* f, ...) {
    va_list ap; va_start(ap, f); int n = vsnprintf(d, 255, f, ap); va_end(ap); return n;
}
int fontPrint__FiiPci(int x, int y, char* t, int f) {
    ++prints; printArgs[0] = x; printArgs[1] = y; printArgs[2] = f;
    printText = t; strncpy(glyphText, t, 63); return 0;
}
void fontEnd__Fv() { ++ends; }
void textureDrawColor__FP8_textureiiiUi(void* t, int x, int y, int w, unsigned c) {
    ++draws; textureTex = t; textureArgs[0] = x; textureArgs[1] = y;
    textureArgs[2] = w; textureColor = c;
}
void ustrcpy__FPUsT0(unsigned short* d, unsigned short* s) {
    ++ustrcpys; int i = 0; do { d[i] = s[i]; } while (s[i++] && i < 255);
}
int fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short*, int f) {
    ++centeredus; centeredArgs[0] = x; centeredArgs[1] = y; centeredArgs[2] = f; return 0;
}
int netPlayerIsLocal__Fi(int i) { localArg = i; return localResult; }
int fontPrintCentered__FiiPci(int x, int y, char* t, int f) {
    ++centeredis; centeredText = t; strncpy(whoText, t, 63);
    assert(x == 176 && y == 256 && f == 10000); return 0;
}
}

static unsigned char dlgBuf[512];
static unsigned char fontBuf[8];

int main() {
    mediumFont = fontBuf;
    haloTexture7 = (void*)0x77;
    // Empty and swooping dialogs draw nothing.
    {
        reset_state();
        drawDialog__Fv();
        assert(fades == 0 && starts == 0 && draws == 0);
        reset_state();
        curDlg = (unsigned long)dlgBuf; swooping = 1;
        drawDialog__Fv();
        assert(fades == 0 && starts == 0);
    }
    // Fade rises once; alpha 64 tints the first color.
    {
        reset_state();
        curDlg = (unsigned long)dlgBuf;
        coreInterfaceFadeUpVal = 0.5f;
        textBody[0] = 'x';
        drawDialog__Fv();
        assert(fades == 1 && flaredIn == 1);
        assert(startColor == (0x40000000u | 0x3a5262u));
        assert(restrictArg == -1); // letterAtATime clear: only the reset call
        assert(scissorArgs[0] == 0 && scissorArgs[1] == 16 && scissorArgs[2] == 640 && scissorArgs[3] == 60);
        assert(wraps == 1 && wrapArgs[0] == 56 && wrapArgs[1] == 10 && wrapText == (void*)textBody);
        assert(wrapArgs[2] == 524 && wrapArgs[3] == 295 && wrapArgs[4] == 295 && wrapArgs[5] == 18);
        assert(ends == 1 && draws == 1 && textureTex == (void*)0x77);
        assert(textureArgs[0] == 35 && textureArgs[1] == 280 && textureArgs[2] == 5);
        assert(textureColor == (0x40000000u | 0x808080u));
        // Second frame: no new fade, same colors.
        drawDialog__Fv();
        assert(fades == 1);
    }
    // Flared-out alpha uses one minus the value; width follows the flag.
    {
        reset_state();
        curDlg = (unsigned long)dlgBuf;
        flaredIn = 1; flaredOut = 1; coreInterfaceFadeUpVal = 0.25f;
        DLG_ShowDialogText = 1; letterAtATime = 1; textBodyChars = 9;
        drawDialog__Fv();
        assert(fades == 0 && startColor == (0x60000000u | 0x3a5262u));
        assert(wrapArgs[4] == 385);
        assert(restricts == 2 && restrictArg == -1); // count then reset
    }
    // Options print only through the stream gates.
    {
        reset_state();
        curDlg = (unsigned long)dlgBuf;
        flaredIn = 1; textPageDone = 1; showOptions = 1; optionCount = 0;
        DLG_ShowDialogText = 1;
        drawDialog__Fv();
        assert(statusCalls == 0 && printus == 1);
        assert(printuArgs[0] == 86 && printuArgs[1] == 60 && printuArgs[2] == 0xf4240);
        assert(strcmp(textKey, "Affirmation") == 0);
        assert(loadSampleRate == 1);
    }
    {
        reset_state();
        curDlg = (unsigned long)dlgBuf;
        flaredIn = 1; textPageDone = 1; showOptions = 1; optionCount = 2;
        waitForSound = 1; cancelStreamWait = 0;
        drawDialog__Fv();
        assert(wraps == 1 && statusCalls == 0); // body only, options gated
        cancelStreamWait = 1;
        {
            int w = wraps;
            drawDialog__Fv();
            assert(wraps - w == 3); // body plus two options
        }
    }
    {
        reset_state();
        curDlg = (unsigned long)dlgBuf;
        flaredIn = 1; textPageDone = 1; showOptions = 1; optionCount = 2;
        statusResult = 0;
        drawDialog__Fv();
        assert(wraps == 3 && statusCalls == 1);
        reset_state();
        curDlg = (unsigned long)dlgBuf;
        flaredIn = 1; textPageDone = 1; showOptions = 1; optionCount = 2;
        statusResult = 1; cancelStreamWait = 0;
        drawDialog__Fv();
        assert(wraps == 1);
        cancelStreamWait = 1;
        {
            int w = wraps;
            drawDialog__Fv();
            assert(wraps - w == 3);
        }
    }
    // Options highlight the selected index and read record strings.
    {
        reset_state();
        memset(dlgBuf, 0, sizeof dlgBuf);
        curDlg = (unsigned long)dlgBuf;
        flaredIn = 1; textPageDone = 1; showOptions = 1; optionCount = 2;
        DLG_ShowDialogText = 1;
        optTbl[0] = 0; optTbl[1] = 1; optionIndex = 1;
        optionLinePos[0] = 100; optionLinePos[1] = 200;
        coreInterfaceFadeUpVal = 0.0f; // alpha 0: exact colors
        drawDialog__Fv();
        assert(wraps == 3);
        assert(wraps == 3);
        assert(wrapArgs[1] == 200 && textIn == (void*)(dlgBuf + 1 * 84 + 88));
        // Cursor block with a zero marker uses row 398 and the faint color.
        assert(printArgs[0] == 56 && printArgs[1] == 398 && printArgs[2] == 0x186a0);
        assert(colorArg == (int)(0x606060u));
        // Hermite interpolation between the two rows.
        *(int*)(dlgBuf + 84) = 5;
        hermiteResult = 0.5f;
        cursorTween = 1.0f;
        padAcceptButtonFontChar = 66;
        drawDialog__Fv();
        assert(hermites == 1);
        assert(hermiteIn[0] == 1.0f && hermiteIn[1] == 0.0f && hermiteIn[2] == 1.0f);
        assert(hermiteIn[3] == -1.0f && hermiteIn[4] == -1.0f);
        // from=200, span=100-200... optionIndex=1,nextIndex=0: 200+(100-200)*0.5=150
        assert(printArgs[1] == 150);
        assert(glyphText[0] == 'B' && glyphText[1] == 0);
    }
    // "More" prompt needs both flags; the name part needs the show flag.
    {
        reset_state();
        curDlg = (unsigned long)dlgBuf;
        flaredIn = 1; textPageDone = 1; showMore = 1;
        DLG_ShowDialogText = 1;
        showOptions = 0;
        const unsigned short nm[] = {'R', 'o', 'b', 'e', 'r', 't', 0};
        memcpy(dlgName, nm, sizeof nm);
        nameChars = 3; flaredOut = 1;
        localResult = 1;
        drawDialog__Fv();
        assert(prints == 1); // prompt only: showOptions is clear
        assert(printArgs[0] == 51 && printArgs[1] == 398);
        assert(strcmp(textKey, "dialogMore") == 0);
        assert(printuArgs[0] == 81 && printuArgs[1] == 398);
        assert(centeredus == 1 && centeredArgs[0] == 174 && centeredArgs[1] == 285);
        assert(nameChars == 2 && localArg == 0);
        // Prompt skipped without showMore. Negative counts are unreachable:
        // the reveal only decrements positive counts by one, so the clamp
        // below can never go negative through this path.
        reset_stubs();
        showMore = 0;
        drawDialog__Fv();
        assert(prints == 0);
    }
    // Remote players get the "(name)" line.
    {
        reset_state();
        curDlg = (unsigned long)dlgBuf;
        flaredIn = 1; DLG_ShowDialogText = 1;
        nameChars = 0;
        localResult = 0; activePlayer = 1;
        strcpy((char*)player + 23984, "Zed");
        drawDialog__Fv();
        assert(centeredis == 1 && strcmp(whoText, "(Zed)") == 0);
        assert(ends == 3); // body, name, and remote blocks
    }
    return 0;
}
