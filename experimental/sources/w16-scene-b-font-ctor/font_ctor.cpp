// Observed: __7VIRFont at 0x01135dc0 is 36 bytes. It stores 0 at +84, +4 and +0 of the
// object, then tail-calls Clear__7VIRFont with the same object; v0 is therefore
// whatever Clear__7VIRFont returns (the original moves the pointer into v0 first, but
// the tail call overwrites it).
// Unknown: what the three stored words are; Clear__7VIRFont is a separate function.
extern "C" {
int Clear__7VIRFont(void* self);

int __7VIRFont(unsigned char* self) {
    *reinterpret_cast<int*>(self + 84) = 0;
    *reinterpret_cast<int*>(self + 4) = 0;
    *reinterpret_cast<int*>(self) = 0;
    return Clear__7VIRFont(self);
}
}
