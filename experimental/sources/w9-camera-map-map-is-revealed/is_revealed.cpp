// Observed isRevealed__3mapv at 0x0015e138, 8 bytes.
// Returns the word _3map$revealed (0x004868bc). Nothing is stored. Nothing here
// was executed on the EE.
extern "C" int _3map$revealed;

extern "C" int isRevealed__3mapv() {
    return _3map$revealed;
}
