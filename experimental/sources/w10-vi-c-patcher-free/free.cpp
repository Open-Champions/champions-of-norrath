// Observed: Free__12VIWndPatcher at 0x010ccc28 is 72 bytes. With the object as this it calls
// RemoveChild__5VIWndP5VIWnd(this, this+2312), RemoveChild__5VIWndP5VIWnd(this, this+1176),
// then Free__12VIWndMessage(this+1176). Nothing is returned. No call frees the read-message
// child at this+2312 beyond removing it.
extern "C" {
void RemoveChild__5VIWndP5VIWnd(void* self, void* child);
void Free__12VIWndMessage(void* self);

void Free__12VIWndPatcher(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    RemoveChild__5VIWndP5VIWnd(self, bytes + 2312);
    RemoveChild__5VIWndP5VIWnd(self, bytes + 1176);
    Free__12VIWndMessage(bytes + 1176);
}
}
