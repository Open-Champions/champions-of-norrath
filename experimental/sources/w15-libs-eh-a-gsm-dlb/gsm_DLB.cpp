// Observed gsm_DLB at 0x004480b0: 4 doublewords (signed 64-bit values) in initialised .data, pinned SLUS_205.65. Four increasing values; the long-term predictor's gain decision is the expected reader (not yet authored).
// Values as read from the image: 6554, 16384, 26214, 32767.
// Unknown: nothing beyond the extent and values; the doubleword width matches the 64-bit "word" the other Gsm_ routines use.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
long long gsm_DLB[4] NORRATH_LOADED(".data") = {6554, 16384, 26214, 32767};
}
