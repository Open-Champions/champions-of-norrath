// Observed: DrawInterfaceOptions__12VIWndOptions at 0x010c59a8 is 1192 bytes. With this, g = VIGetGameOptions__Fv(),
// win = this + 408 (an embedded VIUIWindow whose int at +20 is w), sel the signed word at +1148, mUI the word
// _5VIWnd$mUI, ext = (VIGetFeatureFlags__Fv() & 0x200) != 0 (the flag function is called three times: after the label
// list, after the value rows and at the footer):
//  1. the word at +1152 is set to 10, 0, 1, 6, 11 for sel = 0..4 (unsigned compare, so a larger or negative sel leaves
//     it alone);
//  2. Draw__10VIUIWindow(win); labels via TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(win, text, 40, k, w - 80, 1, color,
//     0) for k = 1..4 with text T__10VIUIWindowPCc(win, "Auto Attack") for k = 1 and
//     GetString__13VIStringTableii(mUI, id, the word at mUI + 48) with ids 747, 752, 794 for k = 2, 3, 4, and a fifth
//     label k = 5 with id 1604 only when ext; color 1 when sel == k - 1, else 0;
//  3. TickR__10VIUIWindowiib(win, 36, sel + 1, 1);
//  4. for k = 1..4, and k = 5 only when ext: DrawFrame__10VIUIWindowiiiiUi(win, w - 176, k, 132, 1, flags) with flags 96
//     when sel == k - 1, else 64; then TextBox(win, text, w - 176, k, 132, 1, 0, 8) where text is the pointer
//     sOnOffText[the byte at this+1176] (k = 1), sCameraTypeText[g[6]] (k = 2), sCameraAxisText[g[7]] (k = 3),
//     sCompactKeyboardText[g[145]] (k = 4), sOnOffText[the byte at this+1177] (k = 5); byte indices are not range checked;
//  5. a 512-byte buffer of 16-bit units gets str8to16("|t ") then strcat16 of GetString(mUI, 750, column); then
//     Text__10VIUIWindowPCUsii9VIUICOLORUi(win, buffer, 40, row, 0, 0) with row 7 when ext, else 6.
// The strings were read from the original's read-only data. The jump table for step 1 is at 0x011d5470.
// Inferred: the interface page: auto-attack toggle, camera type and axis, keyboard layout and (with the extended
// feature flag) one more toggle; the row ids match the Increment/Decrement__12VIWndOptions switch rows 6..11.
// Unknown: the meaning of feature flag bit 0x200, the colour enum name and the result type (void here).
extern "C" {
extern unsigned _5VIWnd$mUI;
extern unsigned sOnOffText[2] __asm__("VIWndOptions$sOnOffText");
extern unsigned sCameraTypeText[2] __asm__("VIWndOptions$sCameraTypeText");
extern unsigned sCameraAxisText[2] __asm__("VIWndOptions$sCameraAxisText");
extern unsigned sCompactKeyboardText[2] __asm__("VIWndOptions$sCompactKeyboardText");
unsigned char* VIGetGameOptions__Fv();
unsigned VIGetFeatureFlags__Fv();
void Draw__10VIUIWindow(void* window);
const unsigned short* T__10VIUIWindowPCc(void* window, const char* text);
const unsigned short* GetString__13VIStringTableii(const void* table, int id, int column);
void str8to16__8VIStringPUsUiPCc(unsigned short* destination, unsigned size, const char* source);
void strcat16__8VIStringPUsUiPCUs(unsigned short* destination, unsigned size, const unsigned short* source);
int TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int c, int d, int color,
                                              unsigned flags);
int Text__10VIUIWindowPCUsii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int color, unsigned flags);
void TickR__10VIUIWindowiib(void* window, int a, int b, bool c);
void DrawFrame__10VIUIWindowiiiiUi(void* window, int a, int b, int c, int d, unsigned flags);

static inline const void* ui_table() {
    return reinterpret_cast<const void*>(static_cast<unsigned long>(_5VIWnd$mUI));
}
static inline int column() {
    return *reinterpret_cast<const int*>(static_cast<unsigned long>(_5VIWnd$mUI) + 48);
}
static inline const unsigned short* text_of(unsigned word) {
    return reinterpret_cast<const unsigned short*>(static_cast<unsigned long>(word));
}

void DrawInterfaceOptions__12VIWndOptions(void* self) {
    int* words = static_cast<int*>(self);
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned char* g = VIGetGameOptions__Fv();
    switch (static_cast<unsigned>(words[287])) {
    case 0: words[288] = 10; break;
    case 1: words[288] = 0; break;
    case 2: words[288] = 1; break;
    case 3: words[288] = 6; break;
    case 4: words[288] = 11; break;
    default: break;
    }
    unsigned char* window = bytes + 408;
    Draw__10VIUIWindow(window);
    TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, T__10VIUIWindowPCc(window, "Auto Attack"), 40, 1,
                                              *reinterpret_cast<int*>(window + 20) - 80, 1, words[287] == 0, 0);
    static const int ids[3] = {747, 752, 794};
    for (int k = 2; k <= 4; ++k) {
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, GetString__13VIStringTableii(ui_table(), ids[k - 2], column()), 40,
                                                  k, *reinterpret_cast<int*>(window + 20) - 80, 1, words[287] == k - 1, 0);
    }
    if (VIGetFeatureFlags__Fv() & 0x200) {
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, GetString__13VIStringTableii(ui_table(), 1604, column()), 40, 5,
                                                  *reinterpret_cast<int*>(window + 20) - 80, 1, words[287] == 4, 0);
    }
    TickR__10VIUIWindowiib(window, 36, words[287] + 1, true);
    for (int k = 1; k <= 4; ++k) {
        DrawFrame__10VIUIWindowiiiiUi(window, *reinterpret_cast<int*>(window + 20) - 176, k, 132, 1, words[287] == k - 1 ? 96 : 64);
        unsigned word;
        if (k == 1) word = sOnOffText[bytes[1176]];
        else if (k == 2) word = sCameraTypeText[g[6]];
        else if (k == 3) word = sCameraAxisText[g[7]];
        else word = sCompactKeyboardText[g[145]];
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text_of(word), *reinterpret_cast<int*>(window + 20) - 176, k, 132, 1,
                                                  0, 8);
    }
    if (VIGetFeatureFlags__Fv() & 0x200) {
        DrawFrame__10VIUIWindowiiiiUi(window, *reinterpret_cast<int*>(window + 20) - 176, 5, 132, 1, words[287] == 4 ? 96 : 64);
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text_of(sOnOffText[bytes[1177]]),
                                                  *reinterpret_cast<int*>(window + 20) - 176, 5, 132, 1, 0, 8);
    }
    unsigned short buffer[256];
    str8to16__8VIStringPUsUiPCc(buffer, 512, "|t ");
    strcat16__8VIStringPUsUiPCUs(buffer, 512, GetString__13VIStringTableii(ui_table(), 750, column()));
    if (VIGetFeatureFlags__Fv() & 0x200) {
        Text__10VIUIWindowPCUsii9VIUICOLORUi(window, buffer, 40, 7, 0, 0);
    } else {
        Text__10VIUIWindowPCUsii9VIUICOLORUi(window, buffer, 40, 6, 0, 0);
    }
}
}
