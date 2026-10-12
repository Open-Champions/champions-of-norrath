// Observed startPacket__Fi at 0x001d34d0, 720 bytes (dialog packet number n).
// Packets are 1096 bytes each in the array whose address is the word dlgArrayPtr.
// Pointer variables are 32-bit words on the target. In order:
//  1. cursorTween = 1.0f, realDlgPos = -1, curDlgStart = curDlg = packet n,
//     dlgPos = n, letterAtATime = 1, and these words are set to zero:
//     textPageDone, textBodyChars, cancelStreamWait, showOptions, showMore,
//     optionIndex, nextIndex, scrollY, scrollTarget, lastYAnchor. Then
//     soundResetStream__Fi(1) is called with waitForSound (a byte) set to 0 in
//     its delay slot.
//  2. If dialogCreature is nonzero: when the signed byte at creature+292 is at
//     least 2 it is set to 1; if the word at creature+296 differs from
//     dialogIdleAnimation, creature byte 292 is cleared and animAdd is called
//     with (creature + 288, dialogIdleAnimation, 1).
//  3. The 16-bit string returned by textGetString__FPc(curDlg + 33) is copied
//     into textBody (an array of 16-bit values) after two leading spaces
//     (textBodyLen starts at 2); the character 94 ('^') is stored as 10. A zero
//     terminator follows, then playSoundForDlg__FP9DlgPacket(curDlgStart).
//  4. optionCount = 0. For i = 0 .. curDlg[84]-1 (word, rechecked every pass)
//     the 84-byte option records start at curDlg + 104. An option counts when
//     its string is empty or scriptStart__FPc(string) is nonzero; its index is
//     appended to optTbl.
//  5. fontStart(mediumFont, 1, 0x80323c3c, -1); height = fontPrintWrapu(56, 0,
//     textBody, 524, 0, 0, 0, 18); for each counted option k of n, optionLinePos[k]
//     = 416 - 18 * (n - k); fontEnd(); scrollEnd = max(0, height + 18 * n - 90).
// What the numbers mean is not established. The three arrays textBody (2048
// bytes), optionLinePos and optTbl are named in the image symbol table.
extern "C" {
extern unsigned dlgArrayPtr;
extern unsigned curDlg;
extern unsigned curDlgStart;
extern int dlgPos;
extern int realDlgPos;
extern float cursorTween;
extern int letterAtATime;
extern int textPageDone;
extern int textBodyChars;
extern int cancelStreamWait;
extern int showOptions;
extern int showMore;
extern int optionIndex;
extern int nextIndex;
extern int scrollY;
extern int scrollTarget;
extern int lastYAnchor;
extern int scrollEnd;
extern int textBodyLen;
extern int optionCount;
extern unsigned char waitForSound;
extern unsigned dialogCreature;
extern unsigned dialogIdleAnimation;
extern unsigned mediumFont;
extern unsigned short textBody[];
extern int optTbl[];
extern int optionLinePos[];

void soundResetStream__Fi(int value);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flag);
unsigned short* textGetString__FPc(char* name);
void playSoundForDlg__FP9DlgPacket(void* packet);
int strcmp(const char* a, const char* b);
int scriptStart__FPc(char* name);
void fontStart__FP4FontiUii(void* font, int a, unsigned color, int b);
int fontPrintWrapu__FiiPUsiiii(int x, int y, unsigned short* text, int width, int a, int b, int c);
void fontEnd__Fv();
}

static inline unsigned char* Ptr(unsigned word) { return (unsigned char*)(unsigned long)word; }

extern "C" void startPacket__Fi(int n) {
    cursorTween = 1.0f;
    realDlgPos = -1;
    curDlgStart = dlgArrayPtr + n * 1096;
    letterAtATime = 1;
    textPageDone = 0;
    textBodyChars = 0;
    cancelStreamWait = 0;
    showOptions = 0;
    showMore = 0;
    optionIndex = 0;
    nextIndex = 0;
    scrollY = 0;
    scrollTarget = 0;
    lastYAnchor = 0;
    dlgPos = n;
    curDlg = curDlgStart;
    waitForSound = 0;
    soundResetStream__Fi(1);

    if (dialogCreature != 0) {
        unsigned char* creature = Ptr(dialogCreature);
        if ((signed char)creature[292] >= 2) creature[292] = 1;
        creature = Ptr(dialogCreature);
        if (*(unsigned*)(creature + 296) != dialogIdleAnimation) {
            creature[292] = 0;
            animAdd__FP14AnimationStateP15AnimationHeaderi(creature + 288, Ptr(dialogIdleAnimation), 1);
        }
    }

    unsigned short* text = textGetString__FPc((char*)Ptr(curDlg) + 33);
    textBodyLen = 2;
    textBody[0] = 32;
    textBody[1] = 32;
    for (; *text != 0; ++text) {
        textBody[textBodyLen++] = (*text == 94) ? 10 : *text;
    }
    textBody[textBodyLen] = 0;
    playSoundForDlg__FP9DlgPacket(Ptr(curDlgStart));

    optionCount = 0;
    for (int i = 0; i < *(int*)(Ptr(curDlg) + 84); ++i) {
        bool counted = strcmp("", (char*)Ptr(curDlg) + i * 84 + 104) == 0;
        if (!counted) counted = scriptStart__FPc((char*)Ptr(curDlg) + i * 84 + 104) != 0;
        if (counted) {
            int slot = optionCount;
            optionCount = slot + 1;
            optTbl[slot] = i;
        }
    }

    fontStart__FP4FontiUii(Ptr(mediumFont), 1, 0x80323c3cu, -1);
    int height = fontPrintWrapu__FiiPUsiiii(56, 0, textBody, 524, 0, 0, 18);
    int count = optionCount;
    int span = 0;
    if (count > 0) {
        for (int k = 0; k < count; ++k) {
            optionLinePos[k] = 416 - (count - k) * 18;
            span += 18;
        }
    }
    fontEnd__Fv();
    int end = height + span - 90;
    if (end < 0) end = 0;
    scrollEnd = end;
}
