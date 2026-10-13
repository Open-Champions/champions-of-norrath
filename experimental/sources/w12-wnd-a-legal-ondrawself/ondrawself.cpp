// Observed: OnDrawSelf__10VIWndLegal at 0x010bf8a8 is 52 bytes. It calls GetTime__7VITimer() and
// discards the result, then calls SetFocus__5VIWndP5VIWndb(this, 0, true). Returns nothing.
// Inferred: drawing this window hands the input focus back to the window manager's default.
// Unknown: why the time is read and dropped (it may be a leftover of removed code).
extern "C" {
long long GetTime__7VITimer();
void SetFocus__5VIWndP5VIWndb(void* self, void* window, bool force);

void OnDrawSelf__10VIWndLegal(void* self) {
    GetTime__7VITimer();
    SetFocus__5VIWndP5VIWndb(self, 0, true);
}
}
