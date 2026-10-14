// Observed: OnMessage__12VIWndOptionsP5VIWndiii at 0x010c4660 is 848 bytes. With (this, from, message, p1, p2) it
// first calls VIGetGameOptions__Fv() (g), then, with mUI the word _5VIWnd$mUI (0x01192c70):
//  - 4352: AddChild__5VIWndP5VIWnd(this, from); the word at +1196 is set to McErrWnd__Fv();
//  - 4354: the word at +1196 is set to from;
//  - 4356: SetFocus__5VIWndP5VIWndb(this, from, 1);
//  - 4358: SetFocus(this, this, 1) and the word at +1184 is cleared; then when p1 is 0 the word at +28 is
//    cleared and SetFocus(this, the word at +20, 1); when p1 is 1 the word at +1188 is set to 4;
//  - 307: SetFocus(this, this, 1); then only when from equals this + 1232 and p1 is 0: g[147] and g[148]
//    become (this byte +1174 == 0) and (this byte +1175 == 0), g[149] and g[151] copy the bytes at +1176 and
//    +1177, and the word at +1188 is set to 4;
//  - 259: nothing;
//  - 295 (focus change): see the second part below;
//  - any other message: OnMessage__5VIWndP5VIWndiii(this, from, message, p1, p2).
// The 295 case: unless p2 equals this (then skip the first part), p1 equals McErrWnd__Fv() or p1 equals
// this + 1232, it runs the first part with w = this + 40: Close(w, 1); Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(
// w, mUI, &{0, 0}, &{320, 246}, 300, 7, 0); Open(w, 0); SetTitle__10VIUIWindowPCUs(w, GetString__13VIStringTableii(
// mUI, 788, the word at mUI + 48)); the word at +1260 is cleared; the old word at +1196 is read; the fields of
// this are loaded from g: word +1156 = g[6]; bytes +1160 = g[7], +1161 = g[4], +1162 = g[5], +1172 = g[145],
// +1173 = (g[146] == 0), +1174 = (g[147] == 0), +1175 = (g[148] == 0), +1176 = g[149], +1177 = g[151]; floats
// +1164 = g float at 8, +1168 = g float at 12; words +1152 and +1144 = -1; words +1180, +1184, +1188, +1192
// = 0; and when the old +1196 word was nonzero RemoveChild__5VIWndP5VIWnd(this, it) is called and +1196 is
// cleared. Second part, for every 295 message that did not return early: when p1 equals this and p2 differs
// from a second McErrWnd__Fv() result, Unload__13VIBattleMusic(this + 1200) is called.
// Inferred: g is the game options record: the window copies it in when it gains focus; +1232 is an embedded
// message window; the pairs are position and size (callee name).
// Unknown: field names, what the option bytes mean, the type of g, and the return type (void).
extern "C" {
unsigned char* VIGetGameOptions__Fv();
void AddChild__5VIWndP5VIWnd(void* self, void* child);
void RemoveChild__5VIWndP5VIWnd(void* self, void* child);
void SetFocus__5VIWndP5VIWndb(void* self, void* target, bool flag);
void OnMessage__5VIWndP5VIWndiii(void* self, void* from, int message, void* p1, void* p2);
void Close__10VIUIWindowb(void* window, bool flag);
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, unsigned ui, const int* position, const int* size,
                                              int a, int b, int c);
void Open__10VIUIWindowb(void* window, bool flag);
void SetTitle__10VIUIWindowPCUs(void* window, const unsigned short* text);
const unsigned short* GetString__13VIStringTableii(const void* table, int id, int column);
void* McErrWnd__Fv();
void Unload__13VIBattleMusic(void* music);
extern unsigned _5VIWnd$mUI;

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<unsigned long>(word));
}
static inline unsigned word_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

void OnMessage__12VIWndOptionsP5VIWndiii(void* self, void* from, int message, void* p1, void* p2) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    int* words = static_cast<int*>(self);
    unsigned char* g = VIGetGameOptions__Fv();
    switch (message) {
    case 4352:
        AddChild__5VIWndP5VIWnd(self, from);
        words[299] = static_cast<int>(word_of(McErrWnd__Fv()));
        break;
    case 4354:
        words[299] = static_cast<int>(word_of(from));
        break;
    case 4356:
        SetFocus__5VIWndP5VIWndb(self, from, true);
        break;
    case 4358:
        SetFocus__5VIWndP5VIWndb(self, self, true);
        words[296] = 0;
        if (p1 == nullptr) {
            words[7] = 0;
            SetFocus__5VIWndP5VIWndb(self, pointer_of(static_cast<unsigned>(words[5])), true);
        } else if (word_of(p1) == 1) {
            words[297] = 4;
        }
        break;
    case 307:
        SetFocus__5VIWndP5VIWndb(self, self, true);
        if (from == bytes + 1232 && p1 == nullptr) {
            g[147] = bytes[1174] == 0;
            g[148] = bytes[1175] == 0;
            g[149] = bytes[1176];
            g[151] = bytes[1177];
            words[297] = 4;
        }
        break;
    case 259:
        break;
    case 295: {
        if (p2 == self && p1 != McErrWnd__Fv() && p1 != bytes + 1232) {
            void* window = bytes + 40;
            Close__10VIUIWindowb(window, true);
            const int position[2] = {0, 0};
            const int size[2] = {320, 246};
            Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, _5VIWnd$mUI, position, size, 300, 7, 0);
            Open__10VIUIWindowb(window, false);
            const void* table = pointer_of(_5VIWnd$mUI);
            SetTitle__10VIUIWindowPCUs(window, GetString__13VIStringTableii(table, 788, static_cast<const int*>(table)[12]));
            words[315] = 0;
            const int child = words[299];
            words[289] = g[6];
            bytes[1160] = g[7];
            bytes[1161] = g[4];
            bytes[1162] = g[5];
            *reinterpret_cast<float*>(bytes + 1164) = *reinterpret_cast<const float*>(g + 8);
            *reinterpret_cast<float*>(bytes + 1168) = *reinterpret_cast<const float*>(g + 12);
            bytes[1172] = g[145];
            bytes[1173] = g[146] == 0;
            bytes[1174] = g[147] == 0;
            bytes[1175] = g[148] == 0;
            bytes[1176] = g[149];
            words[288] = -1;
            bytes[1177] = g[151];
            words[295] = 0;
            words[286] = -1;
            words[296] = 0;
            words[297] = 0;
            words[298] = 0;
            if (child != 0) {
                RemoveChild__5VIWndP5VIWnd(self, pointer_of(static_cast<unsigned>(child)));
                words[299] = 0;
            }
        }
        if (p1 == self && p2 != McErrWnd__Fv()) Unload__13VIBattleMusic(bytes + 1200);
        break;
    }
    default:
        OnMessage__5VIWndP5VIWndiii(self, from, message, p1, p2);
        break;
    }
}
}
