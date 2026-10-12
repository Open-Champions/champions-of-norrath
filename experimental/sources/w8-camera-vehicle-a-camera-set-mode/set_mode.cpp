// Observed SetMode__6Camerai at 0x0026fa88, 112 bytes.
// Stores the mode word at camera+52 last, on every path. Mode 1 first clears
// the words at camera+56, +60 and +152. Modes 3, 4 and 5 first call
// revealEntireMap__3mapv (no arguments) and set the word gameShowEnemiesInMap
// (0x00484f54) to 1. Any other mode only stores the mode. Nothing is returned.
// What the modes mean is not established here. Nothing here was executed on
// the EE.
extern "C" {
void revealEntireMap__3mapv();
extern int gameShowEnemiesInMap;
}

typedef int __attribute__((may_alias)) AliasedInt;

extern "C" void SetMode__6Camerai(unsigned char* camera, int mode) {
    if (mode == 1) {
        *reinterpret_cast<AliasedInt*>(camera + 56) = 0;
        *reinterpret_cast<AliasedInt*>(camera + 60) = 0;
        *reinterpret_cast<AliasedInt*>(camera + 152) = 0;
    } else if (static_cast<unsigned>(mode - 3) < 3u) {
        revealEntireMap__3mapv();
        gameShowEnemiesInMap = 1;
    }
    *reinterpret_cast<AliasedInt*>(camera + 52) = mode;
}
