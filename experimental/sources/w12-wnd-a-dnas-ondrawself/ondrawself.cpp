// Observed: OnDrawSelf__9VIWndDnas at 0x010a7860 is 1516 bytes. ui = the word _5VIWnd$mUI (0x01192c70);
// text(id) = GetString__13VIStringTableii(ui, id, word at ui+48); w is the embedded UI window at this+784;
// ctx is the pointer word at this+776. The 16-byte status block at this+1152 is passed to
// DnasErrorString__9VIWndDnasP14sceDNAS2Status; its first word is the state, the next two words are
// a code (this+1156) and a sub-state (this+1160). The local counter ellipseCount.2103 (0x01192aec) is
// incremented first on every call. Then:
//  1. When the word at this+780 is -1: FindTyped__12VIDictionaryUi14VIResourceTypeRi(ctx[19352],
//     VIHashResourceID_Icon__FPCc("dnas_64"), 1, &this[780]). Begin2D__4VIUI(ui);
//     DrawFakeSplashInterface__9VIWndDnas(this); store 0 at this+1168.
//  2. buffer = strcpy16(text(803)) then strcat16 of " " (1024-byte buffer). show = 0 (a flag "s5").
//  3. When the 64-bit value at this+1152 is zero: show = 1 and append text(777). Otherwise on state - 1 (an
//     unsigned index below 9, any other state appends nothing):
//       0 (state 1): append text(762);
//       1, 2, 3 (states 2 to 4): on the sub-state (below 9, else show = 1 and nothing appended): 0 -> show = 1,
//         append text(775); 1 -> show = 1, append text(758), then the dot animation below; 2 -> show = 1, append
//         text(748); 3 -> show = 0, append text(749); 4 and 6 -> show = 1, nothing; 5 -> show = 1, text(766);
//         7 -> show = 1, text(765); 8 -> show = 1, text(774);
//       4 (state 5): append text(764);
//       5, 6 (states 6, 7): store 1 at this+1168 and replace the buffer by
//         strcpy16(DnasErrorString__9VIWndDnasP14sceDNAS2Status(this, this+1152));
//       7, 8 (states 8, 9): nothing.
//     The dot animation: c = ellipseCount; when c is 121 or more it is reset to 0 first. The buffer is then
//     rewritten with snprintf16__8VIStringPUsiPCUse(buffer, 1024, buffer) when c < 30, or with the format
//     "%S." (c < 60), "%S.." (c < 90), "%S..." (otherwise) taking the buffer as its one argument.
//  4. When the word at this+1168 is nonzero show = 0. extra = 150 when show else 40.
//     lines = CountLines__10VIUIWindowPCUsi(w, buffer, (w[20] - extra) - 40).
//     Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(w, ui, {0, 0}, {320, 246}, 470, n, 0) with n = lines + 3 when
//     this+1168 is nonzero, 10 when show, else lines + 2. Draw__10VIUIWindow(w).
//  5. When this[780] >= 0 and show: Blit__8VIRasteriUiRC8VIVect2I16VIAlphaBlendModef(raster, this[780], 0,
//     {w[12] - w[20]/2 + 20, w[16] - w[24]/2 + 13}, 3, (float word at w+40)) where raster = ctx[19356].
//  6. When show: TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(w, buffer, extra, 3, w[20] - extra - 40, 4, 0, 0) and
//     TextBox(w, text(1857), 40, 8, w[20] - 80, 2, 0, 8); else TextBox(w, buffer, extra, 1, w[20] - extra - 40,
//     lines, 0, 0).
//  7. When the word at this+1168 is nonzero: buffer = "|x " (str8to16__8VIStringPUsUiPCc), then append text(786) when
//     the word at this+1156 equals -9999 else text(760); Text__10VIUIWindowPCUsii9VIUICOLORUi(w, buffer, 40, 6, 0,
//     0) when show (never, since show was cleared) else (w, buffer, 40, lines + 2, 0, 0). End2D__4VIUI(ui).
// Inferred: the DNAS authentication screen: a status text with an animated dot suffix while connecting, an error text once
// the state reaches 6 or 7; show marks the states that use the fixed 150-unit left margin and the icon.
// Unknown: the meanings of the status words and of the sub-states, the string ids (apart from their use here),
// the type behind the colour argument, and the true struct of the status block (a 16-byte guess is not used).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<__UINTPTR_TYPE__>(norrath_host_pointer(word))
#else
#define NORRATH_WORD_ADDRESS(word) static_cast<__UINTPTR_TYPE__>(word)
#endif
extern "C" {
extern unsigned _5VIWnd$mUI;
extern int norrath_dnas_ellipse_count __asm__("ellipseCount.2103");

struct Pair {
    int a, b;
};
unsigned VIHashResourceID_Icon__FPCc(const char* name);
void FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* dictionary, unsigned hash, int type, int* out);
void Begin2D__4VIUI(unsigned ui);
void End2D__4VIUI(unsigned ui);
void DrawFakeSplashInterface__9VIWndDnas(void* self);
const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg);
void strcpy16__8VIStringPUsUiPCUs(unsigned short* destination, unsigned size, const unsigned short* source);
void strcat16__8VIStringPUsUiPCUs(unsigned short* destination, unsigned size, const unsigned short* source);
void strcat16__8VIStringPUsUiPCc(unsigned short* destination, unsigned size, const char* source);
void str8to16__8VIStringPUsUiPCc(unsigned short* destination, unsigned size, const char* source);
int snprintf16__8VIStringPUsiPCUse(unsigned short* destination, int size, const unsigned short* format, ...);
int snprintf16__8VIStringPUsiPCce(unsigned short* destination, int size, const char* format, ...);
const unsigned short* DnasErrorString__9VIWndDnasP14sceDNAS2Status(void* self, void* status);
int CountLines__10VIUIWindowPCUsi(void* window, const unsigned short* text, int width);
int Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, void* ui, const Pair* position, const Pair* size, int a,
                                              int b, int c);
void Draw__10VIUIWindow(void* window);
void Blit__8VIRasteriUiRC8VIVect2I16VIAlphaBlendModef(void* raster, int id, unsigned flags, const Pair* position,
                                                      int mode, float scale);
void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int c, int d,
                                               int color, unsigned flags);
void Text__10VIUIWindowPCUsii9VIUICOLORUi(void* window, const unsigned short* text, int x, int y, int color,
                                          unsigned flags);

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

void OnDrawSelf__9VIWndDnas(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    norrath_dnas_ellipse_count += 1;
    void* context = pointer_of(static_cast<unsigned>(word(self, 776)));
    const unsigned ui = _5VIWnd$mUI;
    void* raster = pointer_of(static_cast<unsigned>(word(context, 19356)));
    if (word(self, 780) == -1)
        FindTyped__12VIDictionaryUi14VIResourceTypeRi(pointer_of(static_cast<unsigned>(word(context, 19352))),
                                                      VIHashResourceID_Icon__FPCc("dnas_64"), 1,
                                                      reinterpret_cast<int*>(bytes + 780));
    Begin2D__4VIUI(ui);
    bool show = false;
    DrawFakeSplashInterface__9VIWndDnas(self);
    word(self, 1168) = 0;
    unsigned short buffer[512];
    strcpy16__8VIStringPUsUiPCUs(buffer, 1024, text(803));
    strcat16__8VIStringPUsUiPCc(buffer, 1024, " ");
    int append = 0;
    if (*reinterpret_cast<long long*>(bytes + 1152) == 0) {
        show = true;
        append = 777;
    } else {
        switch (static_cast<unsigned>(word(self, 1152) - 1)) {
        case 0:
            append = 762;
            break;
        case 1:
        case 2:
        case 3: {
            const unsigned sub = static_cast<unsigned>(word(self, 1160));
            show = true;
            if (sub >= 9) break;
            switch (sub) {
            case 0: append = 775; break;
            case 1: {
                strcat16__8VIStringPUsUiPCUs(buffer, 1024, text(758));
                int count = norrath_dnas_ellipse_count;
                if (count > 120) {
                    norrath_dnas_ellipse_count = 0;
                    count = 0;
                }
                if (count < 30) snprintf16__8VIStringPUsiPCUse(buffer, 1024, buffer);
                else if (count < 60) snprintf16__8VIStringPUsiPCce(buffer, 1024, "%S.", buffer);
                else if (count < 90) snprintf16__8VIStringPUsiPCce(buffer, 1024, "%S..", buffer);
                else snprintf16__8VIStringPUsiPCce(buffer, 1024, "%S...", buffer);
                break;
            }
            case 2: append = 748; break;
            case 3: show = false; append = 749; break;
            case 5: append = 766; break;
            case 7: append = 765; break;
            case 8: append = 774; break;
            default: break;
            }
            break;
        }
        case 4:
            append = 764;
            break;
        case 5:
        case 6:
            word(self, 1168) = 1;
            strcpy16__8VIStringPUsUiPCUs(buffer, 1024, DnasErrorString__9VIWndDnasP14sceDNAS2Status(self, bytes + 1152));
            break;
        default:
            break;
        }
    }
    if (append != 0) strcat16__8VIStringPUsUiPCUs(buffer, 1024, text(append));
    void* window = bytes + 784;
    if (word(self, 1168) != 0) show = false;
    const int extra = show ? 150 : 40;
    const int lines = CountLines__10VIUIWindowPCUsi(window, buffer, word(window, 20) - extra - 40);
    const Pair origin = {0, 0};
    const Pair size = {320, 246};
    if (word(self, 1168) != 0)
        Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, pointer_of(ui), &origin, &size, 470, lines + 3, 0);
    else if (show)
        Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, pointer_of(ui), &origin, &size, 470, 10, 0);
    else
        Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, pointer_of(ui), &origin, &size, 470, lines + 2, 0);
    Draw__10VIUIWindow(window);
    if (word(self, 780) >= 0 && show) {
        const Pair position = {word(window, 12) - (word(window, 20) >> 1) + 20,
                               word(window, 16) - (word(window, 24) >> 1) + 13};
        Blit__8VIRasteriUiRC8VIVect2I16VIAlphaBlendModef(raster, word(self, 780), 0, &position, 3,
                                                         *reinterpret_cast<float*>(bytes + 784 + 40));
    }
    if (show) {
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, buffer, extra, 3, word(window, 20) - extra - 40, 4, 0, 0);
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text(1857), 40, 8, word(window, 20) - 80, 2, 0, 8);
    } else {
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, buffer, extra, 1, word(window, 20) - extra - 40, lines, 0, 0);
    }
    if (word(self, 1168) != 0) {
        str8to16__8VIStringPUsUiPCc(buffer, 1024, "|x ");
        if (word(self, 1156) == -9999) strcat16__8VIStringPUsUiPCUs(buffer, 1024, text(786));
        else strcat16__8VIStringPUsUiPCUs(buffer, 1024, text(760));
        if (show) Text__10VIUIWindowPCUsii9VIUICOLORUi(window, buffer, 40, 6, 0, 0);
        else Text__10VIUIWindowPCUsii9VIUICOLORUi(window, buffer, 40, lines + 2, 0, 0);
    }
    End2D__4VIUI(ui);
}
}
