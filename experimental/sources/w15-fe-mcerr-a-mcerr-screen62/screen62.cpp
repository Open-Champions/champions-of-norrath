// Observed: ProcessScreen62__10VIWndMcErriii at 0x0109eab8 is 860 bytes. With (this, action, param1, param2) it
// calls scePrintf("VIWndMcErr::ProcessScreen62(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1,
// param2) and returns 0 at once unless action is 0. For action 0, with ui = the word _5VIWnd$mUI, column = the
// word at ui +48 and window = this + 40:
//  1. n = CountLines(window, GetString__13VIStringTableii(ui, 968, column), 510);
//     Init(window, ui, {0, 0}, this + 420, 550, n + 1, 0) (no Open call); Begin2D(ui); Draw(window); the word
//     at +416 (call it f) is incremented;
//  2. when f is exactly 5: a nonzero VIMemCardStatus__FPi(0) goes to screen 64; otherwise VIMemCardStatus is
//     called again with a pointer to a local int and a local value below 3000 (signed) goes to screen 60;
//     otherwise VIMemCardPrepare__Fi(-1) and then VIMemCardPadAsync__Fi(2996) are called and a negative result
//     of either goes to screen 64 (the second call is not made after a negative first);
//  3. when no screen was chosen and the word at +416 is 6 or more, VIMemCardAsyncIdle__Fv() is called: 1 means
//     the card work is done and -1 means it failed (screen 64); 0 and every other value change nothing. On 1:
//     printf("FINISHED: %d\n", the word at +416); VIMemCardWriteFile__FPCcPCvi(VIMemCardProductId__Fv(),
//     "12345678", 8) goes to screen 64 when negative and 63 otherwise; then Save__13VIGameOptions(
//     VIGetGameOptions__Fv()) goes to screen 64 when negative and 63 otherwise. Both outcomes print and store as
//     below, so a successful write followed by a failed save ends on screen 64;
//     "goes to screen n" is scePrintf("VIWndMcErr::GotoScreen %d\n", n) and the stores +412 = n, +416 = 0,
//     +408 = 0;
//  4. TextBox(window, GetString(ui, 968, column), 20, 0, width - 40, n, 0, 8) with width the word at window +20;
//     DrawProgress__10VIUIWindowiiif(window, 40, n, width - 80, p) where p is 0 while the word at +416 (re-read
//     here, so zero after a screen change) is below 6, and otherwise clamp(float(f) * 0.0015625, 0, 1) * 0.9 + 0.1
//     with the three constants taken bit for bit from the image (their float bit patterns are 0x3acccccc,
//     0x3f666666 and 0x3dcccccc); End2D(ui). The function returns 0.
// Inferred: the save-progress screen: it checks the card after 5 frames (screens 60 and 64 are the low-space and
// failure screens), starts asynchronous card work, and once the work reports done (after frame 6) writes a small
// marker file and saves the game options; the bar fills in 640 frames. The eight bytes "12345678" look like a
// placeholder payload.
// Unknown: what the status values and the async-idle values other than 1 and -1 mean, what screens 63 and 64 are,
// why the written payload is that text, the field names, and the type of the result (zero is returned).
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
int printf(const char* format, ...);
int VIMemCardPrepare__Fi(int value);
int VIMemCardPadAsync__Fi(int value);
int VIMemCardAsyncIdle__Fv();
const char* VIMemCardProductId__Fv();
int VIMemCardWriteFile__FPCcPCvi(const char* name, const void* data, int size);
void* VIGetGameOptions__Fv();
int Save__13VIGameOptions(void* options);
void DrawProgress__10VIUIWindowiiif(void* window, int a, int b, int c, float fraction);

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(NORRATH_TRANSLATE(word));
}
static inline void goto_screen(int* words, int screen) {
    scePrintf("VIWndMcErr::GotoScreen %d\n", screen);
    words[103] = screen;
    words[102] = 0;
    words[104] = 0;
}

int ProcessScreen62__10VIWndMcErriii(void* self, int action, int param1, int param2) {
    int* words = static_cast<int*>(self);
    unsigned char* bytes = static_cast<unsigned char*>(self);
    scePrintf("VIWndMcErr::ProcessScreen62(Action = %d, Param1 = %d, Param2 = %d)\n", action, param1, param2);
    void* const ui = pointer_of(_5VIWnd$mUI);
    if (action != 0) return 0;
    void* const window = bytes + 40;
    const unsigned short* text = GetString__13VIStringTableii(ui, 968, static_cast<int*>(ui)[12]);
    const int lines = CountLines__10VIUIWindowPCUsi(window, text, 510);
    int position[2] = {0, 0};
    Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, pointer_of(_5VIWnd$mUI), position, words + 105, 550, lines + 1, 0);
    Begin2D__4VIUI(ui);
    Draw__10VIUIWindow(window);
    const int frame = words[104] + 1;
    words[104] = frame;
    int target = 0;
    if (frame == 5) {
        if (VIMemCardStatus__FPi(0) != 0) {
            target = 64;
        } else {
            int card;
            VIMemCardStatus__FPi(&card);
            if (card < 3000) {
                target = 60;
            } else if (VIMemCardPrepare__Fi(-1) < 0) {
                target = 64;
            } else if (VIMemCardPadAsync__Fi(2996) < 0) {
                target = 64;
            }
        }
    }
    if (target != 0) {
        goto_screen(words, target);
    } else if (words[104] >= 6) {
        const int idle = VIMemCardAsyncIdle__Fv();
        if (idle == 1) {
            printf("FINISHED: %d\n", words[104]);
            goto_screen(words, VIMemCardWriteFile__FPCcPCvi(VIMemCardProductId__Fv(), "12345678", 8) < 0 ? 64 : 63);
            goto_screen(words, Save__13VIGameOptions(VIGetGameOptions__Fv()) < 0 ? 64 : 63);
        } else if (idle == -1) {
            goto_screen(words, 64);
        }
    }
    void* const ui2 = pointer_of(_5VIWnd$mUI);
    text = GetString__13VIStringTableii(ui2, 968, static_cast<int*>(ui2)[12]);
    TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text, 20, 0, static_cast<int*>(window)[5] - 40, lines, 0, 8);
    const int count = words[104];
    float fraction = static_cast<float>(count) * __builtin_bit_cast(float, 0x3acccccc);
    fraction = fraction > 0.0f ? fraction : 0.0f;
    fraction = fraction < 1.0f ? fraction : 1.0f;
    float shown = 0.0f;
    if (count >= 6) shown = fraction * __builtin_bit_cast(float, 0x3f666666) + __builtin_bit_cast(float, 0x3dcccccc);
    DrawProgress__10VIUIWindowiiif(window, 40, lines, static_cast<int*>(window)[5] - 80, shown);
    End2D__4VIUI(ui);
    return 0;
}
}
