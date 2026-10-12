// Observed gameMiscTaskSync__Fv at 0x001d0b60, 36 bytes. It calls
// playerRecallSpell__Fv (0x001a4e78) and then playerCheckRecallHotKey__Fv
// (0x001ab1e8) with no arguments and returns. Both are other packages' symbols.
extern "C" void playerRecallSpell__Fv();
extern "C" void playerCheckRecallHotKey__Fv();

extern "C" void gameMiscTaskSync__Fv() {
    playerRecallSpell__Fv();
    playerCheckRecallHotKey__Fv();
}
