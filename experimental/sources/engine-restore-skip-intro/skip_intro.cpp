// Observed skipIntro at 0x004866c0. Four-byte local .sdata word.
// The file bytes are zero. The symbol is local in the pinned image; the
// import from main is the global name skipIntro.
extern "C" {
int skipIntro = 0;
}
