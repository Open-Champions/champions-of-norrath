extern "C" int ustrlen__FPUs(unsigned short* s);

int main() {
    // Independent expectation: the index of the first zero unit in a buffer
    // whose zero position is chosen by the test.
    for (int len = 0; len < 40; len++) {
        unsigned short buf[48];
        for (int i = 0; i < 48; i++) buf[i] = (unsigned short)(0x8000u + i * 257u + 1u);
        buf[len] = 0;
        // Bytes past the terminator may hold anything, including zero low bytes.
        buf[len + 1] = 0x0100;
        if (ustrlen__FPUs(buf) != len) return 1;
    }
    // A unit whose low byte is zero but high byte is not still counts.
    unsigned short wide[3] = {0x4100, 0x0041, 0};
    if (ustrlen__FPUs(wide) != 2) return 2;
    return 0;
}
