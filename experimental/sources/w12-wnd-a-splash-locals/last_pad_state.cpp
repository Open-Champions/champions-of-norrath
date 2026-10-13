// Observed: lastPadState.2196 at 0x011afad8 is a local four-byte word of the image whose initial value is 1.
// OnDrawSelf__11VIWndSplash reads it and stores the word it reads from the station object (app+0xaeb0).
// The name is the image's own local symbol name.
extern "C" {
int norrath_splash_last_pad_state __asm__("lastPadState.2196") = 1;
}
