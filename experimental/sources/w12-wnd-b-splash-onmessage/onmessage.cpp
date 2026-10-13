// Observed: OnMessage__11VIWndSplashP5VIWndiii at 0x010d6698 is 628 bytes. With (this, from, message,
// a, b) it first calls scePrintf with the format "VIWndSplash::OnMessage(From = %d,
// MEssageId = %d, Param1 = %d, Param2 = %d)\n" (spelling as in the image) and (from, message, a, b),
// then selects on the message; each branch prints its own line of text first (given below as the
// text after "VIWndSplash::OnMessage "):
//  - 295 "CHANGE_FOCUS": nothing more unless b equals this. Let c = this + 416. Nothing more if a is
//    c, this + 2408 or this + 4776. Otherwise, when the word at c + 1960 (this + 2376) is nonzero:
//    store zero there and at this + 9640, 9656, 9648, 9652 and 9660. Then, with w = this + 40 and
//    ui = the word _5VIWnd$mUI (0x01192c70): Init(w, ui, pos {0,190}, size {320,224}, 64, 1, 0);
//    Open(w, true); and when a differs from the result of McErrWnd__Fv(), store zero at this + 9660.
//  - 307 "MESSAGE_CLOSE": zero at this + 9648 and this + 9652, then SetFocus(this, this, true).
//  - 4352 "MCERR_ADD_CHILD": AddChild(this, from).
//  - 4354 "MCERR_REM_CHILD": store from at this + 9644.
//  - 4356 "MCERR_SET_FOCUS": SetFocus(this, from, true).
//  - 4358 "MCERR_DONE": SetFocus(this, this, true); then with s = the word at this + 9660: when s is
//    1 and a is 1 store 2 at this + 9660; when s is 1 and a is not 1 store zero at this + 9640 and
//    this + 9660; when s is not 1 add one to the word at this + 9640.
//  - any other message, after printing "DEFAULT": OnMessage__5VIWndP5VIWndiii(this, from, message,
//    a, b).
// Nothing is returned. The line texts are quoted in the code.
// Inferred: the two stack pairs are 2D integer vectors (position, size) because the Init name takes
// two VIVect2I references; the branch labels come only from the image's own strings, and the
// REM_CHILD label does not match what the branch does.
// Unknown: the purpose of the words at this + 9640..9660, of the Init integers, and of
// McErrWnd__Fv's result beyond being compared with a.
extern "C" {
int scePrintf(const char* format, ...);
void OnMessage__5VIWndP5VIWndiii(void* self, void* from, int message, void* a, void* b);
void SetFocus__5VIWndP5VIWndb(void* self, void* window, bool force);
void AddChild__5VIWndP5VIWnd(void* self, void* child);
void* McErrWnd__Fv();
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, unsigned ui, const int* position,
                                              const int* size, int a, int b, int c);
void Open__10VIUIWindowb(void* window, bool flag);
extern unsigned _5VIWnd$mUI;

static inline unsigned word_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

void OnMessage__11VIWndSplashP5VIWndiii(void* self, void* from, int message, void* a, void* b) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    scePrintf("VIWndSplash::OnMessage(From = %d, MEssageId = %d, Param1 = %d, Param2 = %d)\n", word_of(from),
              message, word_of(a), word_of(b));
    switch (message) {
    case 295: {
        scePrintf("VIWndSplash::OnMessage CHANGE_FOCUS\n");
        if (b != self) return;
        unsigned char* c = bytes + 416;
        if (a == c || a == bytes + 2408 || a == bytes + 4776) return;
        if (*reinterpret_cast<int*>(c + 1960) != 0) {
            *reinterpret_cast<int*>(c + 1960) = 0;
            *reinterpret_cast<int*>(bytes + 9640) = 0;
            *reinterpret_cast<int*>(bytes + 9656) = 0;
            *reinterpret_cast<int*>(bytes + 9648) = 0;
            *reinterpret_cast<int*>(bytes + 9652) = 0;
            *reinterpret_cast<int*>(bytes + 9660) = 0;
        }
        void* window = bytes + 40;
        const int position[2] = {0, 190};
        const int size[2] = {320, 224};
        Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, _5VIWnd$mUI, position, size, 64, 1, 0);
        Open__10VIUIWindowb(window, true);
        if (a != McErrWnd__Fv()) *reinterpret_cast<int*>(bytes + 9660) = 0;
        return;
    }
    case 307:
        scePrintf("VIWndSplash::OnMessage MESSAGE_CLOSE\n");
        *reinterpret_cast<int*>(bytes + 9648) = 0;
        *reinterpret_cast<int*>(bytes + 9652) = 0;
        SetFocus__5VIWndP5VIWndb(self, self, true);
        return;
    case 4352:
        scePrintf("VIWndSplash::OnMessage MCERR_ADD_CHILD\n");
        AddChild__5VIWndP5VIWnd(self, from);
        return;
    case 4354:
        scePrintf("VIWndSplash::OnMessage MCERR_REM_CHILD\n");
        *reinterpret_cast<unsigned*>(bytes + 9644) = word_of(from);
        return;
    case 4356:
        scePrintf("VIWndSplash::OnMessage MCERR_SET_FOCUS\n");
        SetFocus__5VIWndP5VIWndb(self, from, true);
        return;
    case 4358: {
        scePrintf("VIWndSplash::OnMessage MCERR_DONE\n");
        SetFocus__5VIWndP5VIWndb(self, self, true);
        int state = *reinterpret_cast<int*>(bytes + 9660);
        if (state == 1) {
            if (word_of(a) == 1u) {
                *reinterpret_cast<int*>(bytes + 9660) = 2;
            } else {
                *reinterpret_cast<int*>(bytes + 9640) = 0;
                *reinterpret_cast<int*>(bytes + 9660) = 0;
            }
        } else {
            *reinterpret_cast<int*>(bytes + 9640) = *reinterpret_cast<int*>(bytes + 9640) + 1;
        }
        return;
    }
    default:
        scePrintf("VIWndSplash::OnMessage DEFAULT\n");
        OnMessage__5VIWndP5VIWndiii(self, from, message, a, b);
        return;
    }
}
}
