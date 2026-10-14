// Observed: ProcessScreen50__10VIWndMcErriii at 0x0109e0c8 is 496 bytes. With (this, action, param1, param2) it
// calls scePrintf("VIWndMcErr::ProcessScreen50(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1,
// param2). With ui = the word _5VIWnd$mUI and window = this + 40:
//  - action 0 (draw): n = CountLines(window, GetString__13VIStringTableii(ui, 964, ui word +48), 310);
//    Init(window, ui, {0, 0}, this + 420, 350, n + 2, 0) (no Open call); Begin2D(ui); Draw(window);
//    TextBox(window, GetString(ui, 964, column), 20, 0, width - 40, n, 0, 8) with width the word at window +20;
//    when the word at +416 is zero the word at +408 is set to 1; +416 is incremented;
//    DrawPrompt__10VIWndMcErriPCUsT2(this, n + 1, GetString(ui, 786, column), 0); End2D(ui);
//  - action 3 with param1 = 134: scePrintf("VIWndMcErr::ExitMcErr(Rval = %d)\n", 0) and then, with a parent word
//    at +428, sends message 4354 and message 4358 (both with argument 0) to it, or without a parent stores 0 at
//    +436;
//  - every other action (and action 3 with another param1) does nothing.
// It returns 0. Each message goes through entry 6 (offset 48) of the table at the word parent + 32: a 16-bit
// adjustment, then the function at +4, called as (parent + adjustment, this, message, argument, 0).
// Inferred: the message screen shown for an unformatted card (ProcessScreen30 goes to screen 50 after printing
// "NOT FORMATTED"), whose confirm key leaves the error window with result 0.
// Unknown: what string 964 says, why +408 is set on the first frame, the field names, and the type of the result
// (zero is returned).
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
static inline void exit_mcerr(int* words, int rval) {
    scePrintf("VIWndMcErr::ExitMcErr(Rval = %d)\n", rval);
    if (words[107] != 0) {
        send_to_parent(words, 4354, 0);
        send_to_parent(words, 4358, rval);
    } else {
        words[109] = rval;
    }
}

int ProcessScreen50__10VIWndMcErriii(void* self, int action, int param1, int param2) {
    int* words = static_cast<int*>(self);
    unsigned char* bytes = static_cast<unsigned char*>(self);
    scePrintf("VIWndMcErr::ProcessScreen50(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1, param2);
    void* const ui = pointer_of(_5VIWnd$mUI);
    if (action == 0) {
        void* const window = bytes + 40;
        const unsigned short* text = GetString__13VIStringTableii(ui, 964, static_cast<int*>(ui)[12]);
        const int lines = CountLines__10VIUIWindowPCUsi(window, text, 310);
        int position[2] = {0, 0};
        Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, pointer_of(_5VIWnd$mUI), position, words + 105, 350, lines + 2, 0);
        Begin2D__4VIUI(ui);
        Draw__10VIUIWindow(window);
        void* const ui2 = pointer_of(_5VIWnd$mUI);
        text = GetString__13VIStringTableii(ui2, 964, static_cast<int*>(ui2)[12]);
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text, 20, 0, static_cast<int*>(window)[5] - 40, lines, 0, 8);
        const int frame = words[104];
        if (frame == 0) words[102] = 1;
        words[104] = frame + 1;
        void* const ui3 = pointer_of(_5VIWnd$mUI);
        DrawPrompt__10VIWndMcErriPCUsT2(self, lines + 1, GetString__13VIStringTableii(ui3, 786, static_cast<int*>(ui3)[12]), 0);
        End2D__4VIUI(ui);
    } else if (action == 3 && param1 == 134) {
        exit_mcerr(words, 0);
    }
    return 0;
}
}
