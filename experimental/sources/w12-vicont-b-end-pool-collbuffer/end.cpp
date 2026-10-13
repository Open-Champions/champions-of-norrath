// Observed: End__Ct6VIPool1ZP12VICollBuffer at 0x01207320, 12 bytes. It ignores its argument and returns 0x7fffffff.
// Inferred: 0x7fffffff is the end marker of the pool's iteration, as in the other pool instances.
extern "C" {
int End__Ct6VIPool1ZP12VICollBuffer(const void*) { return 0x7fffffff; }
}
