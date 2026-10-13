// Observed: OnMessage__16VIWndReadMessageP5VIWndiii at 0x010d1358 is 200 bytes. With (this, from, message, p1,
// p2): any message other than 295 returns at once (the base handler is not called). For message 295 nothing
// happens unless p2 equals this. When it does, with the embedded UI window w = this + 40: Close(w, 1);
// Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(w, mUI, &{0, 0}, &{320, 246}, 440, 10, 0) where mUI is the word
// _5VIWnd$mUI (0x01192c70); SetTitle__10VIUIWindowPCUs(w, GetString__13VIStringTableii(mUI, 789, the word at
// mUI + 48)); Open(w, 0); and the word at +16804 is set to 1.
// Inferred: the pairs are position and size (callee name: two VIVect2I references); the word at mUI + 48
// selects the text column, from the argument position only.
// Unknown: the meaning of 440, 10 and +16804; the return type (void).
extern "C" {
void Close__10VIUIWindowb(void* window, bool flag);
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, unsigned ui, const int* position, const int* size,
                                              int a, int b, int c);
void Open__10VIUIWindowb(void* window, bool flag);
void SetTitle__10VIUIWindowPCUs(void* window, const unsigned short* text);
const unsigned short* GetString__13VIStringTableii(const void* table, int id, int column);
extern unsigned _5VIWnd$mUI;

void OnMessage__16VIWndReadMessageP5VIWndiii(void* self, void*, int message, int, void* p2) {
    if (message != 295) return;
    if (p2 != self) return;
    unsigned char* bytes = static_cast<unsigned char*>(self);
    void* window = bytes + 40;
    Close__10VIUIWindowb(window, true);
    const int position[2] = {0, 0};
    const int size[2] = {320, 246};
    Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, _5VIWnd$mUI, position, size, 440, 10, 0);
    const void* table = reinterpret_cast<const void*>(static_cast<unsigned long>(_5VIWnd$mUI));
    SetTitle__10VIUIWindowPCUs(window, GetString__13VIStringTableii(table, 789, static_cast<const int*>(table)[12]));
    Open__10VIUIWindowb(window, false);
    *reinterpret_cast<int*>(bytes + 16804) = 1;
}
}
