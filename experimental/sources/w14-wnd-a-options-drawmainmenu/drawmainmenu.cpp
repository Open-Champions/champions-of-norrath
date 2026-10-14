// Observed: DrawMainMenu__12VIWndOptions at 0x010c5348 is 844 bytes. With this, win = this + 40 (an embedded
// VIUIWindow whose int at +20 is w), sel the signed word at +1180 and mUI the word _5VIWnd$mUI, and a 512-byte stack
// buffer of 16-bit units (the 512 passed to the string helpers is a byte count), it stores -1 in the word at +1152 and
// then calls Draw__10VIUIWindow(win). For the four items k = 0..3 with string ids 773, 778, 800, 482:
//   buffer = (sel == k) ? str8to16("|q") : "" (the first unit zeroed); then strcat16 of
//   GetString__13VIStringTableii(mUI, id, the word at mUI + 48); and, when sel == k, strcat16 of the text "|w";
//   then TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(win, buffer, 40, k + 1, w - 80, 1, color, 8) with color 1 when
//   sel == k, else 0.
// Finally: buffer = str8to16("|x "), strcat16 of GetString(mUI, 212, column), strcat16 of the text "   |t ", strcat16
// of GetString(mUI, 753, column), and Text__10VIUIWindowPCUsii9VIUICOLORUi(win, buffer, 40, 6, 0, 0).
// The strings "|q", "|w", "|x " and "   |t " were read from the original's read-only data; the strcat16 and str8to16
// helpers are the char-pointer or 16-bit-pointer overloads as noted by the calls below.
// Inferred: this draws the four-entry main options list with a selection marker made of font control codes, and a
// footer line with two button prompts; the word at +1152 is a "current row id" reset to none here.
// Unknown: what the control codes draw, the enum name of the colour, and the result type (void here).
extern "C" {
extern unsigned _5VIWnd$mUI;
void Draw__10VIUIWindow(void* window);
const unsigned short* GetString__13VIStringTableii(const void* table, int id, int column);
void str8to16__8VIStringPUsUiPCc(unsigned short* destination, unsigned size, const char* source);
void strcat16__8VIStringPUsUiPCUs(unsigned short* destination, unsigned size, const unsigned short* source);
void strcat16__8VIStringPUsUiPCc(unsigned short* destination, unsigned size, const char* source);
int TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int c, int d, int color,
                                              unsigned flags);
int Text__10VIUIWindowPCUsii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int color, unsigned flags);

static inline const void* ui_table() {
    return reinterpret_cast<const void*>(static_cast<unsigned long>(_5VIWnd$mUI));
}
static inline int column() {
    return *reinterpret_cast<const int*>(static_cast<unsigned long>(_5VIWnd$mUI) + 48);
}

void DrawMainMenu__12VIWndOptions(void* self) {
    int* words = static_cast<int*>(self);
    unsigned char* window = static_cast<unsigned char*>(self) + 40;
    unsigned short buffer[256];
    words[288] = -1;
    Draw__10VIUIWindow(window);
    static const int ids[4] = {773, 778, 800, 482};
    for (int k = 0; k < 4; ++k) {
        if (words[295] != k) {
            buffer[0] = 0;
        } else {
            str8to16__8VIStringPUsUiPCc(buffer, 512, "|q");
        }
        strcat16__8VIStringPUsUiPCUs(buffer, 512, GetString__13VIStringTableii(ui_table(), ids[k], column()));
        if (words[295] == k) strcat16__8VIStringPUsUiPCc(buffer, 512, "|w");
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, buffer, 40, k + 1, *reinterpret_cast<int*>(window + 20) - 80, 1,
                                                  words[295] == k, 8);
    }
    str8to16__8VIStringPUsUiPCc(buffer, 512, "|x ");
    strcat16__8VIStringPUsUiPCUs(buffer, 512, GetString__13VIStringTableii(ui_table(), 212, column()));
    strcat16__8VIStringPUsUiPCc(buffer, 512, "   |t ");
    strcat16__8VIStringPUsUiPCUs(buffer, 512, GetString__13VIStringTableii(ui_table(), 753, column()));
    Text__10VIUIWindowPCUsii9VIUICOLORUi(window, buffer, 40, 6, 0, 0);
}
}
