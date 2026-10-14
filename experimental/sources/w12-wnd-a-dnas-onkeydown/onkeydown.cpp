// Observed: OnKeyDown__9VIWndDnasi at 0x010a8598 is 56 bytes. With (this, key), when key is 13 or
// 134 it calls SetFocus__5VIWndP5VIWndb(this, 0, true); otherwise it does nothing.
// Inferred: 13 and 134 are the confirm key codes (13 is a carriage return).
// Unknown: what key 134 is.
extern "C" {
void SetFocus__5VIWndP5VIWndb(void* self, void* window, bool force);

void OnKeyDown__9VIWndDnasi(void* self, int key) {
    if (key == 13 || key == 134) SetFocus__5VIWndP5VIWndb(self, 0, true);
}
}
