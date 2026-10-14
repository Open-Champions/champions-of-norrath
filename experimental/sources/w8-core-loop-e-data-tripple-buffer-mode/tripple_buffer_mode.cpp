// Observed trippleBufferMode (the name's spelling is the image's) at
// 0x004843e0, four bytes in .sdata. The file word is 1. Accessed with two word
// loads and one word store. Its meaning beyond the name is unknown.
extern "C" {
int trippleBufferMode = 1;
}
