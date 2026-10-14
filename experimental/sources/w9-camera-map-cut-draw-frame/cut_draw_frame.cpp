// Observed cutDrawFrame__Fv at 0x0018c378, 1408 bytes. Nothing here was
// executed on the EE. "record(n)" below is dialogPacket + 1096 * n + 33, the
// text key address passed to textGetString__FPc for dialog position n - 1
// (the code subtracts 1063 from dialogPacket + 1096 * displayDialogPos).
// 1. Returns at once when the signed byte coreLoadingScreenRunning is nonzero.
// 2. When frontEndRunning is zero: engineScissory1 = 58 - engineScreenSlide and
//    engineScissory2 = 389 - engineScreenSlide. Then resetScissor__Fv(). When
//    the byte cutInit is zero, returns (without the letterbox call below).
// 3. Long-text block, entered only when displayFrame >= 2, musicSize > 40000,
//    strcmp("actintro", levelName + 81) is zero, dialogPacket is nonzero and
//    1 <= displayDialogPos <= nmDialogNodes: fontStart(smallFont, 5,
//    0x805a6e83, -1); text = textGetString(record); r = fontPrintWrapCenteredu(
//    320, 473 - (displayFrame >> 1), text, 620, -100, -100, 33); rate =
//    (float)(342000 + 855 * r) / (float)musicSize; text = textGetString(record)
//    again; fontPrintWrapCenteredu(320, 428 - (int)((float)displayFrame * rate),
//    text, 620, -100, 580, 33); fontEnd(); displayTextDisable = 1.
// 4. Subtitle block, entered only when subtitlesOn and dialogPacket are
//    nonzero, displayTextDisable is zero, 1 <= displayDialogPos <=
//    nmDialogNodes, and the level-name comparison above is not equal:
//    fontStart(smallFont, 7, 0x805a5a5a, -1); r = fontPrintWrapCenteredu(320,
//    390, textGetString(record), 620, -100, -90, 18); extra = r - 11. When
//    extra < 43 the text is printed with fontPrintWrapCenteredu(320, 388, text,
//    620, -100, 580, 18). Otherwise, with t = (float)displayFrame * (1/60),
//    and `action` the array of 20-byte records (+0 float time, +4 and +6
//    halfwords): idx = displayActionPos advanced while idx < nmActions and
//    action[idx].time < t. prev = the nearest record at or below idx (walking
//    down) whose halfword at +4 is -99, halfword at +6 is 0, and time <= t.
//    j = displayActionPos moved down while j > 0 and t + 0.001 <= action[j].time.
//    next = the first record from j upward (when j < nmActions) with halfword
//    +4 = -99, halfword +6 = 0 and time >= t + 0.001. end = next.time, or
//    action[nmActions - 1].time when there is none. With p = prev.time, span =
//    end - p, elapsed = t - p, margin = (span - 0.5) * 0.5: offset = 0 when
//    elapsed < margin, else (int)(clamp((elapsed - margin) / (span - margin -
//    margin), 0, 1) * (float)(extra - 43)). Then fontScissor(0, 377, 640, 433)
//    and fontPrintWrapCenteredu(320, 388 - offset, text, 620, 370, 442, 18).
//    fontEnd() ends the subtitle block.
// 5. When frontEndRunning is zero, hudLetterBox__Fv().
// The 1/60 and 0.001 are the file words 0x3c888888 and 0x3a83126e. Unknown: the
// contents of the dialog record, what the -99 halfword marks, and what the
// original does when no previous record is found (it reads a float through the
// null pointer; 0.0 is used here instead and the case is not exercised).
// Argument types are those of the mangled names; the colour values are
// unsigned words.
extern "C" {
void resetScissor__Fv();
int strcmp(const char* a, const char* b);
void fontStart__FP4FontiUii(void* font, int style, unsigned color, int extra);
unsigned short* textGetString__FPc(char* key);
int fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* text, int width, int a, int b, int c);
void fontEnd__Fv();
void fontScissor__Fiiii(int a, int b, int c, int d);
void hudLetterBox__Fv();
extern signed char coreLoadingScreenRunning;
extern int frontEndRunning;
extern int engineScissory1;
extern int engineScissory2;
extern int engineScreenSlide;
extern unsigned char cutInit;
extern int displayFrame;
extern int musicSize;
extern char levelName[];
extern unsigned char* dialogPacket;
extern int displayDialogPos;
extern int nmDialogNodes;
extern void* smallFont;
extern int displayTextDisable;
extern int subtitlesOn;
extern int displayActionPos;
extern int nmActions;
extern unsigned char* action;
}

typedef float __attribute__((may_alias)) AliasedFloat;
typedef short __attribute__((may_alias)) AliasedShort;

static inline unsigned short* recordText() {
    return textGetString__FPc(reinterpret_cast<char*>(dialogPacket + 1096 * displayDialogPos - 1063));
}
static inline float timeOf(const unsigned char* record) { return *reinterpret_cast<const AliasedFloat*>(record); }
static inline bool isMarker(const unsigned char* record) {
    return *reinterpret_cast<const AliasedShort*>(record + 4) == -99 && *reinterpret_cast<const AliasedShort*>(record + 6) == 0;
}

extern "C" void cutDrawFrame__Fv() {
    if (coreLoadingScreenRunning != 0) return;
    if (frontEndRunning == 0) {
        engineScissory1 = 58 - engineScreenSlide;
        engineScissory2 = 389 - engineScreenSlide;
    }
    resetScissor__Fv();
    if (cutInit == 0) return;

    if (displayFrame >= 2 && musicSize > 40000 && strcmp("actintro", levelName + 81) == 0 && dialogPacket != nullptr &&
        displayDialogPos > 0 && displayDialogPos - 1 < nmDialogNodes) {
        fontStart__FP4FontiUii(smallFont, 5, 0x805a6e83u, -1);
        const int r = fontPrintWrapCenteredu__FiiPUsiiii(320, 473 - (displayFrame >> 1), recordText(), 620, -100, -100, 33);
        const float rate = static_cast<float>(342000 + r * 855) / static_cast<float>(musicSize);
        unsigned short* text = recordText();
        fontPrintWrapCenteredu__FiiPUsiiii(320, 428 - static_cast<int>(static_cast<float>(displayFrame) * rate), text, 620, -100, 580, 33);
        fontEnd__Fv();
        displayTextDisable = 1;
    }

    if (subtitlesOn != 0 && dialogPacket != nullptr && displayTextDisable == 0 && displayDialogPos > 0 &&
        displayDialogPos - 1 < nmDialogNodes && strcmp("actintro", levelName + 81) != 0) {
        fontStart__FP4FontiUii(smallFont, 7, 0x805a5a5au, -1);
        const int r = fontPrintWrapCenteredu__FiiPUsiiii(320, 390, recordText(), 620, -100, -90, 18);
        const int extra = r - 11;
        if (extra < 43) {
            fontPrintWrapCenteredu__FiiPUsiiii(320, 388, recordText(), 620, -100, 580, 18);
        } else {
            const float t = static_cast<float>(displayFrame) * 0x1.111110p-6f;
            const int count = nmActions;
            int idx = displayActionPos;
            if (idx < count) {
                while (idx < count && timeOf(action + 20 * idx) < t) ++idx;
            }
            const unsigned char* prev = nullptr;
            for (int i = idx; i >= 0; --i) {
                const unsigned char* record = action + 20 * i;
                if (!isMarker(record)) continue;
                prev = record;
                if (timeOf(record) <= t) break;
                prev = nullptr;
            }
            const float prevTime = prev != nullptr ? timeOf(prev) : 0.0f;
            const float later = t + 0x1.0624dcp-10f;
            int j = displayActionPos;
            while (j > 0 && later <= timeOf(action + 20 * j)) --j;
            const unsigned char* next = nullptr;
            for (int i = j; i < count; ++i) {
                const unsigned char* record = action + 20 * i;
                if (!isMarker(record)) continue;
                if (later <= timeOf(record)) {
                    next = record;
                    break;
                }
            }
            const float end = next != nullptr ? timeOf(next) : timeOf(action + 20 * (count - 1));
            const float span = end - prevTime;
            const float elapsed = t - prevTime;
            const float margin = (span - 0.5f) * 0.5f;
            int offset = 0;
            if (!(elapsed < margin)) {
                float progress = (elapsed - margin) / ((span - margin) - margin);
                if (progress < 0.0f) progress = 0.0f;
                if (progress > 1.0f) progress = 1.0f;
                offset = static_cast<int>(progress * static_cast<float>(extra - 43));
            }
            fontScissor__Fiiii(0, 377, 640, 433);
            fontPrintWrapCenteredu__FiiPUsiiii(320, 388 - offset, recordText(), 620, 370, 442, 18);
        }
        fontEnd__Fv();
    }
    if (frontEndRunning == 0) hudLetterBox__Fv();
}
