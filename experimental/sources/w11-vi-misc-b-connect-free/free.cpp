// Observed: Free__12VIWndConnect at 0x010a2b88 is 48 bytes. With the object as this it calls
// RemoveChild__5VIWndP5VIWnd(this, this+40) and then Free__12VIWndMessage(this+40). Nothing is
// returned.
extern "C" {
void RemoveChild__5VIWndP5VIWnd(void* self, void* child);
void Free__12VIWndMessage(void* self);

void Free__12VIWndConnect(void* self) {
    unsigned char* message = static_cast<unsigned char*>(self) + 40;
    RemoveChild__5VIWndP5VIWnd(self, message);
    Free__12VIWndMessage(message);
}
}
