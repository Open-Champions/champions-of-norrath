// Observed: OnMessage__9VIWndEulaP5VIWndiii at 0x010b5380 is 200 bytes. With (this, from, message, a, b):
// unless message is 295 and b equals this it returns at once (it does not call the base handler).
// Otherwise, with w = this + 40 and ui = the word _5VIWnd$mUI (0x01192c70):
//   Close(w, true);
//   Init(w, ui, position {0,0}, size {320,224}, 600, 18, 0);
//   SetTitle(w, GetString(ui, 1596, *(int*)(ui + 48)));
//   Open(w, false);
//   store zero at this + 9764 and at this + 1544.
// Nothing is returned. 295 is the message SetFocus sends; b is the window gaining focus.
// Inferred: the two stack pairs are 2D integer vectors (position, size) because the Init name takes
// two VIVect2I references.
// Unknown: the meaning of the last three Init integers, of string number 1596 and of the word at
// ui + 48; what the two cleared words hold.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, unsigned ui, const int* position,
                                              const int* size, int a, int b, int c);
void Open__10VIUIWindowb(void* window, bool flag);
void Close__10VIUIWindowb(void* window, bool flag);
void SetTitle__10VIUIWindowPCUs(void* window, const unsigned short* title);
const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg);
extern unsigned _5VIWnd$mUI;

void OnMessage__9VIWndEulaP5VIWndiii(void* self, void*, int message, void*, void* b) {
    if (message != 295 || b != self) return;
    unsigned char* bytes = static_cast<unsigned char*>(self);
    void* window = bytes + 40;
    Close__10VIUIWindowb(window, true);
    const int position[2] = {0, 0};
    const int size[2] = {320, 224};
    Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, _5VIWnd$mUI, position, size, 600, 18, 0);
    char* table = NORRATH_TRANSLATE(_5VIWnd$mUI);
    SetTitle__10VIUIWindowPCUs(window, GetString__13VIStringTableii(table, 1596, *reinterpret_cast<int*>(table + 48)));
    Open__10VIUIWindowb(window, false);
    *reinterpret_cast<int*>(bytes + 9764) = 0;
    *reinterpret_cast<int*>(bytes + 1544) = 0;
}
}
