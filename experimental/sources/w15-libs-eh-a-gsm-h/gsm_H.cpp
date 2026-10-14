// Observed gsm_H at 0x00448038: 11 doublewords (signed 64-bit values) in initialised .data, pinned SLUS_205.65. Eleven symmetric weights around 8192; the long-term-predictor weighting filter is the expected reader (not yet authored).
// Values as read from the image: -134, -374, 0, 2054, 5741, 8192, 5741, 2054, 0, -374, -134.
// Unknown: nothing beyond the extent and values; the doubleword width matches the 64-bit "word" the other Gsm_ routines use.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
long long gsm_H[11] NORRATH_LOADED(".data") = {-134, -374, 0, 2054, 5741, 8192, 5741, 2054, 0, -374, -134};
}
