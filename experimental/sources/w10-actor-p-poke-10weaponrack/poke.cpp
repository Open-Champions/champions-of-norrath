// Observed msg_poke__10WeaponRacki at 0x0021cc08, 8 bytes: a bare return. It reads neither the
// object nor the integer argument and writes nothing. Nothing here ran on the EE.
extern "C" void msg_poke__10WeaponRacki(void* self, int poke) {
    (void)self;
    (void)poke;
}
