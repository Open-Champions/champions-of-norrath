// Observed readyToSwitch__3Cat at 0x001eef30, 16 bytes. It reads the signed
// byte at offset 292 of the Cat object and returns 1 when that byte is
// exactly 1, otherwise 0. What the byte means is not established.
extern "C" int readyToSwitch__3Cat(const void* self) {
    const signed char value = static_cast<const signed char*>(self)[292];
    return value == 1;
}
