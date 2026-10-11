// Observed: newMode stores its first argument in asyncMode. When the second
// argument is zero it first copies the previous asyncMode into oldAsyncMode,
// and the return register is that previous value. It always stores zero in
// syncState. Fourteen direct calls pass a zero second argument.
// forceGoodCard forwards its own argument. On that nonzero path the original
// does not write the return register.
// Inferred: none.
// Unknown: what the mode numbers mean. syncState belongs to another object.

extern "C" int asyncMode;
extern "C" int oldAsyncMode;
extern "C" int syncState;

extern "C" int newMode__Fii(int mode, int argument) {
    int previous = asyncMode;
    if (argument == 0) {
        oldAsyncMode = previous;
    }
    asyncMode = mode;
    syncState = 0;
    return previous;
}
