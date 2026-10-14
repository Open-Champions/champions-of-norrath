// fxClear__Fv at 0x001e63d8, 56 bytes, read from the pinned SLUS-20565 image.
// Observed: it makes one call, fxUpdateDust__Fiiiiiiii (0x001e6410), and returns
// nothing of its own. The eight integer arguments are 92, 90, 64, 34, 92, 85,
// 64 and 0, in that order (the eighth is the zero register). Fresh compilation
// is not an original-byte match.
extern "C" {
void fxUpdateDust__Fiiiiiiii(int, int, int, int, int, int, int, int);

void fxClear__Fv() {
    fxUpdateDust__Fiiiiiiii(92, 90, 64, 34, 92, 85, 64, 0);
}
}
