// Observed lineToY__Fi at 0x1d3990, extent 20. Returns 18 * line + 295 with
// ordinary 32-bit wraparound. Unknown: what the screen rows mean; the callers
// are in the pre-logo dialog drawing.
extern "C" int lineToY__Fi(int line) {
    return line * 18 + 295;
}
