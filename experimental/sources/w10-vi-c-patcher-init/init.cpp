// Observed: Init__12VIWndPatcher at 0x010ccbb0 is 116 bytes. With the object as this it stores 0
// at this+1144, calls Init__12VIWndMessage(this+1176), stores 0 at (this+1176)+28, calls
// AddChild__5VIWndP5VIWnd(this, this+1176), calls Init__16VIWndReadMessage(this+2312), stores 0
// at (this+2312)+28, calls AddChild__5VIWndP5VIWnd(this, this+2312), stores 0 at this+1148 and
// returns 0. The results of the two Init calls are ignored.
// Unknown: what words 1144 and 1148 and the +28 words of the children mean.
extern "C" {
void Init__12VIWndMessage(void* self);
void Init__16VIWndReadMessage(void* self);
void AddChild__5VIWndP5VIWnd(void* self, void* child);

int Init__12VIWndPatcher(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned char* message = bytes + 1176;
    unsigned char* read_message = bytes + 2312;
    *reinterpret_cast<unsigned*>(bytes + 1144) = 0;
    Init__12VIWndMessage(message);
    *reinterpret_cast<unsigned*>(message + 28) = 0;
    AddChild__5VIWndP5VIWnd(self, message);
    Init__16VIWndReadMessage(read_message);
    *reinterpret_cast<unsigned*>(read_message + 28) = 0;
    AddChild__5VIWndP5VIWnd(self, read_message);
    *reinterpret_cast<unsigned*>(bytes + 1148) = 0;
    return 0;
}
}
