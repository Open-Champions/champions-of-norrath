// Observed: topSema at 0x008de0c0. The symbol size is 0; topArg starts 8 bytes
// later and nothing reads the word between them. The word is loaded as the
// semaphore number by topThread and by iWakeupThread. InitThread stores the
// CreateSema result in it. The image bytes are zero.
// Inferred: none.
// Unknown: none.
extern "C" {
int topSema = 0;
}
