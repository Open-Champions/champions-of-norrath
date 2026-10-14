// Observed: ProcessScreen15__10VIWndMcErriii at 0x0109d410 is 628 bytes. With (this, action, param1, param2) it
// calls scePrintf("VIWndMcErr::ProcessScreen15(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1,
// param2). With ui = the word _5VIWnd$mUI, column = the word at ui +48 and window = this + 40:
//  - action 0 (draw): n = CountLines(window, GetString(ui, 955, column), 510); Init(window, ui, {0, 0},
//    this + 420, 550, n + 2, 0); Open(window, 1); Begin2D(ui); the word at +416 is incremented and, when it
//    becomes exactly 1, the word at +408 is set to 1; Draw(window); TextBox(window, GetString(ui, 955, column),
//    20, 0, width - 40, n, 0, 8) with width the word at window +20; DrawPrompt__10VIWndMcErriPCUsT2(this,
//    n + 1, GetString(ui, 969, column), GetString(ui, 896, column)); End2D(ui);
//  - action 3 (key press) on param1: 136 stores 0 in +408 and 137 stores 1 there; 134 with +408 nonzero goes to
//    screen 4 by scePrintf("VIWndMcErr::GotoScreen %d\n", 4) and the stores +412 = 4, +408 = 0, +416 = 0; 134
//    with +408 zero prints "VIWndMcErr::ExitMcErr(Rval = %d)\n" with 3 and then, with a parent word at +428,
//    sends message 4354 (argument 0) and message 4358 (argument 3) to it, or without a parent stores 3 at +436;
//    other keys do nothing;
//  - every other action does nothing.
// It returns 0. Each message goes through entry 6 (offset 48) of the table at the word parent + 32: a 16-bit
// adjustment, then the function at +4, called as (parent + adjustment, this, message, argument, 0).
// Inferred: a two-choice question screen like ProcessScreen12 but with a fixed message (string 955) and exit
// result 3: +408 is the selected choice (set to 1 on the first drawn frame, moved by keys 136 and 137), 134
// confirms; choice 1 returns to screen 4 and choice 0 leaves the error window with result 3.
// Unknown: what the strings say, which key codes 134, 136 and 137 are, what result 3 means to the caller, the
// field names, and the type of the result (zero is returned).
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

struct VtEntry {
    short delta;
    short index;
    void (*function)(void*, void*, int, int, int);
};

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(NORRATH_TRANSLATE(word));
}
static inline void send_to_parent(void* from, int message, int argument) {
    unsigned char* parent = static_cast<unsigned char*>(pointer_of(static_cast<int*>(from)[107]));
    const VtEntry* entry = reinterpret_cast<const VtEntry*>(pointer_of(*reinterpret_cast<unsigned*>(parent + 32))) + 6;
    entry->function(parent + entry->delta, from, message, argument, 0);
}
static inline void goto_screen(int* words, int screen) {
    scePrintf("VIWndMcErr::GotoScreen %d\n", screen);
    words[103] = screen;
    words[102] = 0;
    words[104] = 0;
}
static inline void exit_mcerr(int* words, int rval) {
    scePrintf("VIWndMcErr::ExitMcErr(Rval = %d)\n", rval);
    if (words[107] != 0) {
        send_to_parent(words, 4354, 0);
        send_to_parent(words, 4358, rval);
    } else {
        words[109] = rval;
    }
}

int ProcessScreen15__10VIWndMcErriii(void* self, int action, int param1, int param2) {
    int* words = static_cast<int*>(self);
    unsigned char* bytes = static_cast<unsigned char*>(self);
    scePrintf("VIWndMcErr::ProcessScreen15(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1, param2);
    void* const ui = pointer_of(_5VIWnd$mUI);
    if (action == 0) {
        void* const window = bytes + 40;
        const unsigned short* text = GetString__13VIStringTableii(ui, 955, static_cast<int*>(ui)[12]);
        const int lines = CountLines__10VIUIWindowPCUsi(window, text, 510);
        int position[2] = {0, 0};
        Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, pointer_of(_5VIWnd$mUI), position, words + 105, 550, lines + 2, 0);
        Open__10VIUIWindowb(window, true);
        Begin2D__4VIUI(ui);
        const int frame = words[104] + 1;
        words[104] = frame;
        if (frame == 1) words[102] = frame;
        Draw__10VIUIWindow(window);
        void* const ui2 = pointer_of(_5VIWnd$mUI);
        text = GetString__13VIStringTableii(ui2, 955, static_cast<int*>(ui2)[12]);
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text, 20, 0, static_cast<int*>(window)[5] - 40, lines, 0, 8);
        void* const ui3 = pointer_of(_5VIWnd$mUI);
        const unsigned short* label = GetString__13VIStringTableii(ui3, 969, static_cast<int*>(ui3)[12]);
        void* const ui4 = pointer_of(_5VIWnd$mUI);
        DrawPrompt__10VIWndMcErriPCUsT2(self, lines + 1, label, GetString__13VIStringTableii(ui4, 896, static_cast<int*>(ui4)[12]));
        End2D__4VIUI(ui);
        return 0;
    }
    if (action == 3) {
        if (param1 == 136) {
            words[102] = 0;
        } else if (param1 == 137) {
            words[102] = 1;
        } else if (param1 == 134) {
            if (words[102] != 0) {
                goto_screen(words, 4);
            } else {
                exit_mcerr(words, 3);
            }
        }
    }
    return 0;
}
}
