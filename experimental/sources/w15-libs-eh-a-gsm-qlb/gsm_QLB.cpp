// Observed gsm_QLB at 0x00448090: 4 doublewords (signed 64-bit values) in initialised .data, pinned SLUS_205.65. Read by Gsm_Long_Term_Synthesis_Filtering as the gain table indexed by the transmitted gain code.
// Values as read from the image: 3277, 11469, 21299, 32767.
// Unknown: nothing beyond the extent and values; the doubleword width matches the 64-bit "word" the other Gsm_ routines use.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
long long gsm_QLB[4] NORRATH_LOADED(".data") = {3277, 11469, 21299, 32767};
}
