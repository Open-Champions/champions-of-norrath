// Observed onTouchSession__16VIRealmInterfaceUiiPv at 0x011092e0, 52 bytes. With (this, a, b, c): when b is not 1, the 64-bit value at
// this + 9728 is set to GetTime__7VITimer(); when b is 1 nothing happens. a and c are not read.
// Inferred: the 64-bit field is a "last session activity" time stamp (the constructor also seeds it from the same timer call) and
// b == 1 is a reply that must not refresh it.
// Unknown: what b's value 1 stands for (an error or "no session" code is a guess); the field's units.
extern "C" {
unsigned long long GetTime__7VITimer();

void onTouchSession__16VIRealmInterfaceUiiPv(void* self, unsigned a, int b, void* c) {
    (void)a;
    (void)c;
    if (b != 1) {
        unsigned long long now = GetTime__7VITimer();
        *reinterpret_cast<unsigned long long*>(static_cast<unsigned char*>(self) + 9728) = now;
    }
}
}
