// Observed gsm_FAC at 0x00447fb8: 8 doublewords (signed 64-bit values) in initialised .data, pinned SLUS_205.65. Read by Gsm_RPE_Decoding as the mantissa factor table (indexed 0..7).
// Values as read from the image: 18431, 20479, 22527, 24575, 26623, 28671, 30719, 32767.
// Unknown: nothing beyond the extent and values; the doubleword width matches the 64-bit "word" the other Gsm_ routines use.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
long long gsm_FAC[8] NORRATH_LOADED(".data") = {18431, 20479, 22527, 24575, 26623, 28671, 30719, 32767};
}
