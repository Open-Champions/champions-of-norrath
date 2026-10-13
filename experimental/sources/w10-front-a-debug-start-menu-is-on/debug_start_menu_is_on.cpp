// Observed: debugStartMenuIsOn__Fv at 0x0017ff10 is 8 bytes. It returns the signed byte
// startMenuState (0x004841c8, one byte) sign-extended into an int, with no other effect.
// Inferred: a nonzero state means the debug start menu is showing, from the function name.
// Unknown: the values the byte takes.

extern "C" signed char startMenuState;

extern "C" int debugStartMenuIsOn__Fv() {
    return startMenuState;
}
