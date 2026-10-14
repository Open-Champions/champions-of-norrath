// Observed: DrawPrompt__10VIWndMcErriPCUsT2 at 0x0109f310 is 516 bytes. With (this, row, first, second), where
// first and second are 16-bit strings, it calls scePrintf("VIWndMcErr::DrawPrompt()\n") and takes
// window = this + 40, width = the word at window +20, third = width / 3 (signed) and half = width >> 1
// (arithmetic). It builds the text in a local 512-byte 16-bit area and draws it with
// TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text, x, row, 400, 1, flag, 8). The "|q" and "|w" below
// are the 8-bit strings at 0x011d3598 and 0x011d35a0:
//  - second null: text = "|q" + first + "|w" (str8to16, then strcat16 with a 16-bit and an 8-bit source);
//    one box at x = half - 200 with flag 1;
//  - second not null, with s = the word at +408 (re-read before each box): the first box has text = "|q" +
//    first + "|w" when s is 0 and a plain copy of first (strcpy16__8VIStringPUsUiPCUs) otherwise, at
//    x = third - 200 with flag (s == 0); the second box has text = "|q" + second + "|w" when s is 1 and a plain
//    copy of second otherwise, at x = 2 * third - 200 with flag (s == 1).
// Nothing is returned.
// Inferred: this draws the one- or two-label button line at the bottom of a message screen; +408 is the
// highlighted choice (the selected label gets the "|q"/"|w" markup, which looks like a highlight on/off pair, and
// the box flag), and row is the text row (the screens pass the line count plus one).
// Unknown: what "|q" and "|w" mean to the text drawer, the TextBox integer arguments (register order only),
// the field names, and the exact type of the flag.
extern "C" {
int scePrintf(const char* format, ...);
void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int c, int d,
                                               int color, unsigned e);
void str8to16__8VIStringPUsUiPCc(unsigned short* dest, unsigned size, const char* source);
void strcat16__8VIStringPUsUiPCUs(unsigned short* dest, unsigned size, const unsigned short* source);
void strcat16__8VIStringPUsUiPCc(unsigned short* dest, unsigned size, const char* source);
unsigned short* strcpy16__8VIStringPUsUiPCUs(unsigned short* dest, unsigned size, const unsigned short* source);

static inline void highlighted(unsigned short* text, const unsigned short* label) {
    str8to16__8VIStringPUsUiPCc(text, 512, "|q");
    strcat16__8VIStringPUsUiPCUs(text, 512, label);
    strcat16__8VIStringPUsUiPCc(text, 512, "|w");
}

void DrawPrompt__10VIWndMcErriPCUsT2(void* self, int row, const unsigned short* first, const unsigned short* second) {
    int* words = static_cast<int*>(self);
    void* const window = static_cast<unsigned char*>(self) + 40;
    unsigned short text[256];
    scePrintf("VIWndMcErr::DrawPrompt()\n");
    const int width = static_cast<int*>(window)[5];
    const int third = width / 3;
    const int half = width >> 1;
    if (second == nullptr) {
        highlighted(text, first);
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text, half - 200, row, 400, 1, 1, 8);
        return;
    }
    if (words[102] == 0) {
        highlighted(text, first);
    } else {
        strcpy16__8VIStringPUsUiPCUs(text, 512, first);
    }
    TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text, third - 200, row, 400, 1, words[102] == 0, 8);
    if (words[102] == 1) {
        highlighted(text, second);
    } else {
        strcpy16__8VIStringPUsUiPCUs(text, 512, second);
    }
    TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text, third * 2 - 200, row, 400, 1, words[102] == 1, 8);
}
}
