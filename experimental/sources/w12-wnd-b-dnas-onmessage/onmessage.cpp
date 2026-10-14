// Observed: OnMessage__9VIWndDnasP5VIWndiii at 0x010a7e50 is 452 bytes. With (this, from, message, a, b):
//  - a message other than 295 is passed unchanged to OnMessage__5VIWndP5VIWndiii and nothing else
//    happens;
//  - message 295 does nothing unless b equals this, and also nothing when a equals this + 1176;
//  - otherwise it sets up three embedded UI windows, with ui = the word _5VIWnd$mUI
//    (0x01192c70) read afresh for each call:
//      w1 = this + 40:  Init(w1, ui, pos {0,350}, size {304,360}, 440, 1, 0); Open(w1, true);
//      w2 = this + 784: Close(w2, true); Init(w2, ui, {0,0}, {320,246}, 470, 5, 0); Open(w2, false);
//                       SetTitle(w2, GetString(ui, 761, *(int*)(ui + 48)));
//      w3 = this + 408: Init(w3, ui, {0,0}, {320,246}, 480, 6, 0); Open(w3, false);
//                       SetTitle(w3, GetString(ui, 767, *(int*)(ui + 48)));
//    then stores zero at this + 1168 and calls memset(this + 1152, 0, 16).
// Nothing is returned. 295 is the message SetFocus sends; b is the window gaining focus.
// Inferred: the two stack pairs are 2D integer vectors (position, size) because the Init name takes
// two VIVect2I references; GetString takes the UI object as its first argument, as the other
// callers in this window family do.
// Unknown: the meaning of the last three Init integers, of string numbers 761 and 767, of the
// word at ui + 48, and of the cleared region at this + 1152; whether a (compared with this +
// 1176) is a window pointer.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
void OnMessage__5VIWndP5VIWndiii(void* self, void* from, int message, void* a, void* b);
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, unsigned ui, const int* position,
                                              const int* size, int a, int b, int c);
void Open__10VIUIWindowb(void* window, bool flag);
void Close__10VIUIWindowb(void* window, bool flag);
void SetTitle__10VIUIWindowPCUs(void* window, const unsigned short* title);
const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg);
void* memset(void* destination, int value, unsigned long count);
extern unsigned _5VIWnd$mUI;

static inline const unsigned short* title_of(int id) {
    char* table = NORRATH_TRANSLATE(_5VIWnd$mUI);
    return GetString__13VIStringTableii(table, id, *reinterpret_cast<int*>(table + 48));
}

void OnMessage__9VIWndDnasP5VIWndiii(void* self, void* from, int message, void* a, void* b) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    if (message != 295) {
        OnMessage__5VIWndP5VIWndiii(self, from, message, a, b);
        return;
    }
    if (b != self || a == bytes + 1176) return;
    void* w1 = bytes + 40;
    void* w2 = bytes + 784;
    void* w3 = bytes + 408;
    const int pos1[2] = {0, 350};
    const int size1[2] = {304, 360};
    Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(w1, _5VIWnd$mUI, pos1, size1, 440, 1, 0);
    Open__10VIUIWindowb(w1, true);
    Close__10VIUIWindowb(w2, true);
    const int pos2[2] = {0, 0};
    const int size2[2] = {320, 246};
    Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(w2, _5VIWnd$mUI, pos2, size2, 470, 5, 0);
    Open__10VIUIWindowb(w2, false);
    SetTitle__10VIUIWindowPCUs(w2, title_of(761));
    Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(w3, _5VIWnd$mUI, pos2, size2, 480, 6, 0);
    Open__10VIUIWindowb(w3, false);
    SetTitle__10VIUIWindowPCUs(w3, title_of(767));
    *reinterpret_cast<int*>(bytes + 1168) = 0;
    memset(bytes + 1152, 0, 16);
}
}
