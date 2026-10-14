// Observed: SetFocus__5VIWndP5VIWndb at 0x010c31f8 is 316 bytes. With (this, window, force) and
// the focus word mFocus (_5VIWnd$mFocus, 0x01192c74), it first calls scePrintf with the format
// "VIWnd :: SetFocus (this = %d, Window = %d, mFocus = %d)\n" and the three values. If force is
// false it stores window into mFocus and returns. Otherwise:
//  - if mFocus is nonzero: scePrintf("Calling OnMessage for mFocus = %d, Window = %d\n", mFocus,
//    window), then the object at mFocus gets its table entry at byte offset 48 of its table
//    (table word at object+32) called with the entry's halfword adjustment added to the
//    object, and (mFocus, 295, mFocus, window) as the remaining arguments;
//  - if window is null: mFocus becomes null;
//  - otherwise scePrintf("Calling OnMessage for Window = %d\n", window), then, when the word
//    mcErrOn is zero, window's own entry at offset 48 is called (adjusted window; mFocus, 295,
//    mFocus, window) and mFocus becomes window; when mcErrOn is nonzero, scePrintf("\tWindow =
//    %d, sWndMcErr = %d\n", window, &sWndMcErr) is printed (a tab starts the text), then
//    OnMessage__10VIWndMcErrP5VIWndiii is called on &sWndMcErr with (mFocus, 295, mFocus,
//    window) and mFocus becomes &sWndMcErr.
// The routine returns nothing. Observed from the class table symbols: the entry at offset 48 of
// the base table is OnMessage__5VIWndP5VIWndiii.
// Inferred: the pointer-valued printf arguments are printed with %d as 32-bit values.
// Unknown: what message 295 means, and the type of sWndMcErr beyond being 0x210 bytes.
extern "C" {
int scePrintf(const char* format, ...);
void OnMessage__10VIWndMcErrP5VIWndiii(void* self, void* from, int message, int a, int b);
extern unsigned _5VIWnd$mFocus;
extern unsigned char sWndMcErr[];
extern int mcErrOn;

struct VtEntry {
    short delta;
    short index;
    void (*function)(void*, void*, int, int, int);
};

static inline unsigned word_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word));
}
static inline void send_message(void* target, void* from, int message, int a, int b) {
    unsigned char* object = static_cast<unsigned char*>(target);
    const VtEntry* entry =
        reinterpret_cast<const VtEntry*>(pointer_of(*reinterpret_cast<unsigned*>(object + 32))) + 6;
    entry->function(object + entry->delta, from, message, a, b);
}

void SetFocus__5VIWndP5VIWndb(void* self, void* window, bool force) {
    scePrintf("VIWnd :: SetFocus (this = %d, Window = %d, mFocus = %d)\n", word_of(self), word_of(window),
              _5VIWnd$mFocus);
    if (!force) {
        _5VIWnd$mFocus = word_of(window);
        return;
    }
    if (_5VIWnd$mFocus != 0) {
        scePrintf("Calling OnMessage for mFocus = %d, Window = %d\n", _5VIWnd$mFocus, word_of(window));
        void* focus = pointer_of(_5VIWnd$mFocus);
        send_message(focus, focus, 295, word_of(focus), word_of(window));
    }
    if (window == 0) {
        _5VIWnd$mFocus = 0;
        return;
    }
    scePrintf("Calling OnMessage for Window = %d\n", word_of(window));
    if (mcErrOn == 0) {
        void* focus = pointer_of(_5VIWnd$mFocus);
        send_message(window, focus, 295, word_of(focus), word_of(window));
        _5VIWnd$mFocus = word_of(window);
    } else {
        scePrintf("\tWindow = %d, sWndMcErr = %d\n", word_of(window), word_of(sWndMcErr));
        void* focus = pointer_of(_5VIWnd$mFocus);
        OnMessage__10VIWndMcErrP5VIWndiii(sWndMcErr, focus, 295, word_of(focus), word_of(window));
        _5VIWnd$mFocus = word_of(sWndMcErr);
    }
}
}
