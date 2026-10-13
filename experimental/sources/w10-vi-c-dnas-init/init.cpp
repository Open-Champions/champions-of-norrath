// Observed: Init__9VIWndDnasP7VIScene at 0x010a8510 is 84 bytes. With (this, scene) it stores
// scene at this+776, calls Init__12VIWndMessage(this+1176) (the scene is still in the second
// argument register at that point but nothing here sets it up as an argument), stores 0 at
// (this+1176)+28, calls AddChild__5VIWndP5VIWnd(this, this+1176), stores -1 at this+780, and
// returns 0. The result of the message-window Init is ignored.
// Unknown: what words 776, 780 and +28 of the child mean; whether Init__12VIWndMessage reads
// its second argument (unresolved here; a private paired run only holds if it does not).
extern "C" {
void Init__12VIWndMessage(void* self);
void AddChild__5VIWndP5VIWnd(void* self, void* child);

int Init__9VIWndDnasP7VIScene(void* self, void* scene) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned char* message = bytes + 1176;
    *reinterpret_cast<unsigned*>(bytes + 776) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(scene));
    Init__12VIWndMessage(message);
    *reinterpret_cast<unsigned*>(message + 28) = 0;
    AddChild__5VIWndP5VIWnd(self, message);
    *reinterpret_cast<int*>(bytes + 780) = -1;
    return 0;
}
}
