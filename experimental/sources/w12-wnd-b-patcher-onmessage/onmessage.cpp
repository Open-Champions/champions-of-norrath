// Observed: OnMessage__12VIWndPatcherP5VIWndiii at 0x010cc5e8 is 652 bytes. With (this, from, message,
// a, b), let ui be the word _5VIWnd$mUI (0x01192c70) and msgwnd = this + 2312:
//  - message 295 (the message SetFocus sends; b is the window gaining focus): nothing happens unless
//    b equals this. If a is neither msgwnd nor this + 1176, three embedded UI windows are set up:
//      w1 = this + 40:  Init(w1, ui, pos {0,350}, size {304,360}, 440, 1, 0); Open(w1, true);
//      w2 = this + 776: Close(w2, true); Init(w2, ui, {0,0}, {320,246}, 430, 4, 0); Open(w2, false);
//                       SetTitle(w2, GetString(ui, 811, *(int*)(ui + 48)));
//      w3 = this + 408: Init(w3, ui, {0,0}, {320,246}, 480, 6, 0); Open(w3, false);
//                       SetTitle(w3, GetString(ui, 767, *(int*)(ui + 48)));
//    then Clear__16VIWndReadMessage(msgwnd), words at +1144, +1152, +1156, +1164, +1168 become 0
//    and the word at +1160 becomes -1. In every other case with b equal to this: when a equals
//    msgwnd and the word at +1156 is 1 and the word at +1168 is zero, +1168 and +1164 become 1.
//  - message 307: reading the word at +1160 as state: 0 gives SetFocus(this, this, true); 1 gives
//    SetFocus(this, this, true) when the word at +2160 is 1, and otherwise stores -1 at +1148 and
//    gives SetFocus(this, nullptr, true); any other state does nothing.
//  - any other message is passed unchanged to OnMessage__5VIWndP5VIWndiii.
// Nothing is returned.
// Inferred: the two stack pairs are 2D integer vectors (position, size) because the Init name takes
// two VIVect2I references; +1160 is a small state machine.
// Unknown: the meaning of message 307, of the state values, of the Init integers, of strings 811
// and 767, and of the word at ui + 48.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
void OnMessage__5VIWndP5VIWndiii(void* self, void* from, int message, void* a, void* b);
void SetFocus__5VIWndP5VIWndb(void* self, void* window, bool force);
void Clear__16VIWndReadMessage(void* message_window);
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, unsigned ui, const int* position,
                                              const int* size, int a, int b, int c);
void Open__10VIUIWindowb(void* window, bool flag);
void Close__10VIUIWindowb(void* window, bool flag);
void SetTitle__10VIUIWindowPCUs(void* window, const unsigned short* title);
const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg);
extern unsigned _5VIWnd$mUI;

static inline const unsigned short* title_of(int id) {
    char* table = NORRATH_TRANSLATE(_5VIWnd$mUI);
    return GetString__13VIStringTableii(table, id, *reinterpret_cast<int*>(table + 48));
}

void OnMessage__12VIWndPatcherP5VIWndiii(void* self, void* from, int message, void* a, void* b) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    int* state = reinterpret_cast<int*>(bytes + 1160);
    if (message == 295) {
        if (b != self) return;
        void* message_window = bytes + 2312;
        if (a != message_window && a != bytes + 1176) {
            void* w1 = bytes + 40;
            void* w2 = bytes + 776;
            void* w3 = bytes + 408;
            const int pos1[2] = {0, 350};
            const int size1[2] = {304, 360};
            Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(w1, _5VIWnd$mUI, pos1, size1, 440, 1, 0);
            Open__10VIUIWindowb(w1, true);
            Close__10VIUIWindowb(w2, true);
            const int pos2[2] = {0, 0};
            const int size2[2] = {320, 246};
            Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(w2, _5VIWnd$mUI, pos2, size2, 430, 4, 0);
            Open__10VIUIWindowb(w2, false);
            SetTitle__10VIUIWindowPCUs(w2, title_of(811));
            Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(w3, _5VIWnd$mUI, pos2, size2, 480, 6, 0);
            Open__10VIUIWindowb(w3, false);
            SetTitle__10VIUIWindowPCUs(w3, title_of(767));
            Clear__16VIWndReadMessage(message_window);
            *reinterpret_cast<int*>(bytes + 1144) = 0;
            *reinterpret_cast<int*>(bytes + 1152) = 0;
            *state = -1;
            *reinterpret_cast<int*>(bytes + 1156) = 0;
            *reinterpret_cast<int*>(bytes + 1164) = 0;
            *reinterpret_cast<int*>(bytes + 1168) = 0;
            return;
        }
        if (a != message_window) return;
        int pending = *reinterpret_cast<int*>(bytes + 1156);
        if (pending != 1) return;
        if (*reinterpret_cast<int*>(bytes + 1168) == 0) {
            *reinterpret_cast<int*>(bytes + 1168) = pending;
            *reinterpret_cast<int*>(bytes + 1164) = pending;
        }
        return;
    }
    if (message == 307) {
        int current = *state;
        if (current == 0) {
            SetFocus__5VIWndP5VIWndb(self, self, true);
        } else if (current == 1) {
            if (*reinterpret_cast<int*>(bytes + 2160) == 1) {
                SetFocus__5VIWndP5VIWndb(self, self, true);
            } else {
                *reinterpret_cast<int*>(bytes + 1148) = -1;
                SetFocus__5VIWndP5VIWndb(self, nullptr, true);
            }
        }
        return;
    }
    OnMessage__5VIWndP5VIWndiii(self, from, message, a, b);
}
}
