// Observed: T__10VIUIWindowPCc at 0x0103f270 is 56 bytes. With (this, text) it calls
// str8to16__8VIStringPUsUiPCc(sConvert16, 2048, text) and returns the address of sConvert16 (0x011e5418).
// The object pointer is not read.
// Inferred: sConvert16 is a shared 16-bit conversion buffer (2048 is passed as its size, in bytes by the
// call's third-argument shape; that unit is a guess).
// Unknown: the element type of sConvert16 beyond "16-bit", and who else writes it.
extern "C" {
void str8to16__8VIStringPUsUiPCc(unsigned short* dest, unsigned size, const char* text);
extern unsigned short sConvert16[];

const unsigned short* T__10VIUIWindowPCc(void*, const char* text) {
    str8to16__8VIStringPUsUiPCc(sConvert16, 2048, text);
    return sConvert16;
}
}
