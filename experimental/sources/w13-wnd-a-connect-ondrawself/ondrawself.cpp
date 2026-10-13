// Observed: OnDrawSelf__12VIWndConnect at 0x010a1318 is 2828 bytes (ui = the word _5VIWnd$mUI at 0x01192c70, read
// once at the start for Begin2D/End2D; the table words use fresh reads; w1 = this + 1176, w2 = this + 1544,
// idx = the word at +1916, entry = (word at +1972) + 620 * idx, with the first word of an entry an integer, a
// text at entry + 4 and a text at entry + 260; the 64-bit values at +1976 and +1984 are times).
//  1. VIMemCardStatus__FPi(0): when it returns 1 the virtual entry at byte offset 40 of the table at this + 32
//     (halfword adjustment, then function address) is called with (this + adjustment, 135) and the function
//     returns without drawing; otherwise the word at +1964 is cleared.
//  2. When the focus word _5VIWnd$mFocus (0x01192c74) differs from this, or the word at +1920 is nonzero,
//     ResetTickPulse__10VIUIWindow(w1). A nonzero child word at +1912 is removed with RemoveChild and cleared.
//     When +1920 is nonzero and +1924 is zero and the 64-bit value at +1984 is below GetTime__7VITimer():
//     ezNetCtlDownInterface(0), SoundPlay__4VIUI11VIUISoundId(ui, 8), and +1984 = GetTime() + 100000.
//  3. Begin2D(ui); Draw(w1); DrawFrame__10VIUIWindowiiiiUi(w1, 40, 1, width - 80, 2, 32) with width the word at
//     w1 + 20; the list line: sprintf(text, "%d", first word of entry); TextBox(w1, T__10VIUIWindowPCc(w1, text),
//     40, 1, 60, 1, 0, 0); TextBox(w1, T(w1, entry + 4), 60, 1, width - 100, 1, 0, 2); TextBox(w1, T(w1,
//     entry + 260), 60, 2, width - 100, 1, 0, 2); the footer: a 1024-unit buffer built with
//     str8to16("|x ") + GetString(ui, 756) + strcat16("   |c ") + GetString(ui, 750), drawn with
//     Text__10VIUIWindowPCUsii9VIUICOLORUi(w1, buffer, 40, 4, 0, 0).
//  4. The file-local counters of this function: dot_delay is decremented; when it is not above 0 it becomes 15
//     and dot (a signed byte) advances, wrapping from 2 to 0. Both are static to this function with the image
//     start values dot_delay = 15, dot = 0; final_delay (a word) starts at 60.
//  5. When the word at +1920 is nonzero, ezNetCtlGetStatus(sStatus) and then:
//     - with +1924 nonzero: unet_startup_async_status__Fv() result r. If r is 0 and final_delay is 0:
//       unet_shutdown__Fv() and SetFocus__5VIWndP5VIWndb(this, 0, 1); the rest of this status part is skipped.
//       Otherwise final_delay is decremented when nonzero. Then w2 gets SetTitle(w2, T(w2, "")), the buffer is
//       GetString(ui, 776) copied with strcpy16, +1964 = 1, the text width of the buffer
//       (GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs with the font at ui + 56, and the words at ui + 72
//       and ui + 80) gives max(width, 200) + 120 for Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(w2, ui, &{0, 0},
//       &{words at +1188, +1192}, that, 3, 0), "." is appended dot + 1 times, then Draw(w2) and TextBox(w2,
//       buffer, 40, 1, width2, 1, 0, 0) with width2 the word at w2 + 20 (no 80 subtracted).
//     - with +1924 zero: SetTitle__10VIUIWindowPCc(w2, entry + 260); Init(w2, ui, &{0, 0}, &{+1188, +1192},
//       max(text width of entry + 260, 300) + 120, 4, 0); Draw(w2); state = the text at sStatus + 16 copied
//       with strcpy. If state equals "Connected" or starts with "CONNECT " nothing more happens here. If it
//       equals "Detached" (strcasecmp): the entry-135 virtual call again, then a prompt text: str8to16("\"")
//       + entry + 260 + "\"\n", followed by GetString(ui, 1026) with snprintf16 when entry + 260 contains
//       "SCE/Ethernet (Network Adaptor)" or "SCE/Modem (Network Adaptor)", else GetString(ui, 780) appended;
//       Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(this + 40, GetString(ui, 767), buffer, 0,
//       &{+1188, +1192}, max(width, 300) + 80), the word at this + 68 = 1, SetFocus(this, this + 40, 1).
//       Any other state gets "." appended dot + 1 times (strcat). Then the status line: GetString(ui, 803)
//       copied, " " and state appended, TextBox(w2, buffer, 40, 1, width2 - 80, 1, 0, 0); str8to16("|c ") +
//       GetString(ui, 753), TextBox(w2, buffer, 40, 3, width2 - 80, 1, 0, 0).
//       Then by the word at sStatus + 12: 3 -> final_delay is decremented and, if it reaches 0, becomes 30,
//       +1924 = 1 and unet_startup_async__Fv() is called (jump to 6); 2 -> final_delay = 60,
//       printf("retrying... (reset timeout)\n") and +1984 = GetTime() + the 64-bit value at +1976; 5 ->
//       final_delay = 60, Setup(this + 40, GetString(ui, 771), GetString(ui, 807), 0, &{+1188, +1192}, 400),
//       the word at this + 68 = 1, SetFocus(this, this + 40, 1), +1920 = 0, +1924 = 0; any other status ->
//       final_delay = 60.
//  6. When the word at +1956 is nonzero it is decremented; if it reaches 0: SoundPlay(ui, 15), +1920 = 1,
//     BeginConnect__12VIWndConnect(this), Close(w2, 1), Init(w2, ui, &{0, 0}, &{+1188, +1192},
//     max(text width of entry + 260, 300) + 120, 4, 0), Open(w2, 0) (the key-134 path of OnKeyDown).
//  7. End2D(ui).
// Inferred: this draws the connection window: a list line with the selected entry, the connection progress with
// animated dots, and the retry/failure prompts; the file-local names come from the image symbols.
// Unknown: the meaning of the status values 2, 3 and 5 (observed numbers only), the TextBox/Text/DrawFrame
// integer arguments (register order only), the layout of sStatus beyond the word at +12 and the text at +16
// (assumed 272 bytes from the gap to the next image symbol), the type of the message-window Setup arguments,
// and the return type (void).
extern "C" {
int VIMemCardStatus__FPi(int* value);
unsigned long long GetTime__7VITimer();
void ResetTickPulse__10VIUIWindow(void* window);
void RemoveChild__5VIWndP5VIWnd(void* self, void* child);
void SetFocus__5VIWndP5VIWndb(void* self, void* target, bool flag);
int ezNetCtlDownInterface(int value);
int ezNetCtlGetStatus(void* status);
int unet_startup_async_status__Fv();
void unet_shutdown__Fv();
void unet_startup_async__Fv();
void SoundPlay__4VIUI11VIUISoundId(const void* ui, int sound);
void Begin2D__4VIUI(void* ui);
void End2D__4VIUI(void* ui);
void Draw__10VIUIWindow(void* window);
void DrawFrame__10VIUIWindowiiiiUi(void* window, int a, int b, int c, int d, unsigned e);
void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int c, int d,
                                               int color, unsigned e);
void Text__10VIUIWindowPCUsii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int color, unsigned c);
const unsigned short* T__10VIUIWindowPCc(void* window, const char* key);
void SetTitle__10VIUIWindowPCUs(void* window, const unsigned short* text);
void SetTitle__10VIUIWindowPCc(void* window, const char* text);
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, unsigned ui, const int* position, const int* size,
                                              int a, int b, int c);
void Open__10VIUIWindowb(void* window, bool flag);
void Close__10VIUIWindowb(void* window, bool flag);
void Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(void* window, const unsigned short* first,
                                                              const unsigned short* second, int prompt,
                                                              const int* size, int a);
void BeginConnect__12VIWndConnect(void* self);
int GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs(void* font, void* raster, void* dictionary,
                                                         const unsigned short* text);
int GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc(void* font, void* raster, void* dictionary, const char* text);
const unsigned short* GetString__13VIStringTableii(const void* table, int id, int column);
void str8to16__8VIStringPUsUiPCc(unsigned short* dest, unsigned size, const char* source);
void strcpy16__8VIStringPUsUiPCUs(unsigned short* dest, unsigned size, const unsigned short* source);
void strcat16__8VIStringPUsUiPCUs(unsigned short* dest, unsigned size, const unsigned short* source);
void strcat16__8VIStringPUsUiPCc(unsigned short* dest, unsigned size, const char* source);
void snprintf16__8VIStringPUsiPCUse(unsigned short* dest, int size, const unsigned short* format, ...);
int sprintf(char* dest, const char* format, ...);
int printf(const char* format, ...);
char* strcpy(char* dest, const char* source);
char* strcat(char* dest, const char* source);
int strcmp(const char* a, const char* b);
int strncmp(const char* a, const char* b, unsigned long count);
int strcasecmp(const char* a, const char* b);
char* strstr(const char* haystack, const char* needle);
extern unsigned _5VIWnd$mUI;
extern unsigned _5VIWnd$mFocus;

struct VtEntry {
    short delta;
    short index;
    void (*function)(void*, int);
};

static int final_delay = 60;
static signed char dot = 0;
static signed char dot_delay = 15;
static unsigned char sStatus[272];

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<unsigned long>(word));
}
static inline unsigned word_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
static inline const unsigned short* string_for(int id) {
    const void* table = pointer_of(_5VIWnd$mUI);
    return GetString__13VIStringTableii(table, id, static_cast<const int*>(table)[12]);
}
static inline unsigned char* ui_bytes() {
    return static_cast<unsigned char*>(pointer_of(_5VIWnd$mUI));
}
static inline int text_width(const char* text) {
    unsigned char* ui = ui_bytes();
    return GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc(
        ui + 56, pointer_of(*reinterpret_cast<unsigned*>(ui + 72)), pointer_of(*reinterpret_cast<unsigned*>(ui + 80)), text);
}
static inline int text_width16(const unsigned short* text) {
    unsigned char* ui = ui_bytes();
    return GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs(
        ui + 56, pointer_of(*reinterpret_cast<unsigned*>(ui + 72)), pointer_of(*reinterpret_cast<unsigned*>(ui + 80)), text);
}
static inline unsigned char* entry_of(const int* words) {
    return static_cast<unsigned char*>(pointer_of(static_cast<unsigned>(words[493]))) + 620 * words[479];
}
static void call_key_down_135(void* self) {
    unsigned char* table = static_cast<unsigned char*>(pointer_of(*reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 32)));
    const VtEntry* entry = reinterpret_cast<const VtEntry*>(table + 40);
    entry->function(static_cast<unsigned char*>(self) + entry->delta, 135);
}

void OnDrawSelf__12VIWndConnect(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    int* words = static_cast<int*>(self);
    void* const ui = pointer_of(_5VIWnd$mUI);
    unsigned char* const w1 = bytes + 1176;
    unsigned char* const w2 = bytes + 1544;
    char text[256];
    unsigned short wide[512];
    if (VIMemCardStatus__FPi(nullptr) == 1) {
        call_key_down_135(self);
        return;
    }
    words[491] = 0;
    if (_5VIWnd$mFocus != word_of(self) || words[480] != 0) ResetTickPulse__10VIUIWindow(w1);
    if (words[478] != 0) {
        RemoveChild__5VIWndP5VIWnd(self, pointer_of(static_cast<unsigned>(words[478])));
        words[478] = 0;
    }
    if (words[480] != 0 && words[481] == 0) {
        const long long now = static_cast<long long>(GetTime__7VITimer());
        if (*reinterpret_cast<long long*>(bytes + 1984) < now) {
            ezNetCtlDownInterface(0);
            SoundPlay__4VIUI11VIUISoundId(pointer_of(_5VIWnd$mUI), 8);
            *reinterpret_cast<unsigned long long*>(bytes + 1984) = GetTime__7VITimer() + 100000;
        }
    }
    Begin2D__4VIUI(ui);
    Draw__10VIUIWindow(w1);
    DrawFrame__10VIUIWindowiiiiUi(w1, 40, 1, *reinterpret_cast<int*>(w1 + 20) - 80, 2, 32);
    sprintf(text, "%d", *reinterpret_cast<int*>(entry_of(words)));
    TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(w1, T__10VIUIWindowPCc(w1, text), 40, 1, 60, 1, 0, 0);
    TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(w1, T__10VIUIWindowPCc(w1, reinterpret_cast<const char*>(entry_of(words) + 4)), 60, 1,
                                              *reinterpret_cast<int*>(w1 + 20) - 100, 1, 0, 2);
    TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(w1, T__10VIUIWindowPCc(w1, reinterpret_cast<const char*>(entry_of(words) + 260)), 60, 2,
                                              *reinterpret_cast<int*>(w1 + 20) - 100, 1, 0, 2);
    str8to16__8VIStringPUsUiPCc(wide, 1024, "|x ");
    strcat16__8VIStringPUsUiPCUs(wide, 1024, string_for(756));
    strcat16__8VIStringPUsUiPCc(wide, 1024, "   |c ");
    strcat16__8VIStringPUsUiPCUs(wide, 1024, string_for(750));
    Text__10VIUIWindowPCUsii9VIUICOLORUi(w1, wide, 40, 4, 0, 0);
    if (--dot_delay <= 0) {
        dot_delay = 15;
        ++dot;
        if (dot == 3) dot = 0;
    }
    if (words[480] != 0) {
        ezNetCtlGetStatus(sStatus);
        if (words[481] != 0) {
            const int r = unet_startup_async_status__Fv();
            if (r == 0 && final_delay == 0) {
                unet_shutdown__Fv();
                SetFocus__5VIWndP5VIWndb(self, nullptr, true);
            } else {
                if (final_delay != 0) --final_delay;
                SetTitle__10VIUIWindowPCUs(w2, T__10VIUIWindowPCc(w2, ""));
                strcpy16__8VIStringPUsUiPCUs(wide, 1024, string_for(776));
                words[491] = 1;
                const int width = text_width16(wide);
                const int position[2] = {0, 0};
                const int size[2] = {words[297], words[298]};
                Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(w2, _5VIWnd$mUI, position, size, (width < 200 ? 200 : width) + 120, 3, 0);
                for (signed char i = 0; i < dot + 1; ++i) strcat16__8VIStringPUsUiPCc(wide, 1024, ".");
                Draw__10VIUIWindow(w2);
                TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(w2, wide, 40, 1, *reinterpret_cast<int*>(w2 + 20), 1, 0, 0);
            }
        } else {
            SetTitle__10VIUIWindowPCc(w2, reinterpret_cast<const char*>(entry_of(words) + 260));
            const int width = text_width(reinterpret_cast<const char*>(entry_of(words) + 260));
            const int position[2] = {0, 0};
            const int size[2] = {words[297], words[298]};
            Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(w2, _5VIWnd$mUI, position, size, (width < 300 ? 300 : width) + 120, 4, 0);
            Draw__10VIUIWindow(w2);
            strcpy(text, reinterpret_cast<const char*>(sStatus + 16));
            if (strcmp(text, "Connected") != 0 && strncmp(text, "CONNECT ", 8) != 0) {
                if (strcasecmp(text, "Detached") == 0) {
                    call_key_down_135(self);
                    const int prompt_width = text_width(reinterpret_cast<const char*>(entry_of(words) + 260));
                    str8to16__8VIStringPUsUiPCc(wide, 1024, "\"");
                    strcat16__8VIStringPUsUiPCc(wide, 1024, reinterpret_cast<const char*>(entry_of(words) + 260));
                    strcat16__8VIStringPUsUiPCc(wide, 1024, "\"\n");
                    if (strstr(reinterpret_cast<const char*>(entry_of(words) + 260), "SCE/Ethernet (Network Adaptor)") != nullptr ||
                        strstr(reinterpret_cast<const char*>(entry_of(words) + 260), "SCE/Modem (Network Adaptor)") != nullptr) {
                        snprintf16__8VIStringPUsiPCUse(wide, 1024, string_for(1026));
                    } else {
                        strcat16__8VIStringPUsUiPCUs(wide, 1024, string_for(780));
                    }
                    unsigned char* const window = bytes + 40;
                    const int vect[2] = {words[297], words[298]};
                    Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(window, string_for(767), wide, 0, vect,
                                                                           (prompt_width < 300 ? 300 : prompt_width) + 80);
                    *reinterpret_cast<int*>(window + 28) = 1;
                    SetFocus__5VIWndP5VIWndb(self, window, true);
                } else {
                    for (signed char i = 0; i < dot + 1; ++i) strcat(text, ".");
                }
            }
            strcpy16__8VIStringPUsUiPCUs(wide, 1024, string_for(803));
            strcat16__8VIStringPUsUiPCc(wide, 1024, " ");
            strcat16__8VIStringPUsUiPCc(wide, 1024, text);
            TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(w2, wide, 40, 1, *reinterpret_cast<int*>(w2 + 20) - 80, 1, 0, 0);
            str8to16__8VIStringPUsUiPCc(wide, 1024, "|c ");
            strcat16__8VIStringPUsUiPCUs(wide, 1024, string_for(753));
            TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(w2, wide, 40, 3, *reinterpret_cast<int*>(w2 + 20) - 80, 1, 0, 0);
            switch (*reinterpret_cast<int*>(sStatus + 12)) {
            case 3:
                if (--final_delay == 0) {
                    final_delay = 30;
                    words[481] = 1;
                    unet_startup_async__Fv();
                    }
                break;
            case 2:
                final_delay = 60;
                printf("retrying... (reset timeout)\n");
                *reinterpret_cast<unsigned long long*>(bytes + 1984) =
                    GetTime__7VITimer() + *reinterpret_cast<unsigned long long*>(bytes + 1976);
                break;
            case 5: {
                final_delay = 60;
                unsigned char* const window = bytes + 40;
                const unsigned short* first = string_for(771);
                const unsigned short* second = string_for(807);
                const int vect[2] = {words[297], words[298]};
                Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(window, first, second, 0, vect, 400);
                *reinterpret_cast<int*>(window + 28) = 1;
                SetFocus__5VIWndP5VIWndb(self, window, true);
                words[480] = 0;
                words[481] = 0;
                break;
            }
            default:
                final_delay = 60;
                break;
            }
        }
    }
    if (words[489] != 0 && --words[489] == 0) {
        SoundPlay__4VIUI11VIUISoundId(pointer_of(_5VIWnd$mUI), 15);
        words[480] = 1;
        BeginConnect__12VIWndConnect(self);
        Close__10VIUIWindowb(w2, true);
        const int width = text_width(reinterpret_cast<const char*>(entry_of(words) + 260));
        const int position[2] = {0, 0};
        const int size[2] = {words[297], words[298]};
        Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(w2, _5VIWnd$mUI, position, size, (width < 300 ? 300 : width) + 120, 4, 0);
        Open__10VIUIWindowb(w2, false);
    }
    End2D__4VIUI(ui);
}
}
