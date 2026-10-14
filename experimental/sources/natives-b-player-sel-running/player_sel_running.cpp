// Observed amxPlayerSelRunning at 0x0013aaa8, 20 bytes. It tail-calls
// playerSelectionRunning and returns that result. The AMX cells are unread.
extern "C" int playerSelectionRunning__Fv();

extern "C" int amxPlayerSelRunning__FP5__amxPi(void*, int*) {
    return playerSelectionRunning__Fv();
}
