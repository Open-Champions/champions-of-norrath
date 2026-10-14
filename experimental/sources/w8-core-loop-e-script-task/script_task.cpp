// Observed scriptTask__Fv at 0x001d3db0, 148 bytes.
// For each index below nmDeferedCalls (read again after every call), it
// calls scriptStart__FPc with the record at deferedCall + (scriptFrame & 1)
// * 5120 + index * 80, so the table is two banks of 64 records of 80 bytes
// with the bank chosen by the low bit of scriptFrame. The result of
// scriptStart is ignored. Afterwards scriptFrame is incremented,
// nmDeferedCalls takes nextNmDeferedCalls, and nextNmDeferedCalls becomes
// zero. The final scriptFrame is read after the loop, so a callee that
// changes it is observed. The record contents and the bank count of 64 are
// inferred from the strides only.
extern "C" {
extern int nmDeferedCalls;
extern int nextNmDeferedCalls;
extern int scriptFrame;
extern unsigned char deferedCall[];
int scriptStart__FPc(char* name);

void scriptTask__Fv() {
    for (int index = 0; index < nmDeferedCalls; ++index) {
        char* const record = reinterpret_cast<char*>(
            deferedCall + (scriptFrame & 1) * 5120 + index * 80);
        scriptStart__FPc(record);
    }
    const int next_frame = scriptFrame + 1;
    const int pending = nextNmDeferedCalls;
    nextNmDeferedCalls = 0;
    nmDeferedCalls = pending;
    scriptFrame = next_frame;
}
}
