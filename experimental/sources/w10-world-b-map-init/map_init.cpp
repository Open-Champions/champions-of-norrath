// Observed init__3mapi at 0x0015e068, 12 bytes in SLUS_205.65. The argument is not read. The
// body stores zero to the two gp-relative words _3map$iconListCount (0x004868b8) and
// _3map$revealed (0x004868bc) and returns nothing.
// Inferred: the integer parameter is unused; map-icon list reset. Unknown: what callers pass,
// and the meaning of _3map$revealed beyond being a word cleared together with the count.
extern int _3map$iconListCount;
extern int _3map$revealed;

extern "C" void init__3mapi(int) {
    _3map$iconListCount = 0;
    _3map$revealed = 0;
}
