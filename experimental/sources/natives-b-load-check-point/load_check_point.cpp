// Observed amxLoadCheckPoint at 0x0013aa08, 32 bytes. It calls
// gameDeathStart with zero and returns zero. The AMX cells are unread.
extern "C" int gameDeathStart__Fi(int value);

extern "C" int amxLoadCheckPoint__FP5__amxPi(void*, int*) {
    (void)gameDeathStart__Fi(0);
    return 0;
}
