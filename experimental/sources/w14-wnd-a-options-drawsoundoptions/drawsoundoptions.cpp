// Observed: DrawSoundOptions__12VIWndOptions at 0x010c5e50 is 856 bytes. With this, g = VIGetGameOptions__Fv(), win =
// this + 408 (an embedded VIUIWindow whose int at +20 is w), sel the signed word at +1148 and mUI the word
// _5VIWnd$mUI:
//  1. the word at +1152 is set to 2, 3, 4, 5 for sel 0, 1, 2, 3 and otherwise left alone;
//  2. Draw__10VIUIWindow(win); then TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(win, GetString__13VIStringTableii(mUI, id,
//     the word at mUI + 48), 40, k, w - 80, 1, color, 0) for k = 1..4 with ids 751, 772, 782, 801 and color 1 when
//     sel == k - 1, else 0;
//  3. TickR__10VIUIWindowiib(win, 36, sel + 1, 1);
//  4. DrawFrame__10VIUIWindowiiiiUi(win, w - 176, 1, 132, 1, flags) with flags 96 when sel == 0, else 64; then
//     TextBox(win, the pointer sBattleMusicText[g[4]], w - 176, 1, 132, 1, 0, 8) (the byte index is not range
//     checked);
//  5. DrawHScroll__10VIUIWindowiiifUi(win, w - 180, 2, 140, the float at g+8, flags) with flags 32 when sel == 1, else 0;
//     then the same with row 3, the float at g+12 and flags 32 when sel == 2;
//  6. DrawFrame(win, w - 176, 4, 132, 1, flags) with flags 96 when sel == 3, else 64; then TextBox(win, the pointer
//     sSoundOutputText[g[5]], w - 176, 4, 132, 1, 0, 8);
//  7. a 512-byte buffer of 16-bit units gets str8to16("|t ") then strcat16 of GetString(mUI, 750, column); then
//     Text__10VIUIWindowPCUsii9VIUICOLORUi(win, buffer, 40, 6, 0, 0).
// Inferred: the sound page: battle-music choice, two volume sliders (the floats Increment/Decrement__12VIWndOptions
// change, rows 3 and 4 there) and an output mode, with row ids 2..5 matching the Increment/Decrement switch rows.
// The tables hold 32-bit text pointers filled in at run time by OnDrawSelf__12VIWndOptions; the string "|t " was read
// from the original's read-only data.
// Unknown: the colour enum name, the meaning of flag values 32, 64 and 96, and the result type (void here).
extern "C" {
extern unsigned _5VIWnd$mUI;
extern unsigned sBattleMusicText[10] __asm__("VIWndOptions$sBattleMusicText");
extern unsigned sSoundOutputText[2] __asm__("VIWndOptions$sSoundOutputText");
unsigned char* VIGetGameOptions__Fv();
void Draw__10VIUIWindow(void* window);
const unsigned short* GetString__13VIStringTableii(const void* table, int id, int column);
void str8to16__8VIStringPUsUiPCc(unsigned short* destination, unsigned size, const char* source);
void strcat16__8VIStringPUsUiPCUs(unsigned short* destination, unsigned size, const unsigned short* source);
int TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int c, int d, int color,
                                              unsigned flags);
int Text__10VIUIWindowPCUsii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int color, unsigned flags);
void TickR__10VIUIWindowiib(void* window, int a, int b, bool c);
void DrawFrame__10VIUIWindowiiiiUi(void* window, int a, int b, int c, int d, unsigned flags);
void DrawHScroll__10VIUIWindowiiifUi(void* window, int a, int b, int c, float value, unsigned flags);

static inline const void* ui_table() {
    return reinterpret_cast<const void*>(static_cast<unsigned long>(_5VIWnd$mUI));
}
static inline int column() {
    return *reinterpret_cast<const int*>(static_cast<unsigned long>(_5VIWnd$mUI) + 48);
}
static inline const unsigned short* text_of(unsigned word) {
    return reinterpret_cast<const unsigned short*>(static_cast<unsigned long>(word));
}

void DrawSoundOptions__12VIWndOptions(void* self) {
    int* words = static_cast<int*>(self);
    unsigned char* g = VIGetGameOptions__Fv();
    const int sel = words[287];
    if (sel == 1) words[288] = 3;
    else if (sel == 0) words[288] = 2;
    else if (sel == 2) words[288] = 4;
    else if (sel == 3) words[288] = 5;
    unsigned char* window = static_cast<unsigned char*>(self) + 408;
    Draw__10VIUIWindow(window);
    static const int ids[4] = {751, 772, 782, 801};
    for (int k = 1; k <= 4; ++k) {
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, GetString__13VIStringTableii(ui_table(), ids[k - 1], column()), 40,
                                                  k, *reinterpret_cast<int*>(window + 20) - 80, 1, words[287] == k - 1, 0);
    }
    TickR__10VIUIWindowiib(window, 36, words[287] + 1, true);
    DrawFrame__10VIUIWindowiiiiUi(window, *reinterpret_cast<int*>(window + 20) - 176, 1, 132, 1, words[287] ? 64 : 96);
    TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text_of(sBattleMusicText[g[4]]),
                                              *reinterpret_cast<int*>(window + 20) - 176, 1, 132, 1, 0, 8);
    DrawHScroll__10VIUIWindowiiifUi(window, *reinterpret_cast<int*>(window + 20) - 180, 2, 140,
                                    *reinterpret_cast<float*>(g + 8), words[287] == 1 ? 32 : 0);
    DrawHScroll__10VIUIWindowiiifUi(window, *reinterpret_cast<int*>(window + 20) - 180, 3, 140,
                                    *reinterpret_cast<float*>(g + 12), words[287] == 2 ? 32 : 0);
    DrawFrame__10VIUIWindowiiiiUi(window, *reinterpret_cast<int*>(window + 20) - 176, 4, 132, 1, words[287] == 3 ? 96 : 64);
    TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text_of(sSoundOutputText[g[5]]),
                                              *reinterpret_cast<int*>(window + 20) - 176, 4, 132, 1, 0, 8);
    unsigned short buffer[256];
    str8to16__8VIStringPUsUiPCc(buffer, 512, "|t ");
    strcat16__8VIStringPUsUiPCUs(buffer, 512, GetString__13VIStringTableii(ui_table(), 750, column()));
    Text__10VIUIWindowPCUsii9VIUICOLORUi(window, buffer, 40, 6, 0, 0);
}
}
