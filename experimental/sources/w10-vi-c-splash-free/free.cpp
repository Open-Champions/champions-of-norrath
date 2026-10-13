// Observed: Free__11VIWndSplash at 0x010d6f00 is 124 bytes. With the object as this it calls
// RemoveChild__5VIWndP5VIWnd(this, this+2408) and Free__12VIWndOptions(this+2408), then
// RemoveChild(this, this+4776) and Free__12VIWndMessage(this+4776), then RemoveChild(this,
// this+416) and Free__12VIWndConnect(this+416). Nothing is returned.
extern "C" {
void RemoveChild__5VIWndP5VIWnd(void* self, void* child);
void Free__12VIWndOptions(void* self);
void Free__12VIWndMessage(void* self);
void Free__12VIWndConnect(void* self);

void Free__11VIWndSplash(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    RemoveChild__5VIWndP5VIWnd(self, bytes + 2408);
    Free__12VIWndOptions(bytes + 2408);
    RemoveChild__5VIWndP5VIWnd(self, bytes + 4776);
    Free__12VIWndMessage(bytes + 4776);
    RemoveChild__5VIWndP5VIWnd(self, bytes + 416);
    Free__12VIWndConnect(bytes + 416);
}
}
