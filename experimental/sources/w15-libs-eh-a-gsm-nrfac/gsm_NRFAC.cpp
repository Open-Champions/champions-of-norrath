// Observed gsm_NRFAC at 0x00447ff8: 8 doublewords (signed 64-bit values) in initialised .data, pinned SLUS_205.65. Eight values that fall from about 0.89 to 0.5 of 32768; the encoder side is the expected reader (not yet authored).
// Values as read from the image: 29128, 26215, 23832, 21846, 20165, 18725, 17476, 16384.
// Unknown: nothing beyond the extent and values; the doubleword width matches the 64-bit "word" the other Gsm_ routines use.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
long long gsm_NRFAC[8] NORRATH_LOADED(".data") = {29128, 26215, 23832, 21846, 20165, 18725, 17476, 16384};
}
