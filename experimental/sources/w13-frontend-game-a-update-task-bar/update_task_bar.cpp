// Observed updateTaskBar__FiP12screenStruct at 0x001b2a90, 608 bytes. Arguments: a player pn and a
// screen record (ints; the numbering is the one the shop grid code uses: [5] move latch, [6] and [7]
// cursor, [8] to [10] scroll/hover words, [12] timer, [20] current tab). pad is netLocalPad + 88 * pn.
//  Returns at once when multiHelpMessageOn[pn] (signed byte), sellMode[pn] or the byte helpMessageOn
//  is nonzero. last = 3 when shopRunning is nonzero, else 6.
//  hot = shopMouseOnHotSpot__FP12screenStruct(screen).
//  1. Pad word +12 has padAcceptButton and hot >= 0: pad word +16 of both netLocalPad[pn] and the
//     matching element of the array pad (0x0034beb8) become zero, screen[20] = hot, [6] [7] [8] [9] [10]
//     = 0, a local player (netPlayerIsLocal) plays the sound "game_butn_tab.anm" from "fx.lmp"
//     (volume 1.0, no object), the words 6, 9 and 10 of the screen record at statsScreen + 116 * pn are
//     cleared, and screen[5] = 0.
//  2. Otherwise screen[5] = 0 is stored whenever one of the pad word +16 bits below is set, and with
//     those bits: 8 or 2 raise the tab (unless screen[20] > last - 2), 4 or 1 lower it (unless
//     screen[20] <= 0). The change sets screen[12] = 0, screen[20] = the new tab, [6] [7] [8] [9]
//     [10] = 0, plays "menu_shop_tab.anm" the same way and clears the statsScreen words as above.
//  No bits, or none of these conditions, change nothing.
// Inferred: the tab bar of the shop and inventory screens: a click on a tab or the shoulder buttons
// change the tab. Unknown: what the zeroed words are, and which of the two sounds is which control.
extern "C" {
extern signed char multiHelpMessageOn[];
extern int sellMode[];
extern signed char helpMessageOn;
extern int shopRunning;
extern unsigned padAcceptButton;
extern unsigned char netLocalPad[];
extern unsigned char pad[];
extern unsigned char statsScreen[];
int shopMouseOnHotSpot__FP12screenStruct(void* screen);
int netPlayerIsLocal__Fi(int player);
void* lumpFindResource__FPcT0(char* lump, char* resource);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
}

namespace {
inline unsigned& word(unsigned char* base, int offset) { return *reinterpret_cast<unsigned*>(base + offset); }

void clearAndPlay(int pn, int* screen, char* sound) {
    screen[6] = 0;
    screen[7] = 0;
    screen[9] = 0;
    screen[10] = 0;
    screen[8] = 0;
    if (netPlayerIsLocal__Fi(pn) != 0)
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), sound), nullptr, 1.0f);
    unsigned char* stats = statsScreen + 116 * pn;
    word(stats, 36) = 0;
    word(stats, 40) = 0;
    word(stats, 24) = 0;
}
}

extern "C" void updateTaskBar__FiP12screenStruct(int pn, void* screenRecord) {
    int* screen = static_cast<int*>(screenRecord);
    if (multiHelpMessageOn[pn] != 0) return;
    if (sellMode[pn] != 0) return;
    if (helpMessageOn != 0) return;
    int last = 6;
    if (shopRunning != 0) last = 3;
    const int hot = shopMouseOnHotSpot__FP12screenStruct(screenRecord);
    unsigned char* held = netLocalPad + 88 * pn;
    if ((word(held, 12) & padAcceptButton) != 0 && hot >= 0) {
        word(pad + 88 * pn, 16) = 0;
        word(held, 16) = 0;
        screen[20] = hot;
        clearAndPlay(pn, screen, const_cast<char*>("game_butn_tab.anm"));
        screen[5] = 0;
        return;
    }
    const unsigned bits = word(held, 16);
    int tab;
    if ((bits & 8) != 0 || (bits & 2) != 0) {
        const int current = screen[20];
        screen[5] = 0;
        if (last - 2 < current) return;
        tab = current + 1;
    } else if ((bits & 4) != 0 || (bits & 1) != 0) {
        const int current = screen[20];
        screen[5] = 0;
        if (current <= 0) return;
        tab = current - 1;
    } else {
        return;
    }
    screen[12] = 0;
    screen[20] = tab;
    clearAndPlay(pn, screen, const_cast<char*>("menu_shop_tab.anm"));
}
