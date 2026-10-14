// Observed: Init__11VIWndSplashP7VIScene at 0x010d5e10 is 196 bytes. With (this, scene) it
// stores scene at this+36, 0 at this+408 and 0 at this+9664, calls Init__12VIWndConnect(
// this+416), stores 0 at (this+416)+28 and calls AddChild__5VIWndP5VIWnd(this, this+416);
// then Init__12VIWndMessage(this+4776), 0 at (this+4776)+28, AddChild(this, this+4776); then
// Init__12VIWndOptionsP7VIScene(this+2408, scene), 0 at (this+2408)+28, AddChild(this,
// this+2408); then stores 0 at this+9644, 9660, 9640, 9656, 9648 and 9652, and returns 0. The
// results of the Init calls are ignored.
// Observed: the scene stays in the second argument register across the Init__12VIWndConnect
// and Init__12VIWndMessage calls, but neither of those routines (0x010a2b20, 0x010c1470) reads
// it, so both are declared with one argument.
// Unknown: what words 408, 9640 to 9664 and the +28 words of the children mean.
extern "C" {
void Init__12VIWndConnect(void* self);
void Init__12VIWndMessage(void* self);
void Init__12VIWndOptionsP7VIScene(void* self, void* scene);
void AddChild__5VIWndP5VIWnd(void* self, void* child);

int Init__11VIWndSplashP7VIScene(void* self, void* scene) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned char* connect = bytes + 416;
    unsigned char* message = bytes + 4776;
    unsigned char* options = bytes + 2408;
    *reinterpret_cast<unsigned*>(bytes + 36) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(scene));
    *reinterpret_cast<unsigned*>(bytes + 408) = 0;
    *reinterpret_cast<unsigned*>(bytes + 9664) = 0;
    Init__12VIWndConnect(connect);
    *reinterpret_cast<unsigned*>(connect + 28) = 0;
    AddChild__5VIWndP5VIWnd(self, connect);
    Init__12VIWndMessage(message);
    *reinterpret_cast<unsigned*>(message + 28) = 0;
    AddChild__5VIWndP5VIWnd(self, message);
    Init__12VIWndOptionsP7VIScene(options, scene);
    *reinterpret_cast<unsigned*>(options + 28) = 0;
    AddChild__5VIWndP5VIWnd(self, options);
    *reinterpret_cast<unsigned*>(bytes + 9644) = 0;
    *reinterpret_cast<unsigned*>(bytes + 9660) = 0;
    *reinterpret_cast<unsigned*>(bytes + 9640) = 0;
    *reinterpret_cast<unsigned*>(bytes + 9656) = 0;
    *reinterpret_cast<unsigned*>(bytes + 9648) = 0;
    *reinterpret_cast<unsigned*>(bytes + 9652) = 0;
    return 0;
}
}
