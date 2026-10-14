// Observed: OnMessage__12VIWndConnectP5VIWndiii at 0x010a2088 is 544 bytes. With (this, from, message, p1, p2),
// mUI being the word _5VIWnd$mUI (0x01192c70) and w = this + 1176 (an embedded UI window):
//  - 4352: AddChild__5VIWndP5VIWnd(this, from);
//  - 4354: the word at +1912 is set to from;
//  - 307: SetFocus__5VIWndP5VIWndb(this, this, 1); 4356: SetFocus(this, from, 1);
//  - 4358: when p1 is not 3, SetFocus(this, this, 1) and the word at +1956 is set to 3. When p1 is 3: the word at
//    +1960 is set to 1, SoundPlay__4VIUI11VIUISoundId(mUI, 8), the word at +28 is cleared, SetFocus(this, the
//    word at +20, 1), and the word at +1920 is cleared;
//  - 259: nothing;
//  - 295: nothing unless p2 equals this. Then Close(w, 1); Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(w, mUI,
//    &{0, 0}, &{320, 246}, 400, 5, 0); Open(w, 1) when p1 equals McErrWnd__Fv() and Open(w, 0) otherwise;
//    SetTitle__10VIUIWindowPCUs(w, GetString__13VIStringTableii(mUI, 815, the word at mUI + 48)); the word at
//    +1924 is cleared; and when p1 differs from a second McErrWnd__Fv() call, the child at +1912, if nonzero, is
//    removed with RemoveChild__5VIWndP5VIWnd(this, child) and the word is cleared. The word at +1960 is cleared
//    last on every path;
//  - any other message: OnMessage__5VIWndP5VIWndiii(this, from, message, p1, p2).
// Nothing is returned.
// Inferred: the pairs are position and size (callee name: two VIVect2I references); 4352..4358 are messages
// of this window set (the generic renderer handles the same numbers); the second McErrWnd comparison is how
// the original decides to drop the child.
// Unknown: the meaning of the words at +1912, +1920, +1924, +1956, +1960, of 400 and 5, and of the message
// numbers; the return type of McErrWnd__Fv (a pointer is assumed from its other caller).
extern "C" {
void AddChild__5VIWndP5VIWnd(void* self, void* child);
void RemoveChild__5VIWndP5VIWnd(void* self, void* child);
void SetFocus__5VIWndP5VIWndb(void* self, void* target, bool flag);
void OnMessage__5VIWndP5VIWndiii(void* self, void* from, int message, void* p1, void* p2);
void SoundPlay__4VIUI11VIUISoundId(const void* ui, int sound);
void Close__10VIUIWindowb(void* window, bool flag);
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, unsigned ui, const int* position, const int* size,
                                              int a, int b, int c);
void Open__10VIUIWindowb(void* window, bool flag);
void SetTitle__10VIUIWindowPCUs(void* window, const unsigned short* text);
const unsigned short* GetString__13VIStringTableii(const void* table, int id, int column);
void* McErrWnd__Fv();
extern unsigned _5VIWnd$mUI;

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<unsigned long>(word));
}
static inline unsigned word_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

void OnMessage__12VIWndConnectP5VIWndiii(void* self, void* from, int message, void* p1, void* p2) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    int* words = static_cast<int*>(self);
    switch (message) {
    case 4352:
        AddChild__5VIWndP5VIWnd(self, from);
        break;
    case 4354:
        words[478] = static_cast<int>(word_of(from));
        break;
    case 307:
        SetFocus__5VIWndP5VIWndb(self, self, true);
        break;
    case 4356:
        SetFocus__5VIWndP5VIWndb(self, from, true);
        break;
    case 4358:
        if (word_of(p1) == 3) {
            words[490] = 1;
            SoundPlay__4VIUI11VIUISoundId(pointer_of(_5VIWnd$mUI), 8);
            words[7] = 0;
            SetFocus__5VIWndP5VIWndb(self, pointer_of(static_cast<unsigned>(words[5])), true);
            words[480] = 0;
        } else {
            SetFocus__5VIWndP5VIWndb(self, self, true);
            words[489] = 3;
        }
        break;
    case 259:
        break;
    case 295: {
        if (p2 != self) break;
        void* window = bytes + 1176;
        Close__10VIUIWindowb(window, true);
        const int position[2] = {0, 0};
        const int size[2] = {320, 246};
        Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, _5VIWnd$mUI, position, size, 400, 5, 0);
        Open__10VIUIWindowb(window, p1 == McErrWnd__Fv());
        const void* table = pointer_of(_5VIWnd$mUI);
        SetTitle__10VIUIWindowPCUs(window, GetString__13VIStringTableii(table, 815, static_cast<const int*>(table)[12]));
        words[481] = 0;
        if (p1 != McErrWnd__Fv()) {
            if (void* child = pointer_of(static_cast<unsigned>(words[478]))) {
                RemoveChild__5VIWndP5VIWnd(self, child);
                words[478] = 0;
            }
        }
        words[490] = 0;
        break;
    }
    default:
        OnMessage__5VIWndP5VIWndiii(self, from, message, p1, p2);
        break;
    }
}
}
