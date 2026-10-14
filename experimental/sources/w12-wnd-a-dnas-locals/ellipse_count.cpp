// Observed: ellipseCount.2103 at 0x01192aec is a local four-byte word of the image that starts at zero.
// OnDrawSelf__9VIWndDnas adds one to it on every call and, in one state, resets it when it passes 120.
// The name is the image's own local symbol name.
extern "C" {
int norrath_dnas_ellipse_count __asm__("ellipseCount.2103") = 0;
}
