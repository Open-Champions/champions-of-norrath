// Observed _shopCheck__Fi at 0x001bbd60, 8 bytes: it returns at once. It takes
// one int argument and neither reads it nor touches any memory.
// Unknown: why the image has an empty function under this name.
extern "C" void _shopCheck__Fi(int) {}
