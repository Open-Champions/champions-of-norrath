// Observed: ProcessScreen63__10VIWndMcErriii at 0x0109ee18 is 668 bytes. With (this, action, param1, param2) it
// calls scePrintf("VIWndMcErr::ProcessScreen63(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1,
// param2). With ui = the word _5VIWnd$mUI and column = the word at ui +48 it then builds a 16-bit text in a
// local 2048-byte area: snprintf16__8VIStringPUsiPCce(text, 2048, "%S\n\n%S", GetString(ui, 973, column),
// GetString(ui, 965, column)). Then, with window = this + 40:
//  - action 0 (draw): n = CountLines(window, text, 510); Init(window, ui, {0, 0}, this + 420, 550, n + 2, 0)
//    (no Open call); Begin2D(ui); the word at +416 is incremented and, when it becomes exactly 1: s =
//    VIMemCardStatus__FPi(&card) where card is a local int; a nonzero s goes to screen 64; a zero s computes
//    key = GetTime__7VITimer() + card as a 64-bit value, calls VISetMcSessionId__FUl(key), prints
//    printf("Writing session key (%ld) to memory card.\n", key) and calls VIMemCardWriteFile__FPCcPCvi(
//    "session.id", &key, 8), a negative result of which goes to screen 64 ("goes to screen n" is
//    scePrintf("VIWndMcErr::GotoScreen %d\n", n) and the stores +412 = n, +416 = 0, +408 = 0);
//    then Draw(window); TextBox(window, text, 20, 0, width - 40, n, 0, 8) with width the word at window +20;
//    DrawPrompt__10VIWndMcErriPCUsT2(this, n + 1, GetString(ui, 786, column), 0); End2D(ui);
//  - action 3 with +416 of 2 or more and param1 = 134: scePrintf("VIWndMcErr::ExitMcErr(Rval = %d)\n", 1) and
//    then, with a parent word at +428, sends message 4354 (argument 0) and message 4358 (argument 1) to it, or
//    without a parent stores 1 at +436;
//  - every other action (and action 3 before frame 2 or with another param1) does nothing.
// It returns 0. Each message goes through entry 6 (offset 48) of the table at the word parent + 32: a 16-bit
// adjustment, then the function at +4, called as (parent + adjustment, this, message, argument, 0).
// Inferred: a "card ready" screen that, on its first frame, records a fresh session key (time plus the card
// value) both in memory and in the file "session.id" on the card; the confirm key leaves the error window with
// result 1 once the screen has been up for two frames. The key's byte order in the file is whatever the 64-bit
// store gives (little endian here).
// Unknown: what strings 973 and 965 say, what the card value is, what the session id is used for, the exact
// width of the unsigned long parameter of VISetMcSessionId (32 bits as built), the field names, and the type of
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
int snprintf16__8VIStringPUsiPCce(unsigned short* buffer, int size, const char* format, ...);
int printf(const char* format, ...);
long long GetTime__7VITimer();
void VISetMcSessionId__FUl(unsigned long id);
int VIMemCardWriteFile__FPCcPCvi(const char* name, const void* data, int size);

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

int ProcessScreen63__10VIWndMcErriii(void* self, int action, int param1, int param2) {
    int* words = static_cast<int*>(self);
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned short text[1024];
    scePrintf("VIWndMcErr::ProcessScreen63(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1, param2);
    void* const ui = pointer_of(_5VIWnd$mUI);
    const unsigned short* first = GetString__13VIStringTableii(ui, 973, static_cast<int*>(ui)[12]);
    const unsigned short* second = GetString__13VIStringTableii(ui, 965, static_cast<int*>(ui)[12]);
    snprintf16__8VIStringPUsiPCce(text, 2048, "%S\n\n%S", first, second);
    if (action == 0) {
        void* const window = bytes + 40;
        const int lines = CountLines__10VIUIWindowPCUsi(window, text, 510);
        int position[2] = {0, 0};
        Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, pointer_of(_5VIWnd$mUI), position, words + 105, 550, lines + 2, 0);
        Begin2D__4VIUI(ui);
        const int frame = words[104] + 1;
        words[104] = frame;
        if (frame == 1) {
            int card;
            bool failed = VIMemCardStatus__FPi(&card) != 0;
            if (!failed) {
                long long key = GetTime__7VITimer() + card;
                VISetMcSessionId__FUl(static_cast<unsigned long>(key));
                printf("Writing session key (%ld) to memory card.\n", key);
                failed = VIMemCardWriteFile__FPCcPCvi("session.id", &key, 8) < 0;
            }
            if (failed) goto_screen(words, 64);
        }
        Draw__10VIUIWindow(window);
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text, 20, 0, static_cast<int*>(window)[5] - 40, lines, 0, 8);
        void* const ui2 = pointer_of(_5VIWnd$mUI);
        DrawPrompt__10VIWndMcErriPCUsT2(self, lines + 1, GetString__13VIStringTableii(ui2, 786, static_cast<int*>(ui2)[12]), 0);
        End2D__4VIUI(ui);
    } else if (action == 3 && words[104] >= 2 && param1 == 134) {
        exit_mcerr(words, 1);
    }
    return 0;
}
}
