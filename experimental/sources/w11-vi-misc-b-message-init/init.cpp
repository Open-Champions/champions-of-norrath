// Observed: Init__12VIWndMessage at 0x010c1470 is 12 bytes. With the object as this it stores 0
// at this+984 and returns 0.
// Unknown: what the word at +984 holds.
extern "C" {
int Init__12VIWndMessage(void* self) {
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 984) = 0;
    return 0;
}
}
