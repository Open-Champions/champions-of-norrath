// nextIndex at 0x486b38, a 4-byte .bss word (the next symbol is 4 bytes later).
// Observed users: the pre-logo dialog draw and message routines read and store it
// as an int. Unknown: its meaning beyond that.
extern "C" int nextIndex;
int nextIndex;
