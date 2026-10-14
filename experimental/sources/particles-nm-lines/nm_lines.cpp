// nmLines at 0x004869fc, four .sbss bytes, next linePos.
// The image value is zero. P_Init stores zero here later.
extern "C" {
unsigned nmLines;
}
static_assert(sizeof nmLines == 4u, "nmLines width");
