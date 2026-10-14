// Observed: OnDrawSelf__12VIWndPatcher at 0x010cbbd0 is 1920 bytes. ui = the word _5VIWnd$mUI (0x01192c70);
// text(id) = GetString__13VIStringTableii(ui, id, word at ui+48); w is the embedded UI window at this+776 and
// w2 a second one at this+408; mode is the signed word at this+1148, sel the word at this+1144, msg the read-message
// window at this+2312. The routine ends at once, without End2D, when mode is -1. Otherwise, in order:
//  1. Begin2D__4VIUI(ui); DrawFakeSplashInterface__12VIWndPatcher(this);
//     Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(w, ui, {0, 0}, {320, 246}, 430, 8, 0).
//  2. A heading text t (initially none) from mode: 0 -> text(757); 1 -> text(754); 2 and 7 -> text(768); 3 -> text(812);
//     5 -> text(792); 6 -> text(814); 4 -> t = text(764), then Init(w, ui, {0, 0}, {320, 246}, 430, 9, 0) and
//     Open__10VIUIWindowb(w, true); any other mode leaves t unset. Then Draw__10VIUIWindow(w).
//  3. When mode is 4: when the first unit of Message__C16VIWndReadMessage(msg) is nonzero, Text__10VIUIWindowPCUsii9VIUICOLORUi(
//     w, text(813), 40, 5, sel == 0, 0); otherwise build "|-" + text(813) in a 1024-byte buffer, Text(w, buffer, 40, 5,
//     0, 0) and store 1 in sel. Then Text(w, text(this[1156] ? 793 : 790), 40, 6, sel == 1, 0); build "|x " + text(212)
//     + ("   |-|t " when the word at this+1156 is nonzero, else "   |t ") + text(750), Text(w, buffer, 40, 8, 0, 0), and
//     TickR__10VIUIWindowiib(w, 36, sel + 5, true). When mode is not 4:
//     TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(w, text(763), 40, 5, w[20] - 80, 2, 0, 8).
//  4. The dot animation: dot_delay is decremented as a byte; when the signed byte is zero or below it becomes 15 and
//     dot advances by one, wrapping to 0 when it reaches 3.
//  5. When t is set: strcpy16__8VIStringPUsUiPCUs(buffer, 1024, t); for modes 3, 5 and 6 append "." once for each of
//     (dot + 1) steps (strcat16__8VIStringPUsUiPCc); then, whether or not t was set, snprintf16__8VIStringPUsiPCce(buffer, 1024,
//     "%S %S", text(803), t) when t is set, else snprintf16(buffer, 1024, "%S", text(803)). The animation dots are then
//     overwritten by that formatted text (they never reach the screen).
//     TextBox(w, buffer, 40, 1, w[20] - 80, 2, 0, 0); DrawProgress__10VIUIWindowiiif(w, 40, 3, w[20] - 80,
//     (float word at this+1152)).
//  6. For mode 2: Draw(w2); TextBox(w2, text(769), 40, 1, w2[20] - 80, 3, 0, 8); Text(w2, "|x " + text(793), 40, 5, 0, 0).
//     For mode 7 the same with text(787). When the word at this+1164 is nonzero: zero it and this+1160,
//     SoundPlay__4VIUI11VIUISoundId(ui, 15), Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(this+1176, text(785),
//     text(770), 0, &{pair at this+788}, 500), store 1 at (this+1176)+28 and SetFocus__5VIWndP5VIWndb(this, this+1176, true).
//  7. End2D__4VIUI(ui).
// Inferred: a patcher screen with progress bar, a two-choice prompt in mode 4 (sel is the highlighted choice), a status
// line with an animated dot suffix, and two overlay windows for modes 2 and 7; word+28 of a child window is a shown flag.
// Unknown: the meaning of the modes, the words at this+1156/+1160/+1164, the type of the colour argument, why the dots
// are built and then overwritten, and the exact string ids' meanings.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<__UINTPTR_TYPE__>(norrath_host_pointer(word))
#else
#define NORRATH_WORD_ADDRESS(word) static_cast<__UINTPTR_TYPE__>(word)
#endif
extern "C" {
extern unsigned _5VIWnd$mUI;
extern signed char norrath_patcher_dot __asm__("dot.2110");
extern unsigned char norrath_patcher_dot_delay __asm__("dot_delay.2111");

struct Pair {
    int a, b;
};
void Begin2D__4VIUI(unsigned ui);
void End2D__4VIUI(unsigned ui);
void DrawFakeSplashInterface__12VIWndPatcher(void* self);
int Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, void* ui, const Pair* position, const Pair* size, int a,
                                              int b, int c);
const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg);
void Open__10VIUIWindowb(void* window, bool flag);
void Draw__10VIUIWindow(void* window);
const unsigned short* Message__C16VIWndReadMessage(const void* window);
void Text__10VIUIWindowPCUsii9VIUICOLORUi(void* window, const unsigned short* text, int x, int y, int color,
                                          unsigned flags);
void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int c, int d,
                                               int color, unsigned flags);
void str8to16__8VIStringPUsUiPCc(unsigned short* destination, unsigned size, const char* source);
void strcpy16__8VIStringPUsUiPCUs(unsigned short* destination, unsigned size, const unsigned short* source);
void strcat16__8VIStringPUsUiPCUs(unsigned short* destination, unsigned size, const unsigned short* source);
void strcat16__8VIStringPUsUiPCc(unsigned short* destination, unsigned size, const char* source);
int snprintf16__8VIStringPUsiPCce(unsigned short* destination, int size, const char* format, ...);
void TickR__10VIUIWindowiib(void* window, int a, int b, bool flag);
void DrawProgress__10VIUIWindowiiif(void* window, int a, int b, int c, float fraction);
void SoundPlay__4VIUI11VIUISoundId(unsigned ui, int sound);
void Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(void* window, const unsigned short* first,
                                                              const unsigned short* second, int prompt,
                                                              const Pair* rect, int size);
void SetFocus__5VIWndP5VIWndb(void* self, void* window, bool force);

static inline int& word(void* base, int offset) {
    return *reinterpret_cast<int*>(static_cast<unsigned char*>(base) + offset);
}
static inline void* pointer_of(unsigned value) {
    return reinterpret_cast<void*>(NORRATH_WORD_ADDRESS(value));
}
static inline const unsigned short* text(int id) {
    void* ui = pointer_of(_5VIWnd$mUI);
    return GetString__13VIStringTableii(ui, id, word(ui, 48));
}

void OnDrawSelf__12VIWndPatcher(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    void* window = bytes + 776;
    const unsigned ui = _5VIWnd$mUI;
    const unsigned short* heading = 0;
    Begin2D__4VIUI(ui);
    DrawFakeSplashInterface__12VIWndPatcher(self);
    const Pair origin = {0, 0};
    const Pair size = {320, 246};
    Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, pointer_of(_5VIWnd$mUI), &origin, &size, 430, 8, 0);
    const int mode = word(self, 1148);
    switch (mode) {
    case -1:
        return;
    case 0: heading = text(757); break;
    case 1: heading = text(754); break;
    case 2: heading = text(768); break;
    case 3: heading = text(812); break;
    case 4:
        heading = text(764);
        Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, pointer_of(_5VIWnd$mUI), &origin, &size, 430, 9, 0);
        Open__10VIUIWindowb(window, true);
        break;
    case 5: heading = text(792); break;
    case 6: heading = text(814); break;
    case 7: heading = text(768); break;
    default: break;
    }
    Draw__10VIUIWindow(window);
    unsigned short buffer[512];
    if (word(self, 1148) == 4) {
        if (*Message__C16VIWndReadMessage(bytes + 2312) != 0) {
            Text__10VIUIWindowPCUsii9VIUICOLORUi(window, text(813), 40, 5, word(self, 1144) == 0, 0);
        } else {
            str8to16__8VIStringPUsUiPCc(buffer, 1024, "|-");
            strcat16__8VIStringPUsUiPCUs(buffer, 1024, text(813));
            Text__10VIUIWindowPCUsii9VIUICOLORUi(window, buffer, 40, 5, 0, 0);
            word(self, 1144) = 1;
        }
        const unsigned short* line = word(self, 1156) != 0 ? text(793) : text(790);
        Text__10VIUIWindowPCUsii9VIUICOLORUi(window, line, 40, 6, word(self, 1144) == 1, 0);
        str8to16__8VIStringPUsUiPCc(buffer, 1024, "|x ");
        strcat16__8VIStringPUsUiPCUs(buffer, 1024, text(212));
        if (word(self, 1156) != 0) strcat16__8VIStringPUsUiPCc(buffer, 1024, "   |-|t ");
        else strcat16__8VIStringPUsUiPCc(buffer, 1024, "   |t ");
        strcat16__8VIStringPUsUiPCUs(buffer, 1024, text(750));
        Text__10VIUIWindowPCUsii9VIUICOLORUi(window, buffer, 40, 8, 0, 0);
        TickR__10VIUIWindowiib(window, 36, word(self, 1144) + 5, true);
    } else {
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text(763), 40, 5, word(window, 20) - 80, 2, 0, 8);
    }
    norrath_patcher_dot_delay = static_cast<unsigned char>(norrath_patcher_dot_delay - 1);
    if (static_cast<signed char>(norrath_patcher_dot_delay) <= 0) {
        norrath_patcher_dot_delay = 15;
        norrath_patcher_dot = static_cast<signed char>(norrath_patcher_dot + 1);
        if (norrath_patcher_dot == 3) norrath_patcher_dot = 0;
    }
    if (heading != 0) {
        strcpy16__8VIStringPUsUiPCUs(buffer, 1024, heading);
        const int now = word(self, 1148);
        if (now == 3 || now == 5 || now == 6) {
            for (signed char i = 0; i < norrath_patcher_dot + 1; i = static_cast<signed char>(i + 1))
                strcat16__8VIStringPUsUiPCc(buffer, 1024, ".");
        }
        snprintf16__8VIStringPUsiPCce(buffer, 1024, "%S %S", text(803), heading);
    } else {
        snprintf16__8VIStringPUsiPCce(buffer, 1024, "%S", text(803));
    }
    TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, buffer, 40, 1, word(window, 20) - 80, 2, 0, 0);
    DrawProgress__10VIUIWindowiiif(window, 40, 3, word(window, 20) - 80, *reinterpret_cast<float*>(bytes + 1152));
    if (word(self, 1148) == 2) {
        void* second = bytes + 408;
        Draw__10VIUIWindow(second);
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(second, text(769), 40, 1, word(second, 20) - 80, 3, 0, 8);
        str8to16__8VIStringPUsUiPCc(buffer, 1024, "|x ");
        strcat16__8VIStringPUsUiPCUs(buffer, 1024, text(793));
        Text__10VIUIWindowPCUsii9VIUICOLORUi(second, buffer, 40, 5, 0, 0);
    }
    if (word(self, 1148) == 7) {
        void* second = bytes + 408;
        Draw__10VIUIWindow(second);
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(second, text(787), 40, 1, word(second, 20) - 80, 3, 0, 8);
        str8to16__8VIStringPUsUiPCc(buffer, 1024, "|x ");
        strcat16__8VIStringPUsUiPCUs(buffer, 1024, text(793));
        Text__10VIUIWindowPCUsii9VIUICOLORUi(second, buffer, 40, 5, 0, 0);
    }
    if (word(self, 1164) != 0) {
        word(self, 1164) = 0;
        word(self, 1160) = 0;
        void* message = bytes + 1176;
        SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 15);
        const unsigned short* first = text(785);
        const unsigned short* second = text(770);
        const Pair rect = *reinterpret_cast<const Pair*>(bytes + 788);
        Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(message, first, second, 0, &rect, 500);
        word(message, 28) = 1;
        SetFocus__5VIWndP5VIWndb(self, message, true);
    }
    End2D__4VIUI(ui);
}
}
