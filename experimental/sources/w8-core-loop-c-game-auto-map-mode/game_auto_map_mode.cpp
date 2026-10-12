// gameAutoMapMode at 0x00484f30, 4 bytes, in SLUS_205.65.
// Observed: zero-initial word in .data, accessed with word loads and stores. Unknown: the mode values.
extern "C" {
int gameAutoMapMode = 0;
}
