// Observed: OnDrawSelf__9VIWndEula at 0x010b4fe8 is 564 bytes. mUI is the word _5VIWnd$mUI
// (0x01192c70); w is the embedded UI window at this+40; GetString__13VIStringTableii is called as
// (mUI, id, word at mUI+48). In order:
//  1. When the word at this+9764 is nonzero: ResetTickPulse__10VIUIWindow(w).
//  2. When the word at this+1548 is nonzero: ComputeTextRows__9VIWndEula(this), then zero this+1548 and this+1556.
//  3. When the word at this+9756 is nonzero: decrement it; when that leaves zero, store 2 there and call
//     Down__9VIWndEula(this) if the word at this+9760 is above zero, else Up__9VIWndEula(this).
//  4. Begin2D__4VIUI(mUI); Draw__10VIUIWindow(w);
//     DrawFrameVScroll__10VIUIWindowiiiif(w, 40, 0, (word at w+20) - 80, 16, (float)(word at this+1556) /
//     (float)((word at this+9752) - 17)).
//  5. Rows: the text table is 8-byte records from this+1560, record = {pointer to 16-bit text, halfword
//     length at +4}. Starting at record number top = word at this+1556, for up to 16 rows while the record's
//     pointer is nonzero: strcpy16__8VIStringPUsUiPCUs(buffer, length * 2, text), zero the unit at buffer[length],
//     Text__10VIUIWindowPCUsii9VIUICOLORUi(w, buffer, 40, row, 0, 0).
//  6. snprintf16__8VIStringPUsiPCce(buffer2, 1024, "|c %S", GetString(mUI, 750, mUI[48]));
//     Text(w, buffer2, ((word at w+20) / 2) - 40, 17, (word at this+1544) == 0, 0); End2D__4VIUI(mUI).
// Inferred: a scrolling license-text view with a scroll bar and one footer line (text 750 under the
// "|c" escape), the footer colour flag depending on the selection word at this+1544; the word at this+9756 is a
// key-repeat countdown whose direction is the word at this+9760 (see OnKeyDown__9VIWndEula).
// Unknown: the buffers' true capacity (the original frame reserves 512 bytes then 1024), the meaning of "|c",
// the type behind the colour argument, and whether 1556 counts rows or records.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<__UINTPTR_TYPE__>(norrath_host_pointer(word))
#else
#define NORRATH_WORD_ADDRESS(word) static_cast<__UINTPTR_TYPE__>(word)
#endif
extern "C" {
extern unsigned _5VIWnd$mUI;
void ResetTickPulse__10VIUIWindow(void* window);
void ComputeTextRows__9VIWndEula(void* self);
void Down__9VIWndEula(void* self);
void Up__9VIWndEula(void* self);
void Begin2D__4VIUI(unsigned ui);
void End2D__4VIUI(unsigned ui);
void Draw__10VIUIWindow(void* window);
void DrawFrameVScroll__10VIUIWindowiiiif(void* window, int a, int b, int c, int d, float fraction);
void strcpy16__8VIStringPUsUiPCUs(unsigned short* destination, unsigned size, const unsigned short* source);
void Text__10VIUIWindowPCUsii9VIUICOLORUi(void* window, const unsigned short* text, int x, int y, int color,
                                          unsigned flags);
int snprintf16__8VIStringPUsiPCce(unsigned short* destination, int size, const char* format, ...);
const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg);

static inline int& word(void* base, int offset) {
    return *reinterpret_cast<int*>(static_cast<unsigned char*>(base) + offset);
}

void OnDrawSelf__9VIWndEula(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    void* ui = reinterpret_cast<void*>(NORRATH_WORD_ADDRESS(_5VIWnd$mUI));
    void* window = bytes + 40;
    if (word(self, 9764) != 0) ResetTickPulse__10VIUIWindow(window);
    if (word(self, 1548) != 0) {
        ComputeTextRows__9VIWndEula(self);
        word(self, 1548) = 0;
        word(self, 1556) = 0;
    }
    if (word(self, 9756) != 0) {
        word(self, 9756) = word(self, 9756) - 1;
        if (word(self, 9756) == 0) {
            word(self, 9756) = 2;
            if (word(self, 9760) > 0) Down__9VIWndEula(self);
            else Up__9VIWndEula(self);
        }
    }
    Begin2D__4VIUI(_5VIWnd$mUI);
    Draw__10VIUIWindow(window);
    const float scroll = static_cast<float>(word(self, 1556));
    DrawFrameVScroll__10VIUIWindowiiiif(window, 40, 0, word(window, 20) - 80, 16,
                                        scroll / static_cast<float>(word(self, 9752) - 17));
    unsigned short line[256];
    int row = 0;
    for (;;) {
        unsigned char* record = bytes + 1560 + 8 * (word(self, 1556) + row);
        const unsigned short* text = reinterpret_cast<const unsigned short*>(
            NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(record)));
        if (text == 0) break;
        const int length = *reinterpret_cast<short*>(record + 4);
        strcpy16__8VIStringPUsUiPCUs(line, static_cast<unsigned>(length) * 2, text);
        line[length] = 0;
        Text__10VIUIWindowPCUsii9VIUICOLORUi(window, line, 40, row, 0, 0);
        row += 1;
        if (row >= 16) break;
    }
    unsigned short footer[512];
    snprintf16__8VIStringPUsiPCce(footer, 1024, "|c %S", GetString__13VIStringTableii(ui, 750, word(ui, 48)));
    Text__10VIUIWindowPCUsii9VIUICOLORUi(window, footer, word(window, 20) / 2 - 40, 17, word(self, 1544) == 0, 0);
    End2D__4VIUI(_5VIWnd$mUI);
}
}
