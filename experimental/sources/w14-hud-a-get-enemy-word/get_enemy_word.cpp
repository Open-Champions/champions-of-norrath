// Observed getEnemyWord__Fi at 0x0015e908, 60 bytes. It tail-calls
// textGetString with one of two short keys: "enemy" when the argument is
// exactly 1, "enemies" for every other value. Whether the key names a
// localized word is inferred from the callee's name only.
extern "C" const char* textGetString__FPc(const char* key);

extern "C" const char* getEnemyWord__Fi(int count) {
    return textGetString__FPc(count == 1 ? "enemy" : "enemies");
}
