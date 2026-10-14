// Observed: OnMessage__12VIWndMessageP5VIWndiii at 0x010c12e8 is 196 bytes. With (this, from, message, p1, p2):
// any message other than 295 is passed unchanged to OnMessage__5VIWndP5VIWndiii. For message 295 nothing
// happens unless p2 equals this. When it does, with the embedded UI window w = this + 40: Close(w, 1);
// Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(w, mUI, &{0, 0}, this + 988, word at +996, word at +1000, 0)
// where mUI is the word _5VIWnd$mUI (0x01192c70); Open(w, 0); SetTitle__10VIUIWindowPCUs(w, T__10VIUIWindowPCUs(w,
// this + 408)); and the word at +984 is set to 1 when the word at +1004 is nonzero, else to 0.
// Inferred: the 8-byte pair at +988 is the window size and +408 a text entry the window translates for its
// title; the stack pair {0, 0} is the position (callee name: two VIVect2I references).
// Unknown: the meaning of +984, +1004 and the last integer arguments; the result type of T__10VIUIWindowPCUs
// (a pointer to 16-bit text is assumed because SetTitle takes one); the return type (void).
extern "C" {
void Close__10VIUIWindowb(void* window, bool flag);
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, unsigned ui, const int* position, const int* size,
                                              int a, int b, int c);
void Open__10VIUIWindowb(void* window, bool flag);
const unsigned short* T__10VIUIWindowPCUs(void* window, const void* entry);
void SetTitle__10VIUIWindowPCUs(void* window, const unsigned short* text);
void OnMessage__5VIWndP5VIWndiii(void* self, void* from, int message, int p1, void* p2);
extern unsigned _5VIWnd$mUI;

void OnMessage__12VIWndMessageP5VIWndiii(void* self, void* from, int message, int p1, void* p2) {
    if (message != 295) {
        OnMessage__5VIWndP5VIWndiii(self, from, message, p1, p2);
        return;
    }
    if (p2 != self) return;
    unsigned char* bytes = static_cast<unsigned char*>(self);
    int* words = static_cast<int*>(self);
    void* window = bytes + 40;
    Close__10VIUIWindowb(window, true);
    const int position[2] = {0, 0};
    Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, _5VIWnd$mUI, position, words + 247, words[249], words[250], 0);
    Open__10VIUIWindowb(window, false);
    SetTitle__10VIUIWindowPCUs(window, T__10VIUIWindowPCUs(window, bytes + 408));
    words[246] = words[251] != 0;
}
}
