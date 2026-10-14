// Observed: ProcessScreen04__10VIWndMcErriii at 0x0109ca88 is 648 bytes. With (this, action, param1, param2)
// it calls scePrintf("VIWndMcErr::ProcessScreen04(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1,
// param2) and returns 0 at once unless action is 0. For action 0 (with ui = the word _5VIWnd$mUI):
//  1. text = GetString__13VIStringTableii(ui, 951, ui word +48); n = CountLines__10VIUIWindowPCUsi(this + 40,
//     text, 460);
//  2. Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(this + 40, ui, {0, 0}, this + 420, 500, n, 0) then
//     Open__10VIUIWindowb(this + 40, 1);
//  3. Begin2D(ui); Draw__10VIUIWindow(this + 40); TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(this + 40, text2,
//     20, 0, width - 40, n, 0, 8) with text2 = GetString(ui re-read, 951, column) and width the word at
//     window +20;
//  4. the word at +416 is incremented; when it reaches exactly 40, VIMemCardStatus__FPi(0) is switched on:
//     values 0 and 3 print "VIWndMcErr::ExitMcErr(Rval = %d)\n" with 1, then, with a parent word at +428, send
//     message 4354 (argument 0) and message 4358 (argument 1) to it, or without a parent store 1 at +436;
//     values 1, 2, 4 and 5 go to another screen with scePrintf("VIWndMcErr::GotoScreen %d\n", n) and the
//     stores +412 = n, +408 = 0, +416 = 0, where n is, when the word at +432 is 1 or otherwise: 12 or 10 for
//     values 1 and 5, 17 or 11 for values 2 and 4; other values do nothing;
//  5. End2D(ui); the function returns 0.
// Each message goes through entry 6 (offset 48) of the table whose address is the word at parent + 32: a
// 16-bit adjustment to the object pointer, then the function at +4, called as (parent + adjustment, this,
// message, argument, 0).
// Inferred: this is the initial memory-card check screen: it shows a text (string 951) and after 40 frames
// asks the card layer for its status and leaves or moves on; +416 is a frame counter, +412 the screen
// number, +428 the parent window and +432 the mode that McErrSetMode stores.
// Unknown: what the status values mean, what screens 10 to 17 are, the field names, and the type of
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
void Open__10VIUIWindowb(void* window, bool open);
void Begin2D__4VIUI(void* ui);
void End2D__4VIUI(void* ui);
void Draw__10VIUIWindow(void* window);
void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int c, int d,
                                               int color, unsigned e);
int VIMemCardStatus__FPi(int* value);

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

int ProcessScreen04__10VIWndMcErriii(void* self, int action, int param1, int param2) {
    int* words = static_cast<int*>(self);
    unsigned char* bytes = static_cast<unsigned char*>(self);
    scePrintf("VIWndMcErr::ProcessScreen04(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1, param2);
    void* const ui = pointer_of(_5VIWnd$mUI);
    if (action != 0) return 0;
    void* const window = bytes + 40;
    const unsigned short* text = GetString__13VIStringTableii(ui, 951, static_cast<int*>(ui)[12]);
    const int lines = CountLines__10VIUIWindowPCUsi(window, text, 460);
    int position[2] = {0, 0};
    Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, pointer_of(_5VIWnd$mUI), position, words + 105, 500, lines, 0);
    Open__10VIUIWindowb(window, true);
    Begin2D__4VIUI(ui);
    Draw__10VIUIWindow(window);
    void* const ui2 = pointer_of(_5VIWnd$mUI);
    text = GetString__13VIStringTableii(ui2, 951, static_cast<int*>(ui2)[12]);
    TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text, 20, 0, static_cast<int*>(window)[5] - 40, lines, 0, 8);
    words[104] = words[104] + 1;
    if (words[104] == 40) {
        int target = 0;
        switch (VIMemCardStatus__FPi(0)) {
        case 0:
        case 3:
            scePrintf("VIWndMcErr::ExitMcErr(Rval = %d)\n", 1);
            if (words[107] != 0) {
                send_to_parent(self, 4354, 0);
                send_to_parent(self, 4358, 1);
            } else {
                words[109] = 1;
            }
            break;
        case 1:
            target = words[108] == 1 ? 12 : 10;
            break;
        case 2:
        case 4:
            target = words[108] == 1 ? 17 : 11;
            break;
        case 5:
            target = words[108] == 1 ? 12 : 10;
            break;
        default:
            break;
        }
        if (target != 0) {
            scePrintf("VIWndMcErr::GotoScreen %d\n", target);
            words[103] = target;
            words[102] = 0;
            words[104] = 0;
        }
    }
    End2D__4VIUI(ui);
    return 0;
}
}
