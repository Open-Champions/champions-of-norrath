// Observed: DrawGameOptions__12VIWndOptions at 0x010c5698 is 784 bytes. With this, g = VIGetGameOptions__Fv(), win =
// this + 408 (an embedded VIUIWindow whose int at +20 is w), sel the signed word at +1148, mUI the word _5VIWnd$mUI and
// sOnOffText the two-word table at 0x011e8c08:
//  1. the word at +1152 is set to 9 when sel is 1, 8 when sel is 0, 7 when sel is 2, and otherwise left alone;
//  2. Draw__10VIUIWindow(win); then three TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(win, text, 40, k, w - 80, 1,
//     color, 0) calls for k = 1, 2, 3 with text GetString__13VIStringTableii(mUI, id, the word at mUI + 48) for ids 738,
//     739, 759, and color 1 when sel == k - 1, else 0;
//  3. TickR__10VIUIWindowiib(win, 36, sel + 1, 1);
//  4. for k = 1, 2, 3: DrawFrame__10VIUIWindowiiiiUi(win, w - 176, k, 132, 1, flags) with flags 96 when sel == k - 1,
//     else 64; then TextBox(win, text, w - 176, k, 132, 1, 0, 8) where text is the pointer sOnOffText[n] with n the
//     byte at this+1174 (k = 1), the byte at this+1175 (k = 2), and (g[146] == 0) (k = 3);
//  5. a 512-byte buffer of 16-bit units gets str8to16("|t ") then strcat16 of GetString(mUI, 750, column); then
//     Text__10VIUIWindowPCUsii9VIUICOLORUi(win, buffer, 40, 5, 0, 0).
// The string "|t " was read from the original's read-only data. The table words are not checked against the table
// size (a byte index above 1 reads past it).
// Inferred: this draws the three-entry gameplay options page: left-hand labels, framed on/off values, and a footer
// prompt; the rows with ids 8, 9 and 7 match the toggles that Increment/Decrement__12VIWndOptions perform.
// Unknown: the meaning of the row ids, the colour enum name, and the result type (void here).
extern "C" {
extern unsigned _5VIWnd$mUI;
extern unsigned sOnOffText[2] __asm__("VIWndOptions$sOnOffText");
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

static inline const void* ui_table() {
    return reinterpret_cast<const void*>(static_cast<unsigned long>(_5VIWnd$mUI));
}
static inline int column() {
    return *reinterpret_cast<const int*>(static_cast<unsigned long>(_5VIWnd$mUI) + 48);
}
static inline const unsigned short* table_text(unsigned index) {
    return reinterpret_cast<const unsigned short*>(static_cast<unsigned long>(
        *(sOnOffText + index)));
}

void DrawGameOptions__12VIWndOptions(void* self) {
    int* words = static_cast<int*>(self);
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned char* g = VIGetGameOptions__Fv();
    const int sel = words[287];
    if (sel == 1) words[288] = 9;
    else if (sel == 0) words[288] = 8;
    else if (sel == 2) words[288] = 7;
    unsigned char* window = bytes + 408;
    Draw__10VIUIWindow(window);
    static const int ids[3] = {738, 739, 759};
    for (int k = 1; k <= 3; ++k) {
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, GetString__13VIStringTableii(ui_table(), ids[k - 1], column()), 40,
                                                  k, *reinterpret_cast<int*>(window + 20) - 80, 1, words[287] == k - 1, 0);
    }
    TickR__10VIUIWindowiib(window, 36, words[287] + 1, true);
    for (int k = 1; k <= 3; ++k) {
        DrawFrame__10VIUIWindowiiiiUi(window, *reinterpret_cast<int*>(window + 20) - 176, k, 132, 1, words[287] == k - 1 ? 96 : 64);
        unsigned index;
        if (k == 1) index = bytes[1174];
        else if (k == 2) index = bytes[1175];
        else index = g[146] == 0;
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, table_text(index), *reinterpret_cast<int*>(window + 20) - 176, k, 132, 1,
                                                  0, 8);
    }
    unsigned short buffer[256];
    str8to16__8VIStringPUsUiPCc(buffer, 512, "|t ");
    strcat16__8VIStringPUsUiPCUs(buffer, 512, GetString__13VIStringTableii(ui_table(), 750, column()));
    Text__10VIUIWindowPCUsii9VIUICOLORUi(window, buffer, 40, 5, 0, 0);
}
}
