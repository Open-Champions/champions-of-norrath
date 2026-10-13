// Observed: GetTime__7VIScene at 0x01144a08 is 8 bytes. It returns the 64-bit value stored at
// byte offset 1152 of the scene object (a single `ld`). Inferred: a 64-bit time or tick counter,
// from the method name only. Unknown: units and the member that writes it.
extern "C" {
long long GetTime__7VIScene(const unsigned char* self) {
    return *reinterpret_cast<const long long*>(self + 1152);
}
}
