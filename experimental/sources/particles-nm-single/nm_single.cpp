// nmSingle at 0x004869f8, four .sbss bytes, next nmLines.
// The image value is zero. P_Init stores zero here later.
extern "C" {
unsigned nmSingle;
}
static_assert(sizeof nmSingle == 4u, "nmSingle width");
