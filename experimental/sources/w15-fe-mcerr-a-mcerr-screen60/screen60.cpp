// Observed: ProcessScreen60__10VIWndMcErriii at 0x0109e638 is 524 bytes. With (this, action, param1, param2) it
// calls scePrintf("VIWndMcErr::ProcessScreen60(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1,
// param2). With ui = the word _5VIWnd$mUI and window = this + 40:
//  - action 0 (draw): n = CountLines(window, GetString__13VIStringTableii(ui, 960, ui word +48), 510);
//    Init(window, ui, {0, 0}, this + 420, 550, n + 2, 0) (no Open call); Begin2D(ui); Draw(window);
//    TextBox(window, GetString(ui, 960, column), 20, 0, width - 40, n, 0, 8) with width the word at window +20;
//    DrawPrompt__10VIWndMcErriPCUsT2(this, n + 1, GetString(ui, 786, column), GetString(ui, 753, column));
//    End2D(ui);
//  - action 3 (key press) on param1: 136 stores 0 in +408 and 137 stores 1 there; 134 goes to screen 20 when
//    +408 is 1; otherwise VIMemCardStatus__FPi(0) is called and a result of 1 also goes to screen 20, any other
//    result to screen 30; "goes to screen n" is scePrintf("VIWndMcErr::GotoScreen %d\n", n) and the stores
//    +412 = n, +408 = 0, +416 = 0; other keys do nothing;
//  - every other action does nothing.
// It returns 0.
// Inferred: a two-choice screen shown when the card has too little space (ProcessScreen30 goes to 60 when its
// figure is below 3000): +408 is the highlighted choice (keys 136 and 137 move it), 134 confirms; choice 1 goes
// back to the card-check screen 20, choice 0 re-checks the card.
// Unknown: what strings 960 and 753 say, which key codes are 134, 136 and 137, the field names, and the type of
// the result (zero is returned).
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

int ProcessScreen60__10VIWndMcErriii(void* self, int action, int param1, int param2) {
    int* words = static_cast<int*>(self);
    unsigned char* bytes = static_cast<unsigned char*>(self);
    scePrintf("VIWndMcErr::ProcessScreen60(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1, param2);
    void* const ui = pointer_of(_5VIWnd$mUI);
    if (action == 0) {
        void* const window = bytes + 40;
        const unsigned short* text = GetString__13VIStringTableii(ui, 960, static_cast<int*>(ui)[12]);
        const int lines = CountLines__10VIUIWindowPCUsi(window, text, 510);
        int position[2] = {0, 0};
        Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, pointer_of(_5VIWnd$mUI), position, words + 105, 550, lines + 2, 0);
        Begin2D__4VIUI(ui);
        Draw__10VIUIWindow(window);
        void* const ui2 = pointer_of(_5VIWnd$mUI);
        text = GetString__13VIStringTableii(ui2, 960, static_cast<int*>(ui2)[12]);
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text, 20, 0, static_cast<int*>(window)[5] - 40, lines, 0, 8);
        void* const ui3 = pointer_of(_5VIWnd$mUI);
        const unsigned short* label = GetString__13VIStringTableii(ui3, 786, static_cast<int*>(ui3)[12]);
        void* const ui4 = pointer_of(_5VIWnd$mUI);
        DrawPrompt__10VIWndMcErriPCUsT2(self, lines + 1, label, GetString__13VIStringTableii(ui4, 753, static_cast<int*>(ui4)[12]));
        End2D__4VIUI(ui);
        return 0;
    }
    if (action == 3) {
        if (param1 == 136) {
            words[102] = 0;
        } else if (param1 == 137) {
            words[102] = 1;
        } else if (param1 == 134) {
            if (words[102] == 1 || VIMemCardStatus__FPi(0) == 1) {
                goto_screen(words, 20);
            } else {
                goto_screen(words, 30);
            }
        }
    }
    return 0;
}
}
