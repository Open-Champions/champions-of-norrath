// Observed: Init__12VIWndConnect at 0x010a2b20 is 100 bytes. With the object as this it stores
// zero at this+1916, this+1920, this+1924, this+1912 and this+1956, calls
// Init__12VIWndMessage(this+40), stores 0 at this+68 (the +28 word of the embedded message
// window), calls AddChild__5VIWndP5VIWnd(this, this+40), stores 0 at this+1960 and this+1964 and
// returns 0. The result of Init__12VIWndMessage is not used.
// Unknown: what the words at +1912 to +1924 and +1956 to +1964 mean (the destructor treats +1912
// as a child pointer, and EndConnect clears +1920 and +1924).
extern "C" {
int Init__12VIWndMessage(void* self);
void AddChild__5VIWndP5VIWnd(void* self, void* child);

int Init__12VIWndConnect(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned char* message = bytes + 40;
    *reinterpret_cast<unsigned*>(bytes + 1916) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1920) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1924) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1912) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1956) = 0;
    Init__12VIWndMessage(message);
    *reinterpret_cast<unsigned*>(message + 28) = 0;
    AddChild__5VIWndP5VIWnd(self, message);
    *reinterpret_cast<unsigned*>(bytes + 1960) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1964) = 0;
    return 0;
}
}
