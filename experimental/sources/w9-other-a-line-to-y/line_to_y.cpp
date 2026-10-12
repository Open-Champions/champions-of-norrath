// Observed lineToY__Fi at 0x001d3990, 20 bytes. Returns argument * 18 + 295 as a
// 32-bit integer. The meaning of 18 and 295 (a line pitch and a first-line
// offset on screen) is inferred from the name only; no layout is claimed.
extern "C" int lineToY__Fi(int line) {
    return line * 18 + 295;
}
