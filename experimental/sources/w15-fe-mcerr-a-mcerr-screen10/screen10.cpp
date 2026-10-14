// Observed: ProcessScreen10__10VIWndMcErriii at 0x0109cd10 is 460 bytes. With (this, action, param1, param2) it
// calls scePrintf("VIWndMcErr::ProcessScreen10(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1,
// param2) and then, with ui = the word _5VIWnd$mUI and window = this + 40:
//  - action 0 (draw): text = GetString__13VIStringTableii(ui, 962, ui word +48); n = CountLines(window, text, 510);
//    Init(window, ui, {0, 0}, this + 420, 550, n + 2, 0); Open(window, 1); Begin2D(ui); Draw(window);
//    TextBox(window, GetString(ui, 962, column), 20, 0, width - 40, n, 0, 8) where width is the word at
//    window +20; DrawPrompt__10VIWndMcErriPCUsT2(this, n + 1, GetString(ui, 786, column), 0); End2D(ui);
//  - action 3 with param1 = 134: VIMemCardStatus__FPi(0); a result of 1 or 5 goes to screen 15 and any other
//    result to screen 4, by scePrintf("VIWndMcErr::GotoScreen %d\n", n) and the stores +412 = n, +408 = 0,
//    +416 = 0;
//  - every other action (and action 3 with another param1) does nothing.
// It returns 0.
// Inferred: a memory-card error message screen (string 962) with a prompt line (string 786); action 0 draws
// and action 3 is a key press, where 134 is the confirm key as used elsewhere in the front end; the screen
// moves to screen 15 after a status of 1 or 5 and back to screen 4 otherwise.
// Unknown: what the status values mean, the text of strings 962 and 786, the meaning of the extra 2 added to
// the window height and of the 550 and 510 widths beyond layout, the names of the fields, and the type of the
// result (zero is returned).
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
void Open__10VIUIWindowb(void* window, bool open);
void Begin2D__4VIUI(void* ui);
void End2D__4VIUI(void* ui);
void Draw__10VIUIWindow(void* window);
void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int c, int d,
                                               int color, unsigned e);
void DrawPrompt__10VIWndMcErriPCUsT2(void* self, int a, const unsigned short* text, const unsigned short* text2);
int VIMemCardStatus__FPi(int* value);

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(NORRATH_TRANSLATE(word));
}
static inline void goto_screen(int* words, int screen) {
    scePrintf("VIWndMcErr::GotoScreen %d\n", screen);
    words[103] = screen;
    words[102] = 0;
    words[104] = 0;
}

int ProcessScreen10__10VIWndMcErriii(void* self, int action, int param1, int param2) {
    int* words = static_cast<int*>(self);
    unsigned char* bytes = static_cast<unsigned char*>(self);
    scePrintf("VIWndMcErr::ProcessScreen10(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1, param2);
    void* const window = bytes + 40;
    void* const ui = pointer_of(_5VIWnd$mUI);
    if (action == 0) {
        const unsigned short* text = GetString__13VIStringTableii(ui, 962, static_cast<int*>(ui)[12]);
        const int lines = CountLines__10VIUIWindowPCUsi(window, text, 510);
        int position[2] = {0, 0};
        Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, pointer_of(_5VIWnd$mUI), position, words + 105, 550, lines + 2, 0);
        Open__10VIUIWindowb(window, true);
        Begin2D__4VIUI(ui);
        Draw__10VIUIWindow(window);
        void* const ui2 = pointer_of(_5VIWnd$mUI);
        text = GetString__13VIStringTableii(ui2, 962, static_cast<int*>(ui2)[12]);
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text, 20, 0, static_cast<int*>(window)[5] - 40, lines, 0, 8);
        void* const ui3 = pointer_of(_5VIWnd$mUI);
        DrawPrompt__10VIWndMcErriPCUsT2(self, lines + 1, GetString__13VIStringTableii(ui3, 786, static_cast<int*>(ui3)[12]), 0);
        End2D__4VIUI(ui);
        return 0;
    }
    if (action == 3 && param1 == 134) {
        const int status = VIMemCardStatus__FPi(0);
        goto_screen(words, status == 1 || status == 5 ? 15 : 4);
    }
    return 0;
}
}
