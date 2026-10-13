// Observed gameMinCamHeight at 0x00484cc8 and gameMaxCamHeight at 0x00484ccc: two
// adjacent file-backed floats in .sdata (4 bytes each). The image bytes are
// 0x00 0x50 0xc3 0xc7 (-100000.0f) and 0x00 0x50 0xc3 0x47 (+100000.0f). Authored
// game code (w9-frontend game load) stores the two values from a level record; why the
// image holds these two values before any level is loaded is not established here.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
// Defined in this order because the EE compiler emits file-scope data in reverse, which
// puts gameMinCamHeight at the lower address as in the image.
extern "C" {
float gameMaxCamHeight NORRATH_LOADED(".sdata") = 100000.0f;
float gameMinCamHeight NORRATH_LOADED(".sdata") = -100000.0f;
}
