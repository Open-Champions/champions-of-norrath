// Observed: VIHashResourceID at 0x0112cbf8 is 76 bytes. Starting from zero, each
// byte of the zero-terminated string is read as a signed char and the running value
// becomes value * 131 + byte (the original forms it as ((value * 33) * 4) - value).
// An empty string gives zero. The value is returned in v0 as a 32-bit word.
extern "C" {
unsigned VIHashResourceID(const char* text) {
    unsigned value = 0;
    while (*text != 0) {
        value = value * 131u + static_cast<unsigned>(static_cast<int>(*reinterpret_cast<const signed char*>(text)));
        ++text;
    }
    return value;
}
}
