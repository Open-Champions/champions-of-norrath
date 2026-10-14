// Observed: ProcessScreen20__10VIWndMcErriii at 0x0109dba0 is 432 bytes. With (this, action, param1, param2) it
// calls scePrintf("VIWndMcErr::ProcessScreen20(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1,
// param2). With ui = the word _5VIWnd$mUI and column = the word at ui +48 it then builds a 16-bit text in a
// local 2048-byte area: snprintf16__8VIStringPUsiPCce(text, 2048, "%S\n\n%S", GetString(ui, 1626, column),
// GetString(ui, 957, column)). Then, with window = this + 40:
//  - action 0 (draw): n = CountLines(window, text, 430); Init(window, ui, {0, 0}, this + 420, 470, n + 2, 0);
//    Begin2D(ui); Draw(window); TextBox(window, text, 20, 0, width - 40, n, 0, 8) with width the word at
//    window +20; DrawPrompt__10VIWndMcErriPCUsT2(this, n + 1, GetString(ui, 786, column), 0); End2D(ui). Unlike
//    screens 10 to 17, Open is not called;
//  - action 3 with param1 = 134: goes to screen 30 by scePrintf("VIWndMcErr::GotoScreen %d\n", 30) and the
//    stores +412 = 30, +408 = 0, +416 = 0;
//  - every other action (and action 3 with another param1) does nothing.
// It returns 0.
// Inferred: a notice screen whose body joins two strings (1626 and 957); the confirm key moves on to screen 30.
// Unknown: what the strings say, the meaning of screen 30 here, why this screen does not open the window (the
// window is presumably already open from the screen that led here), the field names, whether the 2048 passed to
// snprintf16 is a byte or unit count (the area is 2048 bytes), and the type of the result (zero is returned).
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
int scePrintf(const char* format, ...);
extern unsigned _5VIWnd$mUI;
const unsigned short* GetString__13VIStringTableii(const void* table, int id, int column);
int CountLines__10VIUIWindowPCUsi(void* window, const unsigned short* text, int width);
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, void* ui, const int* position, const int* size, int width,
                                               int a, int b);
void Begin2D__4VIUI(void* ui);
void End2D__4VIUI(void* ui);
void Draw__10VIUIWindow(void* window);
void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int c, int d,
                                               int color, unsigned e);
void DrawPrompt__10VIWndMcErriPCUsT2(void* self, int a, const unsigned short* text, const unsigned short* text2);
int snprintf16__8VIStringPUsiPCce(unsigned short* buffer, int size, const char* format, ...);

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(NORRATH_TRANSLATE(word));
}
static inline void goto_screen(int* words, int screen) {
    scePrintf("VIWndMcErr::GotoScreen %d\n", screen);
    words[103] = screen;
    words[102] = 0;
    words[104] = 0;
}

int ProcessScreen20__10VIWndMcErriii(void* self, int action, int param1, int param2) {
    int* words = static_cast<int*>(self);
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned short text[1024];
    scePrintf("VIWndMcErr::ProcessScreen20(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1, param2);
    void* const ui = pointer_of(_5VIWnd$mUI);
    const unsigned short* first = GetString__13VIStringTableii(ui, 1626, static_cast<int*>(ui)[12]);
    const unsigned short* second = GetString__13VIStringTableii(ui, 957, static_cast<int*>(ui)[12]);
    snprintf16__8VIStringPUsiPCce(text, 2048, "%S\n\n%S", first, second);
    void* const window = bytes + 40;
    if (action == 0) {
        const int lines = CountLines__10VIUIWindowPCUsi(window, text, 430);
        int position[2] = {0, 0};
        Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, pointer_of(_5VIWnd$mUI), position, words + 105, 470, lines + 2, 0);
        Begin2D__4VIUI(ui);
        Draw__10VIUIWindow(window);
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text, 20, 0, static_cast<int*>(window)[5] - 40, lines, 0, 8);
        void* const ui2 = pointer_of(_5VIWnd$mUI);
        DrawPrompt__10VIWndMcErriPCUsT2(self, lines + 1, GetString__13VIStringTableii(ui2, 786, static_cast<int*>(ui2)[12]), 0);
        End2D__4VIUI(ui);
    } else if (action == 3 && param1 == 134) {
        goto_screen(words, 30);
    }
    return 0;
}
}
