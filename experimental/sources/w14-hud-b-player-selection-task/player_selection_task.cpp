// Observed playerSelectionTask__Fv at 0x0027a658, 12,200 bytes, 960-byte frame. A per-frame task of the
// character selection screens. It takes no arguments and returns nothing. It keeps its state in the file-local
// globals of the original unit (selectMode, newMode, curPlayer, ..., declared below with their image names).
// Records used: playerSelectInfo (41-byte records, four of them; byte +32 is the chosen character, +33..+36
// hair style, hair colour, skin, beard bytes, +37..+40 four stat bytes), targets (636-byte records, ten of
// them: word +0 a file name stem, +4 and +12 text keys, bytes +17 class row, +19, +24..+27 option counts),
// targetObjs (ten object pointers; the animation state is at object + 288, a halfword at +294 holds flags
// 0x80 / 0x10 / 0x8, a signed byte at +292).
// Every frame:
//  1. throb = (sinf(((frameNm & 63) * c) * 2 * pi_like) + 1) * 0.5; throbR = round(throb * 46) + 109, throbG =
//     round(throb * 43) + 110, throbB = 104, throbA = (int)(flareAlpha * 95). Returns when coreLoading is set or the
//     camera mode is not 0.
//  2. For each of the ten targets with an object whose flags have 0x80 and signed byte < 2, the idle animation
//     "<name>_intro3.anm" (flag 0x8) or "<name>_intro1.anm" is looked up in npcLumpName and blended in.
//  3. moveCamera and npcLookAtCamera; on the first call the fade is started and the task waits while textures load.
//  4. Fades: with flareDelta zero, bodyAlpha rises by 0.109 (or falls by 0.131 when hermiteTime is nonzero or
//     lockAlpha is set), flareAlpha follows bodyAlpha and headerAlpha rises; otherwise flareAlpha moves by
//     flareDelta times 0.99999994, limited to 0..1.
//  5. A nine-way switch on selectMode: 0 start, 1 character choice (requestToExit asks the "previous player"
//     question), 2 custom look (skin, hair style, hair colour, beard sliders), 3 stat allocation (and its warning),
//     4 name pad, 5 leaving fade, 6 "next player" prompt, 7 advance to the next player, 8 return to the name pad.
//     Each case's body is described next to it below.
//  6. Unless selectMode is 6 or 7 a bordered backdrop is drawn from bodyAlpha, and when flareAlpha is nonzero the
//     prompt line for the current mode ("SelectPrompt*") with its own border.
// Inferred: the screen flow of choosing the character, its look and its stats for each local player. Unknown:
// the record field names, what most of the numeric constants (colours, positions, the 0.077-style gp floats) are
// exactly, and the meaning of the values hudRunSlider, changeSkin and friends return.
typedef unsigned short UString;

// The image's sprintf keeps its own symbol; the host check supplies formatText under its plain name.
#if defined(__mips__)
#define LIBRARY_SYMBOL(name) __asm__(#name)
#else
#define LIBRARY_SYMBOL(name)
#endif

extern "C" {
// file-local state of the original unit, named as in the image's symbol table
extern int curPlayer;
extern int charDelta;
extern int selectMode;
extern int newMode;
extern int lockAlpha;
extern int requestToExit;
extern int statAllocation;
extern int msgAlpha;
extern int msgAlphaDelta;
extern unsigned char curStat;
extern unsigned char statAllocWarn;
extern unsigned char startExitFade;
extern signed char curSlider;
extern signed char lastSlider;
extern signed char maxSlider;
extern float hermiteTime;
extern float delta;
extern float flareAlpha __asm__("norrath_player_select_view_486d3c");   // the image has three objects named flareAlpha
extern float hermDeltaScale;
extern float flareDelta __asm__("norrath_player_select_view_486d44");   // and two named flareDelta
extern float headerAlpha;
extern float bodyAlpha;
extern float otherHermiteTime;
extern float throb;
extern int throbR;
extern int throbG;
extern int throbB;
extern int throbA;
extern char* npcLumpName;
extern signed char firstTime __asm__("norrath_player_select_view_486d88");   // and two named firstTime
extern signed char playerSelectCount;
extern signed char playerSelectStartPlayer;
extern UString* playerSelectExitMessage;
// shared state
extern int frameNm;
extern int coreLoading;
extern void* gameCam;
extern void* smallFont;
extern signed char language;
extern unsigned padAcceptButton;
extern unsigned padBackButton;
extern int padAcceptButtonFontChar;
extern int padBackButtonFontChar;
extern unsigned char pad[];
extern unsigned char targets[];
extern unsigned char playerSelectInfo[];
extern unsigned char targetObjs[];
extern unsigned char abilityTable[];
extern unsigned char hairStyleSetting[];
extern unsigned char hairColorSetting[];
extern unsigned char beardSetting[];
extern unsigned char skinSetting[];
extern unsigned char playerSelectSoundLump[];
extern unsigned statTable[];
extern UString hudFooterUni[];

float sinf(float value);
int GetMode__6Camera(void* camera);
void moveCamera__Fv();
void npcLookAtCamera__Fv();
int texIsLoading__Fv();
int hudFadeScreen__Fi(int amount);
void* lumpFindResource__FPcT0(const char* lump, const char* name);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float blend, int mode);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* animation, void* object, float volume);
void soundPlaySound__FP10_vagHeaderP6Point3fff(void* sound, void* position, float a, float b, float c);
int formatText(char* buffer, const char* format, ...) LIBRARY_SYMBOL(sprintf);
UString* textGetString__FPc(const char* key);
void usprintf__FPUsT0ffffffff(UString* destination, UString* format, float a, float b, float c, float d, float e, float f, float g, float h);
void usprintf__FPUsPcffffffff(UString* destination, const char* format, float a, float b, float c, float d, float e, float f, float g, float h);
void ustrcpy__FPUsT0(UString* destination, UString* source);
void hudShowMsg__FPUsN20iii(UString* title, UString* body, UString* footer, int alpha, int a, int b);
int hudRunYesNo__FPUsiiii(UString* text, int a, int b, int c, int d);
void allDone__Fi(int value);
void initPosition__Fii(int current, int target);
void engineSetBlur__Fi(int value);
void resetName__Fv();
void switchToNamePad__Fv();
int doNamePad__Fv();
void drawNamePad__Fv();
void initPreSelect__Fv();
void initPostSelect__Fv();
void hudResetFadeScreen__Fi(int value);
float evalHermite__Ffffff(float a, float b, float c, float d, float e);
void boxDraw__FiiiiiUii(int a, int b, int c, int d, int e, unsigned color, int f);
int hudRunSlider__FiiiiiUifPii(int x, int y, int width, int a, int b, unsigned color, float fraction, int* value, int player);
int changeSkin__Fii(int character, int value);
int changeHairStyle__Fii(int character, int value);
int changeHairColor__Fii(int character, int value);
int changeBeard__Fii(int character, int value);
int getHairStyle__Fi(int character);
int gameButtonToIcon__Fi(int button);
void fontStart__FP4FontiUii(void* font, int a, unsigned color, int b);
void fontEnd__Fv();
void fontColor__FUiii(unsigned color, int a, int b);
void fontPrintu__FiiPUsi(int x, int y, UString* text, int width);
void fontPrintCenteredu__FiiPUsi(int x, int y, UString* text, int width);
int fontPrintWrapu__FiiPUsiiii(int x, int y, UString* text, int a, int b, int c, int d);
int fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, UString* text, int a, int b, int c, int d);
int fontStringSizeu__FP4FontPUsi(void* font, UString* text, int width);
void textureSmoothBorder__FiiiiiUiiii(int x, int y, int w, int h, int a, unsigned color, int b, int c, int d);
}

namespace {
constexpr float bits(unsigned word) { return __builtin_bit_cast(float, word); }
// unnamed gp-relative single-precision words of the image, named by their gp offset
constexpr float g22356 = bits(0x3c820820u);
constexpr float g22352 = bits(0x40490fdau);
constexpr float g22348 = bits(0x3e4ccccc);
constexpr float g22344 = bits(0x3ddf3b63u);
constexpr float g22340 = bits(0x3e062280u);
constexpr float g22336 = bits(0x3ddf3b63u);
constexpr float g22332 = bits(0x3f7fffffu);
constexpr float g22328 = bits(0x3e15818cu);
constexpr float g22324 = bits(0x3d5d2f1au);
constexpr float g22320 = bits(0x3e4ccccc);
constexpr float g22316 = bits(0x3d5d2f1au);
constexpr float g22312 = bits(0x3d5d2f1au);
constexpr float g22308 = bits(0xbd9db22du);
constexpr float g22304 = bits(0x3e4ccccc);
constexpr float g22300 = bits(0x3d9db22du);
constexpr float g22296 = bits(0x3d9db22du);
constexpr float g22292 = bits(0x3dcccccbu);
constexpr float g22288 = bits(0xbd9db22du);
constexpr float g22284 = bits(0xbd9db22du);
constexpr float g22280 = bits(0x3e999999u);
constexpr float g22276 = bits(0x3d9db22du);
constexpr float g22272 = bits(0xbd9db22du);
constexpr float g22268 = bits(0xbd9db22du);
constexpr float g22264 = bits(0x3d9db22du);
constexpr float g22260 = bits(0x3e999999u);
constexpr float g22256 = bits(0xbd9db22du);
constexpr float g22252 = bits(0x3d9db22du);
constexpr float g22248 = bits(0xbd9db22du);
constexpr float g22244 = bits(0x3e4ccccc);
constexpr float g22240 = bits(0x3e15818cu);
constexpr float g22236 = bits(0x3e15818cu);

inline unsigned char* infoOf(int player) { return playerSelectInfo + 41 * player; }
inline unsigned char* targetOf(int character) { return targets + 636 * character; }
inline void* pointerAt(unsigned char* base, int offset) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(base + offset)));
}
inline char* textAt(unsigned char* base, int offset) { return static_cast<char*>(pointerAt(base, offset)); }
inline int& intAt(unsigned char* base, int offset) { return *reinterpret_cast<int*>(base + offset); }
inline void* objectOf(int character) { return pointerAt(targetObjs, 4 * character); }
inline unsigned toUnsigned(float value) { return static_cast<unsigned>(value); }
inline int roundHalfAway(float value) { return static_cast<int>(value >= 0.0f ? value + 0.5f : value - 0.5f); }
inline int paneWidth() { return language == 0 ? 269 : 334; }
inline int paneMargin() { return language == 0 ? 9 : 4; }
inline unsigned padWord(int slot, int offset) { return *reinterpret_cast<unsigned*>(pad + 88 * slot + offset); }

inline void playMenuSound(const char* name) {
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", name), nullptr, 1.0f);
}

// Blends a character's object into the animation "<stem><suffix>" of the npc lump, if the lump has it.
inline void transitionTo(int character, const char* format, float blend, int mode) {
    char name[128];
    formatText(name, format, textAt(targetOf(character), 0));
    void* header = lumpFindResource__FPcT0(npcLumpName, name);
    if (header != nullptr) animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(static_cast<unsigned char*>(objectOf(character)) + 288, header, blend, mode);
}

// The fade-state update of modes 2, 3, 4 and 6 while flareDelta is nonzero. `step` is the value of flareDelta read
// before the frame's fade computation. When a fade out finishes (negative step, flareAlpha at zero) the mode
// becomes newMode and flareDelta becomes `arrival`.
inline void finishFade(float step, float arrival) {
    const float alpha = flareAlpha;
    if (alpha == 1.0f) {
        step = 0.0f;
        flareDelta = step;
    }
    if (!(step < 0.0f)) {
        bodyAlpha = alpha;
    } else if (alpha != 0.0f) {
        bodyAlpha = alpha;
    } else {
        selectMode = newMode;
        flareDelta = arrival;
        bodyAlpha = alpha;
    }
}

inline unsigned alphaColor(float scale, unsigned rgb) { return (toUnsigned(flareAlpha * scale) << 24) | rgb; }

inline void zeroFormat(UString* destination, UString* format, float a) {
    usprintf__FPUsT0ffffffff(destination, format, a, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}
inline void zeroFormatC(UString* destination, const char* format, float a) {
    usprintf__FPUsPcffffffff(destination, format, a, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}


// Mode 1 backdrop text: the character's class, description, strengths and weaknesses, measured and then drawn.
void drawCharacterInfo() {
    char name[128];
    fontStart__FP4FontiUii(smallFont, 6, (toUnsigned(bodyAlpha * 99.0f) << 24) | 0x316864u, -1);
    const int wrap = paneWidth() - paneMargin() - 41;
    const int classHeight = fontPrintWrapu__FiiPUsiiii(
        0, 0, textGetString__FPc(textAt(targetOf(static_cast<signed char>(infoOf(curPlayer)[32])), 4)), wrap, -100, -100, 17);
    formatText(name, "%s_description", textAt(targetOf(static_cast<signed char>(infoOf(curPlayer)[32])), 0));
    const int descriptionHeight = fontPrintWrapu__FiiPUsiiii(0, 0, textGetString__FPc(name), paneWidth() - paneMargin() - 41, -100, -100, 17);
    formatText(name, "%s_strengths", textAt(targetOf(static_cast<signed char>(infoOf(curPlayer)[32])), 0));
    const int strengthsHeight = fontPrintWrapu__FiiPUsiiii(0, 0, textGetString__FPc(name), paneWidth() - paneMargin() - 41, -100, -100, 17);
    fontPrintu__FiiPUsi(41, 40, textGetString__FPc("selectClass"), 1000000);
    fontPrintu__FiiPUsi(41, classHeight + 59, textGetString__FPc("selectSex"), 1000000);
    fontPrintu__FiiPUsi(41, classHeight + 93, textGetString__FPc("selectDescription"), 1000000);
    fontPrintu__FiiPUsi(41, classHeight + 40 + descriptionHeight + 72, textGetString__FPc("selectStrengths"), 1000000);
    fontPrintu__FiiPUsi(41, classHeight + 40 + (strengthsHeight + 89) + descriptionHeight + 2, textGetString__FPc("selectWeaknesses"), 1000000);
    fontEnd__Fv();

    fontStart__FP4FontiUii(smallFont, 6, (toUnsigned(bodyAlpha * 79.0f) << 24) | 0x585e5du, -1);
    const int x = paneMargin() + 41;
    fontPrintWrapu__FiiPUsiiii(x, 57, textGetString__FPc(textAt(targetOf(static_cast<signed char>(infoOf(curPlayer)[32])), 4)),
                               paneWidth() - paneMargin() - 41, -50, 600, 17);
    fontPrintu__FiiPUsi(x, classHeight + 76, textGetString__FPc(textAt(targetOf(static_cast<signed char>(infoOf(curPlayer)[32])), 12)), 1000000);
    formatText(name, "%s_description", textAt(targetOf(static_cast<signed char>(infoOf(curPlayer)[32])), 0));
    fontPrintWrapu__FiiPUsiiii(x, classHeight + 110, textGetString__FPc(name), paneWidth() - paneMargin() - 41, -100, 580, 17);
    formatText(name, "%s_strengths", textAt(targetOf(static_cast<signed char>(infoOf(curPlayer)[32])), 0));
    fontPrintWrapu__FiiPUsiiii(x, classHeight + 40 + descriptionHeight + 89, textGetString__FPc(name), paneWidth() - paneMargin() - 41, -100, 580, 17);
    formatText(name, "%s_weaknesses", textAt(targetOf(static_cast<signed char>(infoOf(curPlayer)[32])), 0));
    fontPrintWrapu__FiiPUsiiii(x, classHeight + 40 + (strengthsHeight + 106) + descriptionHeight + 2, textGetString__FPc(name),
                               paneWidth() - paneMargin() - 41, -100, 580, 17);
    fontEnd__Fv();
}

// One of the four look sliders of mode 2. `character` is the chosen row, `value` the setting, `limit` the row's
// option count and `index` the slider number (it has the pad focus while curSlider equals it).
inline void lookSlider(int y, int character, int value, int limit, int index, int (*change)(int, int)) {
    const int x = paneMargin() + 51;
    const int width = paneWidth() - 81;
    const float fraction = static_cast<float>(value) / static_cast<float>(limit - 1);
    const unsigned color = (static_cast<unsigned>(static_cast<int>(flareAlpha * 128.0f)) << 24) | 0x202020u;
    int player = -1;
    if (otherHermiteTime == 1.0f) player = curSlider == index ? curPlayer : -1;
    int chosen;
    if (hudRunSlider__FiiiiiUifPii(x, y, width, 10, 6, color, fraction, &chosen, player) != 0) {
        if (change(character, chosen) != 0) playMenuSound("menu_main_toggle.anm");
    }
}

// Mode 2 backdrop: titles, the highlighted box that glides between the sliders, and the sliders.
void drawCustomLook(int selected) {
    UString line[128];
    const int left = paneMargin() + 41;
    const int boxWidth = left + (paneWidth() - 61);
    zeroFormat(line, textGetString__FPc("selectCustomLookTitle"), static_cast<float>(padAcceptButtonFontChar));
    fontStart__FP4FontiUii(smallFont, 6, alphaColor(99.0f, 0x316864u), -1);
    fontPrintu__FiiPUsi(41, 49, line, 1000000);
    fontColor__FUiii(alphaColor(79.0f, 0x585e5du), -1, 0);
    const int half = (paneWidth() - 61) / 2;
    fontPrintWrapCenteredu__FiiPUsiiii(left + half, 104, textGetString__FPc("selectCustomLookSkin"), paneWidth() - paneMargin() - 41, -100, 580, 17);
    fontPrintWrapCenteredu__FiiPUsiiii(left + (paneWidth() - 61) / 2, 156, textGetString__FPc("selectCustomLookHairStyle"),
                                       paneWidth() - paneMargin() - 41, -100, 580, 17);
    fontPrintWrapCenteredu__FiiPUsiiii(left + (paneWidth() - 61) / 2, 208, textGetString__FPc("selectCustomLookHairColor"),
                                       paneWidth() - paneMargin() - 41, -100, 580, 17);
    if (maxSlider == 4)
        fontPrintWrapCenteredu__FiiPUsiiii(left + (paneWidth() - 61) / 2, 260, textGetString__FPc("selectCustomLookBeard"),
                                           paneWidth() - paneMargin() - 41, -100, 580, 17);
    fontEnd__Fv();

    float glide = evalHermite__Ffffff(otherHermiteTime, 0.0f, 1.0f, 0.5f, 0.5f);
    float top = static_cast<float>(52 * lastSlider + 104) +
                static_cast<float>((52 * curSlider + 104) - (52 * lastSlider + 104)) * glide;
    if (0.0f <= top) {
        glide = evalHermite__Ffffff(otherHermiteTime, 0.0f, 1.0f, 0.5f, 0.5f);
        top = static_cast<float>(52 * lastSlider + 104) + static_cast<float>((52 * curSlider + 104) - (52 * lastSlider + 104)) * glide;
        top = top + 0.5f;
    } else {
        glide = evalHermite__Ffffff(otherHermiteTime, 0.0f, 1.0f, 0.5f, 0.5f);
        top = static_cast<float>(52 * lastSlider + 104) + static_cast<float>((52 * curSlider + 104) - (52 * lastSlider + 104)) * glide;
        top = top - 0.5f;
    }
    const int boxTop = static_cast<int>(top);
    boxDraw__FiiiiiUii(left, boxTop, boxWidth, boxTop + 52, 6,
                       (toUnsigned(flareAlpha * 128.0f) << 24) | static_cast<unsigned>(throbR) | (static_cast<unsigned>(throbB) << 16) |
                           (static_cast<unsigned>(throbG) << 8),
                       1);

    unsigned char* target = targetOf(selected);
    lookSlider(130, selected, static_cast<signed char>(skinSetting[selected]), static_cast<signed char>(target[24]), 0, changeSkin__Fii);
    const int style = getHairStyle__Fi(selected);
    lookSlider(182, selected, style, static_cast<signed char>(targetOf(selected)[25]), 1, changeHairStyle__Fii);
    lookSlider(234, selected, static_cast<signed char>(hairColorSetting[selected]), static_cast<signed char>(targetOf(selected)[26]), 2,
               changeHairColor__Fii);
    if (maxSlider == 4)
        lookSlider(286, selected, static_cast<signed char>(beardSetting[selected]) + 1, static_cast<signed char>(targetOf(selected)[27]), 3,
                   changeBeard__Fii);
}

// Mode 3 backdrop: the four stat rows with their values and the +/- buttons for the selected one.
void drawStatAllocation(int selected) {
    UString line[128];
    const unsigned titleColor = alphaColor(79.0f, 0x585e5du);
    zeroFormat(line, textGetString__FPc("selectSetStats"), static_cast<float>(padAcceptButtonFontChar));
    fontStart__FP4FontiUii(smallFont, 6, alphaColor(99.0f, 0x316864u), -1);
    fontPrintWrapu__FiiPUsiiii(41, 49, line, paneWidth() - paneMargin() - 41, -100, 580, 17);
    fontColor__FUiii(alphaColor(79.0f, 0x325d18u), -1, 0);
    fontPrintu__FiiPUsi(41, 83, textGetString__FPc("selectStatAllocate"), 1000000);
    zeroFormatC(line, "%d", static_cast<float>(statAllocation));
    fontPrintCenteredu__FiiPUsi(paneWidth() - 19, 83, line, 1000000);
    fontColor__FUiii(titleColor, -1, 0);
    unsigned char* const statTarget = targets + 16 + 636 * selected;
    int y = 117;
    for (int row = 0; row < 4; ++row, y += 17) {
        if (row == curStat)
            fontColor__FUiii(static_cast<unsigned>(throbR) | (static_cast<unsigned>(throbA) << 24) |
                                 ((static_cast<unsigned>(throbB) << 16) | (static_cast<unsigned>(throbG) << 8)),
                             -1, 0);
        fontPrintu__FiiPUsi(41, y, textGetString__FPc(reinterpret_cast<const char*>(static_cast<__UINTPTR_TYPE__>(statTable[row]))), 1000000);
        zeroFormatC(line, "%d", static_cast<float>(static_cast<signed char>(infoOf(curPlayer)[37 + row])));
        fontPrintCenteredu__FiiPUsi(paneWidth() - 39, y, line, 1000000);
        if (row == curStat) {
            const int minimum = *reinterpret_cast<int*>(abilityTable + 16 * static_cast<signed char>(statTarget[1]) + 4 * row);
            if (static_cast<signed char>(infoOf(curPlayer)[37 + row]) != minimum) {
                zeroFormatC(line, "%c", static_cast<float>(gameButtonToIcon__Fi(0x8000)));
                fontPrintCenteredu__FiiPUsi(paneWidth() - 57, y, line, 1000000);
            }
            zeroFormatC(line, "%c", static_cast<float>(gameButtonToIcon__Fi(8192)));
            fontPrintCenteredu__FiiPUsi(paneWidth() - 19, y, line, 1000000);
            if (row == curStat) fontColor__FUiii(titleColor, -1, 0);
        }
    }
    fontColor__FUiii(alphaColor(99.0f, 0x316864u), -1, 0);
    const char* description = nullptr;
    if (curStat == 0) description = "selectStatStrengthDesc";
    else if (curStat == 1) description = "selectStatIntelDesc";
    else if (curStat == 2) description = "selectStatDexterityDesc";
    else if (curStat == 3) description = "selectStatStaminaDesc";
    if (description != nullptr) ustrcpy__FPUsT0(line, textGetString__FPc(description));
    fontPrintWrapu__FiiPUsiiii(41, 4 * 17 + 151, line, paneWidth() - paneMargin() - 41, -100, 580, 17);
    fontEnd__Fv();
}
}  // namespace

extern "C" void playerSelectionTask__Fv() {
    const int selected = static_cast<signed char>(infoOf(curPlayer)[32]);
    const float phase = static_cast<float>(frameNm & 63) * g22356;
    throb = (sinf((phase + phase) * g22352) + 1.0f) * 0.5f;
    throbR = roundHalfAway(throb * 46.0f) + 109;
    throbG = roundHalfAway(throb * 43.0f) + 110;
    throbB = 104;
    throbA = static_cast<int>(flareAlpha * 95.0f);
    if (coreLoading != 0) return;
    if (GetMode__6Camera(gameCam) != 0) return;

    for (int i = 0; i < 10; ++i) {
        unsigned char* object = static_cast<unsigned char*>(objectOf(i));
        if (object == nullptr) continue;
        const unsigned flags = *reinterpret_cast<unsigned short*>(object + 294);
        if ((flags & 0x80) == 0) continue;
        if (!(static_cast<signed char>(object[292]) < 2)) continue;
        transitionTo(i, (flags & 8) != 0 ? "%s_intro3.anm" : "%s_intro1.anm", g22348, 3);
    }
    moveCamera__Fv();
    npcLookAtCamera__Fv();
    if (firstTime != 0) {
        hermiteTime = 0.0f;
        const int loading = texIsLoading__Fv();
        if (hudFadeScreen__Fi(loading != 0 ? 0 : -8) != 0) return;
        firstTime = 0;
    }

    const float flareStep = flareDelta;
    if (flareStep == 0.0f) {
        float body = bodyAlpha;
        if (hermiteTime == 0.0f && lockAlpha == 0) {
            body = body + g22344;
            if (1.0f < body) body = 1.0f;
        } else {
            body = body - g22340;
            if (body < 0.0f) body = 0.0f;
        }
        bodyAlpha = body;
        flareAlpha = body;
        float header = headerAlpha;
        if (header != 1.0f) {
            header = header + g22336;
            headerAlpha = 1.0f < header ? 1.0f : header;
        }
    } else {
        float alpha = flareAlpha + flareStep * g22332;
        if (1.0f < alpha) alpha = 1.0f;
        flareAlpha = alpha;
        if (alpha < 0.0f) flareAlpha = 0.0f;
    }

    UString line[128];
    char name[128];
    switch (static_cast<unsigned>(selectMode) < 9u ? selectMode : 9) {
    case 0:
        selectMode = 1;
        hermDeltaScale = g22328;
        flareDelta = 0.0f;
        break;

    case 1:
        if (requestToExit != 0) {
            if (playerSelectStartPlayer < curPlayer) {
                int alpha = msgAlpha + msgAlphaDelta;
                if (alpha >= 127) {
                    alpha = 126;
                    msgAlpha = alpha;
                }
                if (alpha < 0) alpha = 0;
                zeroFormat(line, textGetString__FPc("selectPrevPlayer"), static_cast<float>(curPlayer));
                msgAlpha = alpha;
                usprintf__FPUsT0ffffffff(hudFooterUni, textGetString__FPc("MemcardYesNo"), static_cast<float>(padAcceptButtonFontChar),
                                         static_cast<float>(padBackButtonFontChar), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                hudShowMsg__FPUsN20iii(textGetString__FPc("selectPrevPlayerTitle"), line, hudFooterUni, msgAlpha, 0, 0);
                if (msgAlpha == 126) {
                    const unsigned buttons = padWord(curPlayer, 12);
                    if ((buttons & padAcceptButton) != 0) msgAlphaDelta = -17;
                    else if ((buttons & padBackButton) != 0) msgAlphaDelta = -16;
                } else if (msgAlpha == 0 && msgAlphaDelta < 0) {
                    if (msgAlphaDelta == -17) {
                        initPosition__Fii(static_cast<signed char>(infoOf(curPlayer)[32]), static_cast<signed char>(infoOf(curPlayer)[-9]));
                        delta = g22324;
                        curPlayer = curPlayer - 1;
                        transitionTo(static_cast<signed char>(infoOf(curPlayer)[32]), "%s_intro2.anm", g22320, 14);
                        selectMode = 8;
                    }
                    requestToExit = 0;
                    msgAlphaDelta = 0;
                }
            } else {
                const int answer = hudRunYesNo__FPUsiiii(playerSelectExitMessage, 0, curPlayer, 0, 0);
                if (answer == 2) allDone__Fi(1);
                else if (answer == 1) requestToExit = 0;
            }
        } else if (flareStep == 0.0f) {
            if (hermiteTime == 0.0f) {
                hermDeltaScale = 1.0f;
                const int row = static_cast<signed char>(infoOf(curPlayer)[32]);
                if (row < 9 && (padWord(curPlayer, 8) & 0x802000u) != 0) {
                    delta = g22316;
                    charDelta = 1;
                    initPosition__Fii(row, row + 1);
                    engineSetBlur__Fi(73);
                } else if (infoOf(curPlayer)[32] != 0 && (padWord(curPlayer, 8) & 0x408000u) != 0) {
                    delta = g22312;
                    charDelta = -1;
                    engineSetBlur__Fi(73);
                    initPosition__Fii(static_cast<signed char>(infoOf(curPlayer)[32]), static_cast<signed char>(infoOf(curPlayer)[32]) - 1);
                } else {
                    const unsigned buttons = padWord(curPlayer, 12);
                    if ((buttons & padAcceptButton) != 0) {
                        flareDelta = g22308;
                        flareAlpha = bodyAlpha;
                        maxSlider = static_cast<signed char>(targetOf(selected)[19] + 3);
                        otherHermiteTime = 1.0f;
                        lastSlider = 0;
                        curSlider = 0;
                        playMenuSound("menu_main_accept.anm");
                    } else if ((buttons & padBackButton) != 0) {
                        msgAlphaDelta = 16;
                        requestToExit = 1;
                        msgAlpha = 0;
                        playMenuSound("menu_main_back.anm");
                    }
                }
            } else if (charDelta != 0 && 0.5f < hermiteTime) {
                if (charDelta > 0) infoOf(curPlayer)[32] = infoOf(curPlayer)[32] + 1;
                if (charDelta < 0) infoOf(curPlayer)[32] = infoOf(curPlayer)[32] - 1;
                charDelta = 0;
            }
        } else if (flareStep < 0.0f) {
            if (flareAlpha != 0.0f) {
                bodyAlpha = flareAlpha;
            } else {
                resetName__Fv();
                transitionTo(selected, "%s_intro2.anm", g22304, 14);
                const char* const sounds[10] = {"bwm014b", "bwf014a", "erm017_altc", "erf031a", "ecm049",
                                                "ecf031a", "ewm114", "ewf053", "esm017_alta", "esf045"};
                formatText(name, "%s.vag", sounds[selected]);
                soundPlaySound__FP10_vagHeaderP6Point3fff(
                    lumpFindResource__FPcT0(reinterpret_cast<const char*>(playerSelectSoundLump), name), nullptr, 1.0f, 1.0f, 0.0f);
                selectMode = 2;
                flareDelta = g22300;
                bodyAlpha = flareAlpha;
            }
        } else {
            if (0.0f < flareStep && flareAlpha == 1.0f) flareDelta = 0.0f;
            bodyAlpha = flareAlpha;
        }
        drawCharacterInfo();
        break;

    case 2:
        if (flareStep != 0.0f) {
            finishFade(flareStep, g22296);
        } else if (otherHermiteTime != 1.0f) {
            const float next = otherHermiteTime + g22292;
            otherHermiteTime = 1.0f < next ? 1.0f : next;
        } else {
            lastSlider = curSlider;
            const unsigned buttons = padWord(curPlayer, 12);
            if ((buttons & padAcceptButton) != 0) {
                unsigned char* const info = infoOf(curPlayer);
                info[33] = hairStyleSetting[selected];
                info[34] = hairColorSetting[selected];
                info[36] = beardSetting[selected];
                info[35] = skinSetting[selected];
                newMode = 3;
                statAllocation = 20;
                curStat = 0;
                statAllocWarn = 0;
                const unsigned char* ability = abilityTable + 16 * static_cast<signed char>(targetOf(selected)[17]);
                for (int k = 3; k >= 0; --k, ability += 4) info[37 + 3 - k] = *ability;
                flareDelta = g22288;
                playMenuSound("menu_main_accept.anm");
            } else if ((buttons & padBackButton) != 0) {
                newMode = 1;
                flareDelta = g22284;
                transitionTo(selected, "%s_intro4.anm", g22280, 6);
                playMenuSound("menu_main_back.anm");
            } else if ((padWord(curPlayer, 16) & 0x101000u) != 0 && static_cast<unsigned char>(curSlider) != 0) {
                otherHermiteTime = 0.0f;
                curSlider = static_cast<signed char>(static_cast<unsigned char>(curSlider) - 1);
                playMenuSound("menu_main_accept.anm");
            } else if ((padWord(curPlayer, 16) & 0x204000u) != 0 && curSlider < maxSlider - 1) {
                curSlider = static_cast<signed char>(curSlider + 1);
                otherHermiteTime = 0.0f;
                playMenuSound("menu_main_accept.anm");
            }
        }
        drawCustomLook(selected);
        break;

    case 3:
        if (flareStep != 0.0f) {
            finishFade(flareStep, g22276);
        } else {
            bool leave = false;
            if (statAllocWarn != 0) {
                int alpha = msgAlpha + msgAlphaDelta;
                if (alpha >= 127) {
                    alpha = 126;
                    msgAlpha = alpha;
                }
                if (alpha < 0) alpha = 0;
                msgAlpha = alpha;
                UString* prompt = textGetString__FPc("MemcardContinue");
                usprintf__FPUsT0ffffffff(hudFooterUni, prompt, static_cast<float>(padAcceptButtonFontChar), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                UString* title = textGetString__FPc("selectSetStatsWarnTitle");
                UString* body = textGetString__FPc("selectSetStatsWarnBody");
                hudShowMsg__FPUsN20iii(title, body, hudFooterUni, msgAlpha, 0, 0);
                const int shown = msgAlpha;
                if (shown == 126 && (padWord(curPlayer, 12) & padAcceptButton) != 0) msgAlphaDelta = -16;
                if (msgAlphaDelta < 0 && shown == 0) statAllocWarn = 0;
            } else {
                const unsigned buttons = padWord(curPlayer, 12);
                if ((buttons & padAcceptButton) != 0) {
                    if (statAllocation != 0) {
                        statAllocWarn = 1;
                        msgAlphaDelta = 16;
                    } else {
                        leave = true;
                        playMenuSound("menu_main_accept.anm");
                    }
                } else if ((buttons & padBackButton) != 0) {
                    newMode = 2;
                    flareDelta = g22272;
                    playMenuSound("menu_main_back.anm");
                } else {
                    bool done = false;
                    const int player = curPlayer;
                    if ((padWord(player, 16) & 0x101000u) != 0 && curStat != 0) {
                        curStat = static_cast<unsigned char>(curStat - 1);
                        playMenuSound("menu_main_accept.anm");
                        done = true;
                    }
                    if (!done && (padWord(player, 16) & 0x204000u) != 0 && curStat < 3) {
                        curStat = static_cast<unsigned char>(curStat + 1);
                        playMenuSound("menu_main_accept.anm");
                        done = true;
                    }
                    if (!done && (padWord(player, 16) & 0x802000u) != 0 && statAllocation != 0) {
                        const int remaining = statAllocation;
                        unsigned char* const stat = infoOf(player) + 37 + curStat;
                        if (static_cast<signed char>(*stat) < 100) {
                            *stat = static_cast<unsigned char>(*stat + 1);
                            statAllocation = remaining - 1;
                            playMenuSound("menu_main_toggle.anm");
                            done = true;
                        }
                    }
                    if (!done && (padWord(player, 16) & 0x408000u) != 0) {
                        const int which = curStat;
                        unsigned char* const stat = infoOf(player) + 37 + which;
                        const int character = static_cast<signed char>(infoOf(player)[32]);
                        const int minimum = *reinterpret_cast<int*>(abilityTable + 4 * (4 * static_cast<signed char>(targetOf(character)[17]) + which));
                        if (minimum < static_cast<signed char>(*stat)) {
                            *stat = static_cast<unsigned char>(*stat - 1);
                            statAllocation = statAllocation + 1;
                            playMenuSound("menu_main_toggle.anm");
                        }
                    }
                }
            }
            if (leave) {
                switchToNamePad__Fv();
                newMode = 4;
                flareDelta = g22268;
            }
        }
        drawStatAllocation(selected);
        break;

    case 4:
        if (flareStep != 0.0f) {
            finishFade(flareStep, g22264);
        } else {
            const int result = doNamePad__Fv();
            if (result != 0) {
                if (result == 1) {
                    transitionTo(selected, "%s_intro4.anm", g22260, 22);
                    if (curPlayer - playerSelectStartPlayer == playerSelectCount - 1) {
                        hudResetFadeScreen__Fi(0);
                        selectMode = 5;
                    } else {
                        newMode = 6;
                        msgAlphaDelta = 16;
                    }
                }
                if (result == -1) newMode = 3;
                flareDelta = g22256;
            }
        }
        drawNamePad__Fv();
        break;

    case 5:
        bodyAlpha = flareAlpha;
        if ((*reinterpret_cast<unsigned short*>(static_cast<unsigned char*>(objectOf(selected)) + 294) & 0x80) != 0) startExitFade = 1;
        if (startExitFade != 0 && hudFadeScreen__Fi(4) == 128) allDone__Fi(0);
        break;

    case 6: {
        if ((*reinterpret_cast<unsigned short*>(static_cast<unsigned char*>(objectOf(selected)) + 294) & 0x10) != 0) return;
        int alpha = msgAlpha + msgAlphaDelta;
        if (alpha >= 127) {
            alpha = 126;
            msgAlpha = alpha;
        }
        if (alpha < 0) alpha = 0;
        msgAlpha = alpha;
        if (flareStep != 0.0f) finishFade(flareStep, g22252);
        usprintf__FPUsT0ffffffff(line, textGetString__FPc("selectNextPlayer"), static_cast<float>(curPlayer + 1), static_cast<float>(curPlayer + 2),
                                 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        usprintf__FPUsT0ffffffff(hudFooterUni, textGetString__FPc("MemcardContinueBack"), static_cast<float>(padAcceptButtonFontChar),
                                 static_cast<float>(padBackButtonFontChar), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        hudShowMsg__FPUsN20iii(textGetString__FPc("selectNextPlayerTitle"), line, hudFooterUni, msgAlpha, 0, 0);
        const int shown = msgAlpha;
        if (shown == 126) {
            const unsigned buttons = padWord(curPlayer, 12);
            if ((buttons & (padAcceptButton | padBackButton)) != 0) {
                msgAlphaDelta = (buttons & padAcceptButton) != 0 ? -17 : -16;
                break;
            }
        }
        if (shown != 0) break;
        if (msgAlphaDelta >= 0) break;
        if (msgAlphaDelta == -17) {
            initPostSelect__Fv();
        } else {
            flareDelta = g22248;
            newMode = 4;
            transitionTo(selected, "%s_intro2.anm", g22244, 14);
        }
        msgAlphaDelta = 0;
        break;
    }

    case 7:
        hermDeltaScale = g22240;
        bodyAlpha = flareAlpha;
        if (hermiteTime == 0.0f) {
            curPlayer = curPlayer + 1;
            infoOf(curPlayer)[32] = 0;
            initPreSelect__Fv();
        }
        break;

    case 8:
        hermDeltaScale = g22236;
        if (hermiteTime == 0.0f) {
            switchToNamePad__Fv();
            selectMode = 4;
        }
        break;

    default:
        break;
    }

    if (static_cast<unsigned>(selectMode - 6) < 2u) return;
    const float backdrop = bodyAlpha * 76.0f;
    if (backdrop != 0.0f) textureSmoothBorder__FiiiiiUiiii(21, 29, paneWidth(), 374, 6, toUnsigned(backdrop) << 24, 1, 0, 0);
    if (flareAlpha * 75.0f == 0.0f) return;
    if (selectMode == 1) {
        const unsigned char row = infoOf(curPlayer)[32];
        if (static_cast<unsigned>(row) - 1u < 8u) {
            usprintf__FPUsT0ffffffff(line, textGetString__FPc("SelectPromptPrevNext"), 127.0f, static_cast<float>(padAcceptButtonFontChar),
                                     static_cast<float>(padBackButtonFontChar), 128.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        } else if (static_cast<signed char>(row) < 9) {
            usprintf__FPUsT0ffffffff(line, textGetString__FPc("SelectPromptNext"), static_cast<float>(padAcceptButtonFontChar),
                                     static_cast<float>(padBackButtonFontChar), 128.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        } else {
            usprintf__FPUsT0ffffffff(line, textGetString__FPc("SelectPromptPrev"), 127.0f, static_cast<float>(padAcceptButtonFontChar),
                                     static_cast<float>(padBackButtonFontChar), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        }
    } else if (selectMode == 4 || selectMode == 6) {
        zeroFormat(line, textGetString__FPc("SelectPromptName"), static_cast<float>(padBackButtonFontChar));
    } else {
        usprintf__FPUsT0ffffffff(line, textGetString__FPc("SelectPrompt"), static_cast<float>(padAcceptButtonFontChar),
                                 static_cast<float>(padBackButtonFontChar), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    }
    fontStart__FP4FontiUii(smallFont, 6, alphaColor(99.0f, 0x316864u), static_cast<int>(flareAlpha * 128.0f));
    fontPrintCenteredu__FiiPUsi(320, 410, line, 1000000);
    fontEnd__Fv();
    const int width = fontStringSizeu__FP4FontPUsi(smallFont, line, 1000000) + 40;
    textureSmoothBorder__FiiiiiUiiii(320 - width / 2, 408, width, 32, 6, toUnsigned(flareAlpha * 75.0f) << 24, 0, 0, 0);
}
