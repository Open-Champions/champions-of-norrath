// Observed: OnDrawSelf__16VIWndReadMessage at 0x010d11f0 is 360 bytes. With (this), ui being the word
// _5VIWnd$mUI (0x01192c70), w = this + 40 and text = this + 412 (16-bit text):
// Begin2D__4VIUI(ui); Draw__10VIUIWindow(w); when the word at +16804 is nonzero: n =
// CountLines__10VIUIWindowPCUsi(w, text, 360), then the words at +16800, +16796 and +16804 become 0, n and 0.
// Then with lines = the word at +16796 and top = the word at +16800: DrawFrameVScroll__10VIUIWindowiiiif(w, 40, 1,
// width - 80, 7, f) where width is the word at w + 20 and f is (float)top / (float)(lines - 7) when lines is 8
// or more, else -1.0f; TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(w, text, 40, 1, width - 80, 7, 0, top << 24);
// a 512-unit buffer is filled with str8to16__8VIStringPUsUiPCc(buffer, 512, "|t ") and
// strcat16__8VIStringPUsUiPCUs(buffer, 512, GetString__13VIStringTableii(ui, 750, the word at ui + 48)); then
// Text__10VIUIWindowPCUsii9VIUICOLORUi(w, buffer, 40, 9, 0, 0); End2D__4VIUI(ui). The word at ui is re-read for
// the GetString call; the earlier ui is used for Begin2D and End2D.
// Inferred: a scrolled text box with a footer line; +16800 is the first visible line, +16796 the line count and
// +16804 a request to recount; the string "|t " looks like markup for the footer, from the text only.
// Unknown: the meaning of the TextBox, Text and DrawFrameVScroll integer arguments (register order only), the
// meaning of "|t ", the field names and the return type (void).
extern "C" {
void Begin2D__4VIUI(void* ui);
void End2D__4VIUI(void* ui);
void Draw__10VIUIWindow(void* window);
int CountLines__10VIUIWindowPCUsi(void* window, const unsigned short* text, int width);
void DrawFrameVScroll__10VIUIWindowiiiif(void* window, int a, int b, int c, int d, float scroll);
void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int c, int d,
                                               int color, unsigned e);
void Text__10VIUIWindowPCUsii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int color, unsigned c);
void str8to16__8VIStringPUsUiPCc(unsigned short* dest, unsigned size, const char* source);
void strcat16__8VIStringPUsUiPCUs(unsigned short* dest, unsigned size, const unsigned short* source);
const unsigned short* GetString__13VIStringTableii(const void* table, int id, int column);
extern unsigned _5VIWnd$mUI;

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<unsigned long>(word));
}

void OnDrawSelf__16VIWndReadMessage(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    int* words = static_cast<int*>(self);
    void* ui = pointer_of(_5VIWnd$mUI);
    void* window = bytes + 40;
    const unsigned short* text = reinterpret_cast<const unsigned short*>(bytes + 412);
    unsigned short buffer[512];
    Begin2D__4VIUI(ui);
    Draw__10VIUIWindow(window);
    if (words[4201] != 0) {
        const int lines = CountLines__10VIUIWindowPCUsi(window, text, 360);
        words[4200] = 0;
        words[4199] = lines;
        words[4201] = 0;
    }
    const int lines = words[4199];
    const int width = words[15];
    float scroll = -1.0f;
    if (lines >= 8) scroll = static_cast<float>(words[4200]) / static_cast<float>(lines - 7);
    DrawFrameVScroll__10VIUIWindowiiiif(window, 40, 1, width - 80, 7, scroll);
    TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text, 40, 1, width - 80, 7, 0,
                                              static_cast<unsigned>(words[4200]) << 24);
    str8to16__8VIStringPUsUiPCc(buffer, 512, "|t ");
    const void* table = pointer_of(_5VIWnd$mUI);
    strcat16__8VIStringPUsUiPCUs(buffer, 512, GetString__13VIStringTableii(table, 750, static_cast<const int*>(table)[12]));
    Text__10VIUIWindowPCUsii9VIUICOLORUi(window, buffer, 40, 9, 0, 0);
    End2D__4VIUI(ui);
}
}
