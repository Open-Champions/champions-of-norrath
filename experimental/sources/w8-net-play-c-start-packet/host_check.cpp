#include <cstdio>
#include <cstring>
#include <string>
#include <sys/mman.h>
#define strcmp unit_strcmp
#include "start_packet.cpp"
#undef strcmp
// x86_64 build with -Wl,-pagezero_size,0x10000: dialog pointers are 32-bit words on
// the target, so the packets and the creature live at 0x20000000 here.
unsigned dlgArrayPtr, curDlg, curDlgStart, dialogCreature, dialogIdleAnimation, mediumFont;
int dlgPos, realDlgPos, letterAtATime, textPageDone, textBodyChars, cancelStreamWait, showOptions, showMore;
int optionIndex, nextIndex, scrollY, scrollTarget, lastYAnchor, scrollEnd, textBodyLen, optionCount;
float cursorTween;
unsigned char waitForSound;
unsigned short textBody[1024];
int optTbl[64];
int optionLinePos[64];

static std::string trace;
static unsigned short script_text[256];
static int font_height;
extern "C" int unit_strcmp(const char* a, const char* b) { return strcmp(a, b); }
extern "C" void soundResetStream__Fi(int v) { trace += "reset(" + std::to_string(v) + ",wait=" + std::to_string(waitForSound) + ") "; }
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(void* s, void* h, int f) {
    trace += "animAdd(+" + std::to_string((long)((unsigned char*)s - (unsigned char*)0x20004000)) + "," + std::to_string((unsigned long)h) + "," + std::to_string(f) + ") ";
}
extern "C" unsigned short* textGetString__FPc(char* name) {
    trace += "text(" + std::to_string((long)(name - (char*)(unsigned long)curDlg)) + ") ";
    return script_text;
}
extern "C" void playSoundForDlg__FP9DlgPacket(void* p) {
    trace += "snd(" + std::to_string(((unsigned long)p - 0x20000000) / 1096) + ",len=" + std::to_string(textBodyLen) + ",term=" + std::to_string(textBody[textBodyLen]) + ") ";
}
extern "C" int scriptStart__FPc(char* name) { trace += std::string("script(") + name + ") "; return name[0] == 'y'; }
extern "C" void fontStart__FP4FontiUii(void* f, int a, unsigned c, int b) { trace += "fontStart(" + std::to_string((unsigned long)f) + "," + std::to_string(a) + "," + std::to_string(c) + "," + std::to_string(b) + ") "; }
extern "C" int fontPrintWrapu__FiiPUsiiii(int x, int y, unsigned short* t, int w, int a, int b, int c) {
    trace += "print(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(t == textBody) + "," + std::to_string(w) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ") ";
    return font_height;
}
extern "C" void fontEnd__Fv() { trace += "fontEnd "; }

static int fails;
#define EXPECT(cond, tag) do { if (!(cond)) { printf("FAIL %s (trace: %s)\n", tag, trace.c_str()); ++fails; } } while (0)
static void settext(const char* s) { size_t n = strlen(s); for (size_t i = 0; i < n; ++i) script_text[i] = (unsigned char)s[i]; script_text[n] = 0; }
static void setopts(unsigned char* pkt, int n, const char* const* names) {
    memcpy(pkt + 84, &n, 4);
    for (int i = 0; i < n; ++i) strcpy((char*)pkt + 104 + 84 * i, names[i]);
}
int main() {
    unsigned char* heap = (unsigned char*)mmap((void*)0x20000000, 1 << 16, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON | MAP_FIXED, -1, 0);
    if ((unsigned long)heap != 0x20000000) return 100;
    dlgArrayPtr = 0x20000000;
    mediumFont = 0x1234;
    // 1: no creature, text with a caret, three options (one empty, one scripted, one rejected)
    {
        memset(heap, 0, 3 * 1096); trace.clear();
        const char* names[3] = {"", "yes", "no"};
        setopts(heap + 1096, 3, names);
        settext("ab^cd"); font_height = 100;
        memset(textBody, 0xff, sizeof textBody); memset(optTbl, 0xee, sizeof optTbl); memset(optionLinePos, 0xee, sizeof optionLinePos);
        waitForSound = 7; cursorTween = 0; scrollY = 5; optionIndex = 4; dialogCreature = 0;
        startPacket__Fi(1);
        EXPECT(curDlg == 0x20000000u + 1096 && curDlgStart == curDlg && dlgPos == 1 && realDlgPos == -1 && letterAtATime == 1, "globals");
        EXPECT(cursorTween == 1.0f && textPageDone == 0 && textBodyChars == 0 && cancelStreamWait == 0 && showOptions == 0 && showMore == 0, "zeros");
        EXPECT(optionIndex == 0 && nextIndex == 0 && scrollY == 0 && scrollTarget == 0 && lastYAnchor == 0 && waitForSound == 0, "zeros2");
        const unsigned short want[] = {32, 32, 'a', 'b', 10, 'c', 'd', 0};
        EXPECT(memcmp(textBody, want, sizeof want) == 0 && textBodyLen == 7, "text");
        EXPECT(optionCount == 2 && optTbl[0] == 0 && optTbl[1] == 1, "options");
        EXPECT(optionLinePos[0] == 416 - 36 && optionLinePos[1] == 416 - 18, "linepos");
        EXPECT(scrollEnd == 100 + 36 - 90, "scrollEnd");
        EXPECT(trace == "reset(1,wait=0) text(33) snd(1,len=7,term=0) script(yes) script(no) fontStart(4660,1,2150775868,-1) print(56,0,1,524,0,0,18) fontEnd ", "trace");
    }
    // 2: empty text, no options, small height clamps to zero; creature handling
    for (int variant = 0; variant < 4; ++variant) {
        memset(heap, 0, 0x5000); trace.clear();
        int zero = 0; memcpy(heap + 84, &zero, 4);
        settext(""); font_height = 20; scrollEnd = 999;
        dialogCreature = 0x20004000; dialogIdleAnimation = 0x55;
        unsigned char* cr = heap + 0x4000;
        cr[292] = (variant == 0) ? 0 : (variant == 1) ? 1 : (variant == 2) ? 2 : 9;
        unsigned anim = (variant == 3) ? 0x55 : 0x66; memcpy(cr + 296, &anim, 4);
        startPacket__Fi(0);
        EXPECT(textBodyLen == 2 && textBody[0] == 32 && textBody[1] == 32 && textBody[2] == 0 && optionCount == 0 && scrollEnd == 0, "empty");
        bool played = (variant != 3);
        EXPECT(trace.find("animAdd(") != std::string::npos == played, "anim-played");
        if (played) {
            EXPECT(cr[292] == 0 && trace.find("animAdd(+288,85,1)") != std::string::npos, "anim-args");  // 0x55 == 85
            // the second field is the header pointer, i.e. dialogIdleAnimation
        } else {
            EXPECT(cr[292] == 1, "kept-one");  // byte 9 >= 2 is first clamped to 1
        }
    }
    // byte >= 2 with matching animation: clamped to 1
    {
        memset(heap, 0, 0x5000); trace.clear(); settext("x"); font_height = 0;
        unsigned char* cr = heap + 0x4000; dialogCreature = 0x20004000; dialogIdleAnimation = 0x77; cr[292] = 0x7f; unsigned a = 0x77; memcpy(cr + 296, &a, 4);
        startPacket__Fi(0); EXPECT(cr[292] == 1, "clamp");
        cr[292] = (unsigned char)-5; startPacket__Fi(0); EXPECT(cr[292] == (unsigned char)-5, "negative-byte-kept");
    }
    // scrollEnd with options: height 300, 5 options all accepted
    {
        memset(heap, 0, 0x5000); trace.clear(); dialogCreature = 0;
        const char* names[5] = {"", "", "", "", ""}; setopts(heap, 5, names); settext("t"); font_height = 300;
        startPacket__Fi(0);
        EXPECT(optionCount == 5 && scrollEnd == 300 + 90 - 90, "five");
        for (int k = 0; k < 5; ++k) EXPECT(optionLinePos[k] == 416 - (5 - k) * 18 && optTbl[k] == k, "five-pos");
    }
    printf(fails ? "FAILED\n" : "");
    return fails ? 1 : 0;
}
