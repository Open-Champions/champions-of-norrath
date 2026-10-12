// Observed assertion message text at 0x004635e8 in read-only data: 48
// characters plus the terminator. Zero bytes follow it up to the next string.
// It is the format that __assert__FPCcUiT0i passes to scePrintf: condition
// text (%s), file (%s), line (%d), the context pointer (%s) and the caller
// word (%x), ending with a newline.
extern "C" const char norrath_assert_format_view[] = "Assertion \"%s\" failed in %s:%d  context:%s (%x)\n";
static_assert(sizeof(norrath_assert_format_view) == 49, "observed text length plus terminator");
