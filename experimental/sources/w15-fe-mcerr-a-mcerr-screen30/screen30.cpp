// Observed: ProcessScreen30__10VIWndMcErriii at 0x0109ded8 is 496 bytes. With (this, action, param1, param2) it
// calls scePrintf("VIWndMcErr::ProcessScreen30(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1,
// param2) and returns 0 at once unless action is 0. For action 0, with ui = the word _5VIWnd$mUI and
// window = this + 40:
//  1. n = CountLines(window, GetString__13VIStringTableii(ui, 951, ui word +48), 460);
//     Init(window, ui, {0, 0}, this + 420, 500, n, 0) (no Open call); Begin2D(ui); Draw(window);
//     TextBox(window, GetString(ui, 951, column), 20, 0, width - 40, 256, 0, 8) with width the word at
//     window +20 (the line count passed to TextBox is the constant 256, not n);
//  2. the word at +416 is incremented; when it reaches exactly 75, s = VIMemCardStatus__FPi(&card) is switched
//     on, where card is a local int the call fills in:
//       s = 0 or 3: VIMemCardIsPrepared__Fv() equal to 1 goes to screen 63; otherwise card below 3000 goes to
//                   screen 60 and any larger card (signed compare) to screen 61;
//       s = 1 or 5: goes to screen 20;
//       s = 2 or 4: printf("NOT FORMATTED\n"), then goes to screen 50;
//       any other s: nothing;
//     "goes to screen n" is scePrintf("VIWndMcErr::GotoScreen %d\n", n) and the stores +412 = n, +408 = 0,
//     +416 = 0;
//  3. End2D(ui); the function returns 0.
// Inferred: a waiting screen (string 951, the same text as screen 4) that polls the card after 75 frames; the
// local value looks like a free-space or size figure (card below 3000 versus at least 3000) that picks
// between the not-enough-space screen 60 and screen 61; 2 and 4 look like an unformatted card because of the
// printed text.
// Unknown: what the status values and the local value mean, what screens 20, 50, 60, 61 and 63 are, the
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
void Begin2D__4VIUI(void* ui);
void End2D__4VIUI(void* ui);
void Draw__10VIUIWindow(void* window);
void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int c, int d,
                                               int color, unsigned e);
int VIMemCardStatus__FPi(int* value);
int VIMemCardIsPrepared__Fv();
int printf(const char* format, ...);

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(NORRATH_TRANSLATE(word));
}
static inline void goto_screen(int* words, int screen) {
    scePrintf("VIWndMcErr::GotoScreen %d\n", screen);
    words[103] = screen;
    words[102] = 0;
    words[104] = 0;
}

int ProcessScreen30__10VIWndMcErriii(void* self, int action, int param1, int param2) {
    int* words = static_cast<int*>(self);
    unsigned char* bytes = static_cast<unsigned char*>(self);
    scePrintf("VIWndMcErr::ProcessScreen30(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1, param2);
    void* const ui = pointer_of(_5VIWnd$mUI);
    if (action != 0) return 0;
    void* const window = bytes + 40;
    const unsigned short* text = GetString__13VIStringTableii(ui, 951, static_cast<int*>(ui)[12]);
    const int lines = CountLines__10VIUIWindowPCUsi(window, text, 460);
    int position[2] = {0, 0};
    Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, pointer_of(_5VIWnd$mUI), position, words + 105, 500, lines, 0);
    Begin2D__4VIUI(ui);
    Draw__10VIUIWindow(window);
    void* const ui2 = pointer_of(_5VIWnd$mUI);
    text = GetString__13VIStringTableii(ui2, 951, static_cast<int*>(ui2)[12]);
    TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text, 20, 0, static_cast<int*>(window)[5] - 40, 256, 0, 8);
    const int frame = words[104] + 1;
    words[104] = frame;
    if (frame == 75) {
        int card;
        int target = 0;
        switch (VIMemCardStatus__FPi(&card)) {
        case 0:
        case 3:
            if (VIMemCardIsPrepared__Fv() == 1) {
                target = 63;
            } else {
                target = card < 3000 ? 60 : 61;
            }
            break;
        case 1:
        case 5:
            target = 20;
            break;
        case 2:
        case 4:
            printf("NOT FORMATTED\n");
            target = 50;
            break;
        default:
            break;
        }
        if (target != 0) goto_screen(words, target);
    }
    End2D__4VIUI(ui);
    return 0;
}
}
