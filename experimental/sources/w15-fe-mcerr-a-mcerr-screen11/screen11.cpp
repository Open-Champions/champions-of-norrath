// Observed: ProcessScreen11__10VIWndMcErriii at 0x0109cee0 is 652 bytes. With (this, action, param1, param2) it
// calls scePrintf("VIWndMcErr::ProcessScreen11(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1,
// param2) and then, with ui = the word _5VIWnd$mUI and window = this + 40:
//  - action 0 (draw): the same sequence as ProcessScreen10 but for string 963: text = GetString(ui, 963, ui word
//    +48); n = CountLines(window, text, 510); Init(window, ui, {0, 0}, this + 420, 550, n + 2, 0);
//    Open(window, 1); Begin2D(ui); Draw(window); TextBox(window, GetString(ui, 963, column), 20, 0, width - 40,
//    n, 0, 8); DrawPrompt__10VIWndMcErriPCUsT2(this, n + 1, GetString(ui, 786, column), 0); End2D(ui);
//  - action 3 with param1 = 134: s = VIMemCardStatus__FPi(0). For s of 0 or 3, VIIsMcSameSession__Fv() is
//    called: a result of 1 prints "VIWndMcErr::ExitMcErr(Rval = %d)\n" with 1 and then, with a parent word at
//    +428, sends message 4354 (argument 0) and message 4358 (argument 1) to it, or without a parent stores 1 at
//    +436; any other result goes to screen 16. For s of 2 or 4 it goes to screen 16; every other s goes to
//    screen 4. "Goes to screen n" is scePrintf("VIWndMcErr::GotoScreen %d\n", n) and the stores +412 = n,
//    +408 = 0, +416 = 0;
//  - every other action (and action 3 with another param1) does nothing.
// It returns 0. Each message goes through entry 6 (offset 48) of the table at the word parent + 32: a 16-bit
// adjustment, then the function at +4, called as (parent + adjustment, this, message, argument, 0).
// Inferred: the screen shown for a card state whose message is string 963, with the same prompt as screen 10;
// the same-session test decides between leaving and asking again (screen 16).
// Unknown: what the status values and the session test mean, the text of the strings, the field names, and the
// type of the result (zero is returned).
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
int VIIsMcSameSession__Fv();

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

int ProcessScreen11__10VIWndMcErriii(void* self, int action, int param1, int param2) {
    int* words = static_cast<int*>(self);
    unsigned char* bytes = static_cast<unsigned char*>(self);
    scePrintf("VIWndMcErr::ProcessScreen11(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1, param2);
    void* const window = bytes + 40;
    void* const ui = pointer_of(_5VIWnd$mUI);
    if (action == 0) {
        const unsigned short* text = GetString__13VIStringTableii(ui, 963, static_cast<int*>(ui)[12]);
        const int lines = CountLines__10VIUIWindowPCUsi(window, text, 510);
        int position[2] = {0, 0};
        Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, pointer_of(_5VIWnd$mUI), position, words + 105, 550, lines + 2, 0);
        Open__10VIUIWindowb(window, true);
        Begin2D__4VIUI(ui);
        Draw__10VIUIWindow(window);
        void* const ui2 = pointer_of(_5VIWnd$mUI);
        text = GetString__13VIStringTableii(ui2, 963, static_cast<int*>(ui2)[12]);
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text, 20, 0, static_cast<int*>(window)[5] - 40, lines, 0, 8);
        void* const ui3 = pointer_of(_5VIWnd$mUI);
        DrawPrompt__10VIWndMcErriPCUsT2(self, lines + 1, GetString__13VIStringTableii(ui3, 786, static_cast<int*>(ui3)[12]), 0);
        End2D__4VIUI(ui);
        return 0;
    }
    if (action == 3 && param1 == 134) {
        const int status = VIMemCardStatus__FPi(0);
        int target = 4;
        if (status == 0 || status == 3) {
            const int same = VIIsMcSameSession__Fv();
            if (same == 1) {
                scePrintf("VIWndMcErr::ExitMcErr(Rval = %d)\n", 1);
                if (words[107] != 0) {
                    send_to_parent(self, 4354, 0);
                    send_to_parent(self, 4358, 1);
                } else {
                    words[109] = same;
                }
                return 0;
            }
            target = 16;
        } else if (status == 2 || status == 4) {
            target = 16;
        }
        goto_screen(words, target);
    }
    return 0;
}
}
