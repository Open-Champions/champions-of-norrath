// Observed: dot.2110 (0x01192c8c) and dot_delay.2111 (0x01192c8d) are two local one-byte variables of the
// image, both zero at start. OnDrawSelf__12VIWndPatcher counts dot_delay down from 15 and steps dot through 0, 1, 2.
// The names are the image's own local symbol names.
extern "C" {
signed char norrath_patcher_dot __asm__("dot.2110") = 0;
unsigned char norrath_patcher_dot_delay __asm__("dot_delay.2111") = 0;
}
