// gameDeathStart at 0x001d0dc8, 64 bytes, in SLUS_205.65.
// Observed: stores the low byte of the argument in the byte deathNetHack (0x00486afd), clears
// the byte deathState (0x00486afc), calls hudResetFadeScreen(0), then registers the function
// gameDeath (0x001cd7c0) with engineAddTask at priority 20, name text "deathTask", flag 0.
// The byte store of deathState happens before the hudResetFadeScreen call and the byte store of
// deathNetHack before that. Nothing is returned.
// Unknown: what values callers pass, and how the two bytes are consumed.
extern "C" unsigned char deathNetHack;
extern "C" unsigned char deathState;
extern "C" void hudResetFadeScreen__Fi(int value);
extern "C" void gameDeath__Fv();
extern "C" void engineAddTask__FPFv_viPci(void (*callback)(), int priority, char* name, int flag);

extern "C" void gameDeathStart__Fi(int value) {
    deathNetHack = static_cast<unsigned char>(value);
    deathState = 0;
    hudResetFadeScreen__Fi(0);
    engineAddTask__FPFv_viPci(gameDeath__Fv, 20, const_cast<char*>("deathTask"), 0);
}
