// amxCheckIfAnyMonstersRemain is the 48-byte native at 0x0013b788. It sets
// the byte noMonsters at 0x00486840 to 1, walks the grid with
// checkForMonsters at 0x0013b760, and returns 1 only when that byte is then 0.
extern "C" char noMonsters;
extern "C" void checkForMonsters__FP10GameObject(void* object);
extern "C" void objectIterateGrid__FPFP10GameObject_v(void (*visit)(void*));
extern "C" int amxCheckIfAnyMonstersRemain__FP5__amxPi(void* amx, int* params) {
    (void)amx;
    (void)params;
    noMonsters = 1;
    objectIterateGrid__FPFP10GameObject_v(checkForMonsters__FP10GameObject);
    return noMonsters == 0 ? 1 : 0;
}
