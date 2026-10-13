// Observed: Free__9VIWndDnas at 0x010a8568 is 48 bytes. With the object as this it calls
// RemoveChild__5VIWndP5VIWnd(this, this+1176) and then Free__12VIWndMessage(this+1176).
// Nothing is returned.
extern "C" {
void RemoveChild__5VIWndP5VIWnd(void* self, void* child);
void Free__12VIWndMessage(void* self);

void Free__9VIWndDnas(void* self) {
    void* message = static_cast<unsigned char*>(self) + 1176;
    RemoveChild__5VIWndP5VIWnd(self, message);
    Free__12VIWndMessage(message);
}
}
