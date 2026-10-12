// Observed layer0Dir at 0x006cc6a0, 24580 bytes. Every file byte is zero, so the definition is
// zero-initialized storage. The cd-archive package describes it as 768 rows of 32 bytes plus a count word.
extern "C" {
unsigned char layer0Dir[24580] = {};
}
