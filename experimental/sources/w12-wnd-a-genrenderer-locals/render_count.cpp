// Observed: renderCount.3634 at 0x01192c5c is a local four-byte word of the image that starts at zero.
// Only OnDrawSelf__20VIWndGenericRenderer is known to use it: it counts page renders up to 1000.
// The symbol name is the image's own local name; the unit exists so that unit can import it.
extern "C" {
int norrath_generic_renderer_render_count __asm__("renderCount.3634") = 0;
}
