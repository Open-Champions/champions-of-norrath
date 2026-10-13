// Observed: OnMessage__10VIWndMcErrP5VIWndiii at 0x0109c6d8 is 260 bytes. With (this, from, message,
// param1, param2) it first calls scePrintf with "VIWndMcErr::OnMessage (From = %d, MessageId = %d,
// Param1 = %d, Param2 = %d)\n" and (from, message, param1, param2). If message is not 295 it calls
// scePrintf("VIWndMcErr::OnMessage Default\n") and returns. Otherwise it calls
// scePrintf("VIWndMcErr::OnMessage VIWND_CHANGEFOCUS\n") and then:
//  - if param2 equals this: scePrintf("... VIWND_CHANGEFOCUS Gained new focus\n"), then with
//    the embedded UI window w = this+40: Close__10VIUIWindowb(w, 1), then
//    Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(w, mUI, &{0, 0}, &{320, 246}, 380, 7, 0) where mUI is
//    the word _5VIWnd$mUI (0x01192c70), and Open__10VIUIWindowb(w, 0);
//  - then, if param1 equals this: scePrintf("... VIWND_CHANGEFOCUS Lost focus\n").
// Nothing is returned. Message 295 is the number SetFocus sends; the game's own text names it
// VIWND_CHANGEFOCUS.
// Inferred: param1 is the window losing focus and param2 the window gaining it (from the two
// messages); the two stack pairs are 2D integer vectors (position and size) because the callee's
// name takes two VIVect2I references.
// Unknown: the meaning of the last three integers (380, 7, 0) and the return type.
extern "C" {
int scePrintf(const char* format, ...);
void Close__10VIUIWindowb(void* window, bool flag);
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, unsigned ui, const int* position,
                                              const int* size, int a, int b, int c);
void Open__10VIUIWindowb(void* window, bool flag);
extern unsigned _5VIWnd$mUI;

static inline unsigned word_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

void OnMessage__10VIWndMcErrP5VIWndiii(void* self, void* from, int message, void* param1, void* param2) {
    scePrintf("VIWndMcErr::OnMessage (From = %d, MessageId = %d, Param1 = %d, Param2 = %d)\n", word_of(from),
              message, word_of(param1), word_of(param2));
    if (message != 295) {
        scePrintf("VIWndMcErr::OnMessage Default\n");
        return;
    }
    scePrintf("VIWndMcErr::OnMessage VIWND_CHANGEFOCUS\n");
    if (param2 == self) {
        scePrintf("VIWndMcErr::OnMessage VIWND_CHANGEFOCUS Gained new focus\n");
        void* window = static_cast<unsigned char*>(self) + 40;
        Close__10VIUIWindowb(window, true);
        const int position[2] = {0, 0};
        const int size[2] = {320, 246};
        Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, _5VIWnd$mUI, position, size, 380, 7, 0);
        Open__10VIUIWindowb(window, false);
    }
    if (param1 == self) scePrintf("VIWndMcErr::OnMessage VIWND_CHANGEFOCUS Lost focus\n");
}
}
