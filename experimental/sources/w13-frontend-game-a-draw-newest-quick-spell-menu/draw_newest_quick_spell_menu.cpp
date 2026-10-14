// Observed drawNewestQuickSpellMenu__Fiiicl at 0x001c5078, 3428 bytes, 416-byte frame. Arguments: a
// player pn, half (0 or 1, the screen half; x offsets are 320 * half), flag, kind (signed byte) and
// index (a long). rec is player + 23984 * pn. Returns nothing. Nothing happens for a non-local player
// (netPlayerIsLocal). Record words: float at +23564 (a countdown), int at +23568 (alpha step), bytes
// at +23580 and +23581 (the ring slot assigned to each spell button), and ring slot k (k = 0 to 7) at
// rec + 21976 + 104 * k: name text at +0, halfword spell number at +32 (-1 for empty).
//  1. Centre: x = {0, 471, 156, 316}[pn] + 67, y = 145; with flag nonzero x = 141 and y = 249.
//  2. Input: the stick/d-pad bits are read from netLocalPad + 88 * pn, word +8, when the game allows
//     it (gameControlPaused, DLG_Active, the camera mode word +52 equal to 1, gamePause,
//     debugPauseMenuOn, MC_Busy, engineTaskActive("jumpgate"), hudNetWaiting all idle) or interfaceOn
//     is nonzero: bit 0x2000 gives dx = 1, 0x8000 dx = -1, 0x1000 dy = -1, 0x4000 dy = 1. Any direction
//     blanks the pad's controller buttons (netBlankControllerButtons(pn, 3, spellButton1[pn] |
//     spellButton2[pn])).
//  3. With a direction or flag nonzero: rec float +23564 = 30, hudQuickSpellOn[pn] = 1, rec int +23568 =
//     4; otherwise hudQuickSpellOn[pn] = 0. The countdown being exactly zero sets +23568 to -1. The
//     fade alpha (local array a.1122, per player) changes by 4 * +23568 and is limited to 0 to 128; the
//     countdown drops by one. Above 64, for a player index from 2 up with four or more players,
//     hudBustCovered becomes 1. An alpha of zero or less sets hudQuickSpellOn[pn] = 0 and ends.
//  4. The selected slot sel (0 to 7, -1 for none) comes from (dx, dy): (0,-1) 0, (1,-1) 1, (1,0) 2,
//     (1,1) 3, (0,1) 4, (-1,1) 5, (-1,0) 6, (-1,-1) 7, (0,0) none. A change from curBoxOld[pn]
//     plays "menu_spell_high.anm" (fx.lmp) for a local player; curBoxOld[pn] = sel;
//     "spellbookarrow%d.tex" of sel is drawn (hud.lmp) at (x - 24, y - 24) when sel is not -1 and flag
//     is zero.
//  5. The spell book is drawn (ResourceMap::get(spellBook) at (x - 68, y - 65) + 320 * half) with
//     colour alpha << 24 | 0x808080, and radii r1 = (int)(hermite(alpha / 128) * 60) and
//     r2 = (int)(hermite * 33) are formed. For the eight angles -16384, -8192, ... in steps of 8192 the
//     ring slot's icon (the skill's virtual function at vtable + 64 when the name is "newstyle", else
//     globalIcons[spell]; the emptySpell map when none) is drawn at (x + r1 * cos, y + r1 * sin); the
//     selected one with a pulsing "smallspelliconcorona.tex" behind it and a pulsing colour; with flag
//     zero the two button glyphs (hudGetRemappedButtonIcon(pn, 1 and 2), format "%c") are printed beside
//     the slot assigned to each button at radius r2.
//  6. Buttons: a new press (netLocalPad word +12) of spellButton1[pn] or spellButton2[pn] assigns the
//     selected slot to quick spell 0 or 1 (msgHudAssignQuickSpell(pn, 0 or 1, sel)) when flag is zero;
//     with flag nonzero a press of equipButton calls msgHudAssignSpellBookSpell(pn, sel, kind, index).
//  7. The selected slot's text (textGetString("EmptySpellSlot") for an empty slot, else the slot name
//     or skills[spell]'s first word, plus "_name") is printed centred below the ring at half alpha.
//  8. With flag nonzero last: the icon of skills[index] (virtual function) when kind is 1, else
//     globalIcons[37], at (320 * half + 142, 248), the arrow again (sel not -1), and the
//     "inventry.lmp" "spellbookparch.tex" panel at (320 * half + 57, 165).
// Constants (bit patterns): 30.0, 1/128, 60.0, 33.0, 16.0, 239.0, 128.0, 1.0.
// Unknown: the roles of flag, kind and index beyond the above; what the fixed radii mean.
extern "C" {
extern unsigned char player[];
extern unsigned char netLocalPad[];
extern int gameControlPaused;
extern int gamePause;
extern signed char interfaceOn;
extern signed char debugPauseMenuOn;
extern signed char hudNetWaiting;
extern signed char hudBustCovered;
extern int nmPlayers;
extern int drawFrameNm;
extern void* smallFont;
extern void* gameCam;
extern unsigned equipButton;
extern int hudQuickSpellOn[];
extern unsigned spellButton1[];
extern unsigned spellButton2[];
extern int norrath_quick_spell_alpha[] __asm__("a.1122");
extern int norrath_quick_spell_box[] __asm__("curBoxOld");
extern unsigned char spellBook[];
extern unsigned char emptySpell[];
extern unsigned globalIcons[];
extern unsigned skills[];
int netPlayerIsLocal__Fi(int player);
int DLG_Active__Fv();
int MC_Busy__Fv();
int engineTaskActive__FPc(const char* name);
void netBlankControllerButtons__Fiii(int player, int a, unsigned buttons);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
int sprintf(char* out, const char* format, ...);
int strcmp(const char* left, const char* right);
char* strcat(char* to, const char* from);
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color);
void* get__11ResourceMap(void* map);
float evalHermite__Ffffff(float t, float a, float b, float c, float d);
float icos__Fi(int angle);
float isin__Fi(int angle);
void fontStart__FP4FontiUii(void* font, int size, unsigned color, int flags);
void fontEnd__Fv();
void fontPrint__FiiPci(int x, int y, char* text, int limit);
int hudGetRemappedButtonIcon__Fii(int player, int button);
void msgHudAssignQuickSpell__Fiii(int player, int button, int slot);
void msgHudAssignSpellBookSpell__Fiiil(int player, int slot, int kind, long index);
unsigned short* textGetString__FPc(const char* name);
void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* text, int limit);
}

#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_base;
// Host check hook: calls the function word of a skill's vtable entry (a function word is not a host pointer).
extern "C" unsigned norrath_host_call_icon(unsigned functionWord, unsigned char* object);
#endif

namespace {
// Pointer words of the image are 32-bit; a host check maps them to offsets in one arena.
inline unsigned char* at(unsigned word) {
#ifdef NORRATH_HOST_ARENA
    return norrath_host_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
template <class T>
inline T load(const unsigned char* base, int offset) {
    T value;
    __builtin_memcpy(&value, base + offset, sizeof value);
    return value;
}
inline float floatFromBits(unsigned bits) {
    float value;
    __builtin_memcpy(&value, &bits, sizeof value);
    return value;
}
// Texture words are opaque handles.
inline void* handle(unsigned word) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word)); }
// Calls the skill object's function word at (vtable + 64 + 4) with the object moved by the halfword
// at (vtable + 64), the way the machine code does; the result is a texture handle word.
inline unsigned skillIcon(unsigned skillWord) {
    unsigned char* skill = at(skillWord);
    unsigned char* entry = at(load<unsigned>(skill, 96)) + 64;
#ifdef NORRATH_HOST_ARENA
    return norrath_host_call_icon(load<unsigned>(entry, 4), skill + load<short>(entry, 0));
#else
    typedef unsigned (*IconFunction)(unsigned char* object);
    return reinterpret_cast<IconFunction>(static_cast<__UINTPTR_TYPE__>(load<unsigned>(entry, 4)))(skill + load<short>(entry, 0));
#endif
}
}

extern "C" void drawNewestQuickSpellMenu__Fiiicl(int pn, int half, int flag, char kind, long index) {
    if (netPlayerIsLocal__Fi(pn) == 0) return;
    const int xs[4] = {0, 471, 156, 316};
    char name[64];
    unsigned char* rec = player + 23984 * pn;
    int baseX = xs[pn] + 67;
    int baseY = 145;
    if (flag != 0) {
        baseX = 141;
        baseY = 249;
    }
    int dx = 0;
    int dy = 0;
    bool moved = false;
    bool idle = false;
    if (gameControlPaused == 0 && DLG_Active__Fv() == 0 && load<int>(static_cast<unsigned char*>(gameCam), 52) == 1 && gamePause == 0 &&
        debugPauseMenuOn == 0 && MC_Busy__Fv() == 0 && engineTaskActive__FPc("jumpgate") == 0 && hudNetWaiting == 0)
        idle = true;
    if (idle || interfaceOn != 0) {
        const unsigned bits = load<unsigned>(netLocalPad + 88 * pn, 8);
        if ((bits & 0x2000) != 0) { dx = 1; moved = true; }
        if ((bits & 0x8000) != 0) { dx = -1; moved = true; }
        if ((bits & 0x1000) != 0) { dy = -1; moved = true; }
        if ((bits & 0x4000) != 0) { dy = 1; moved = true; }
    }
    if (moved) netBlankControllerButtons__Fiii(pn, 3, spellButton1[pn] | spellButton2[pn]);
    if (dx != 0 || dy != 0 || flag != 0) {
        *reinterpret_cast<float*>(rec + 23564) = floatFromBits(0x41f00000u);
        hudQuickSpellOn[pn] = 1;
        *reinterpret_cast<int*>(rec + 23568) = 4;
    } else {
        hudQuickSpellOn[pn] = 0;
    }
    const float countdown = *reinterpret_cast<float*>(rec + 23564);
    if (countdown == 0.0f) *reinterpret_cast<int*>(rec + 23568) = -1;
    {
        const int alpha = norrath_quick_spell_alpha[pn] + *reinterpret_cast<int*>(rec + 23568) * 4;
        norrath_quick_spell_alpha[pn] = alpha;
        *reinterpret_cast<float*>(rec + 23564) = countdown - 1.0f;
        if (alpha >= 129) norrath_quick_spell_alpha[pn] = 128;
        else if (alpha < 0) norrath_quick_spell_alpha[pn] = 0;
    }
    if (norrath_quick_spell_alpha[pn] > 64 && pn >= 2 && nmPlayers >= 4) hudBustCovered = 1;
    if (norrath_quick_spell_alpha[pn] <= 0) {
        hudQuickSpellOn[pn] = 0;
        return;
    }

    int sel = -1;
    if (dx == 0) {
        if (dy == -1) sel = 0;
        else if (dy == 1) sel = 4;
    } else if (dx == 1) {
        if (dy == -1) sel = 1;
        else if (dy == 0) sel = 2;
        else sel = 3;
    } else if (dx == -1) {
        if (dy == 1) sel = 5;
        else if (dy == 0) sel = 6;
        else if (dy == -1) sel = 7;
    }
    if (norrath_quick_spell_box[pn] != sel) {
        if (netPlayerIsLocal__Fi(pn) != 0)
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("menu_spell_high.anm")), nullptr, 1.0f);
    }
    norrath_quick_spell_box[pn] = sel;
    sprintf(name, "spellbookarrow%d.tex", sel);
    const int halfX = (half * 4 + half) << 6;
    if (sel != -1 && flag == 0) {
        void* arrow = lumpFindResource__FPcT0(const_cast<char*>("hud.lmp"), name);
        textureDrawColor__FP8_textureiiiUi(arrow, halfX + baseX - 24, baseY - 24, 7, static_cast<unsigned>(norrath_quick_spell_alpha[pn]) << 24 | 0x808080u);
    }

    void* book = get__11ResourceMap(spellBook);
    textureDrawColor__FP8_textureiiiUi(book, halfX + (baseX - 68), baseY - 65, 6, static_cast<unsigned>(norrath_quick_spell_alpha[pn]) << 24 | 0x808080u);
    const float eased = evalHermite__Ffffff(static_cast<float>(norrath_quick_spell_alpha[pn]) * floatFromBits(0x3c000000u), 0.0f, 1.0f, 0.0f, 0.0f);
    const float ringBig = static_cast<float>(static_cast<int>(eased * floatFromBits(0x42700000u)));
    const int ringSmall = static_cast<int>(eased * floatFromBits(0x42040000u));
    for (int angle = -16384; angle <= 0xbfff; angle += 8192) {
        const int slotX = baseX + static_cast<int>(ringBig * icos__Fi(angle)) + halfX;
        const int slotY = baseY + static_cast<int>(ringBig * isin__Fi(angle));
        const int shifted = angle + 16384;
        const int ring = (shifted < 0 ? angle + 24575 : shifted) >> 13;
        unsigned char* slot = rec + 22000 + 104 * ring;
        void* icon;
        if (load<short>(slot, 8) == -1) {
            icon = get__11ResourceMap(emptySpell);
        } else {
            if (strcmp(reinterpret_cast<const char*>(rec + 21976 + 104 * ring), "newstyle") == 0) icon = handle(skillIcon(skills[load<short>(slot, 8)]));
            else icon = handle(globalIcons[load<short>(slot, 8)]);
            if (icon == nullptr) icon = get__11ResourceMap(emptySpell);
        }
        if (((angle < 0 ? angle + 8191 : angle) >> 13) + 2 == sel) {
            void* corona = lumpFindResource__FPcT0(const_cast<char*>("hud.lmp"), const_cast<char*>("smallspelliconcorona.tex"));
            textureDrawColor__FP8_textureiiiUi(corona, slotX - 6, slotY - 6, 7,
                                               static_cast<unsigned>(static_cast<int>(isin__Fi(drawFrameNm << 10) * floatFromBits(0x41800000u) + floatFromBits(0x43000000u))) << 24 | 0x808080u);
            unsigned color = static_cast<unsigned>(static_cast<int>(isin__Fi(drawFrameNm << 10) * floatFromBits(0x41800000u) + floatFromBits(0x436f0000u))) |
                             static_cast<unsigned>(norrath_quick_spell_alpha[pn]) << 24;
            unsigned second = static_cast<unsigned>(static_cast<int>(isin__Fi(drawFrameNm << 10) * floatFromBits(0x41800000u) + floatFromBits(0x436f0000u))) << 16;
            second |= static_cast<unsigned>(static_cast<int>(isin__Fi(drawFrameNm << 10) * floatFromBits(0x41800000u) + floatFromBits(0x436f0000u))) << 8;
            textureDrawColor__FP8_textureiiiUi(icon, slotX, slotY, 6, color | second);
        } else {
            textureDrawColor__FP8_textureiiiUi(icon, slotX, slotY, 6, static_cast<unsigned>(norrath_quick_spell_alpha[pn]) << 24 | 0x808080u);
        }
        if (flag != 0) continue;
        fontStart__FP4FontiUii(smallFont, 7, static_cast<unsigned>(norrath_quick_spell_alpha[pn]) << 24 | 0x323c3cu, -1);
        const int textX = baseX + static_cast<int>(static_cast<float>(ringSmall) * icos__Fi(angle)) + halfX;
        const int textY = baseY + static_cast<int>(static_cast<float>(ringSmall) * isin__Fi(angle));
        const int textRing = (angle + 16384 < 0 ? angle + 24575 : angle + 16384) >> 13;
        for (int button = 0; button < 2; ++button) {
            char glyph[16];
            sprintf(glyph, "%c", hudGetRemappedButtonIcon__Fii(pn, button + 1));
            if (static_cast<signed char>(rec[23580 + button]) == textRing) fontPrint__FiiPci(textX + 5, textY, glyph, 100000);
        }
        fontEnd__Fv();
    }

    if (sel != -1) {
        if (flag == 0) {
            const unsigned pressed = load<unsigned>(netLocalPad + 88 * pn, 12);
            if ((pressed & spellButton1[pn]) != 0) msgHudAssignQuickSpell__Fiii(pn, 0, sel);
            else if ((pressed & spellButton2[pn]) != 0) msgHudAssignQuickSpell__Fiii(pn, 1, sel);
        } else {
            if ((load<unsigned>(netLocalPad + 88 * pn, 12) & equipButton) != 0) msgHudAssignSpellBookSpell__Fiiil(pn, sel, kind, index);
        }
        unsigned char* slot = rec + 22000 + 104 * sel;
        unsigned short* label;
        if (load<short>(slot, 8) == -1) {
            label = textGetString__FPc("EmptySpellSlot");
        } else {
            const char* slotName = reinterpret_cast<const char*>(rec + 21976 + 104 * sel);
            char text[128];
            if (strcmp(slotName, "newstyle") == 0)
                sprintf(text, "%s", at(load<unsigned>(at(skills[load<short>(slot, 8)]), 0)));
            else
                sprintf(text, "%s", slotName);
            strcat(text, "_name");
            label = textGetString__FPc(text);
        }
        const int alpha = norrath_quick_spell_alpha[pn];
        fontStart__FP4FontiUii(smallFont, 7, static_cast<unsigned>((alpha + static_cast<int>(static_cast<unsigned>(alpha) >> 31)) >> 1) << 24 | 0x808080u, -1);
        fontPrintCenteredu__FiiPUsi(halfX + baseX + 24, baseY + 87, label, 1000000);
        fontEnd__Fv();
    }
    if (flag == 0) return;
    void* panelIcon;
    if (kind == 1) panelIcon = handle(skillIcon(skills[static_cast<int>(index)]));
    else panelIcon = handle(globalIcons[37]);
    if (panelIcon != nullptr)
        textureDrawColor__FP8_textureiiiUi(panelIcon, halfX + 142, 248, 6, static_cast<unsigned>(norrath_quick_spell_alpha[pn]) << 24 | 0x808080u);
    if (sel != -1 && flag != 0) {
        void* arrow = lumpFindResource__FPcT0(const_cast<char*>("hud.lmp"), name);
        textureDrawColor__FP8_textureiiiUi(arrow, halfX + baseX - 24, baseY - 24, 6, static_cast<unsigned>(norrath_quick_spell_alpha[pn]) << 24 | 0x808080u);
    }
    void* parchment = lumpFindResource__FPcT0(const_cast<char*>("inventry.lmp"), const_cast<char*>("spellbookparch.tex"));
    textureDrawColor__FP8_textureiiiUi(parchment, halfX + 57, 165, 6, static_cast<unsigned>(norrath_quick_spell_alpha[pn]) << 24 | 0x808080u);
}
