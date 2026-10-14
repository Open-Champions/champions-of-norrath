extern "C" char noMonsters = 0;
static int entered;
static int clear_on_visit;
extern "C" void checkForMonsters__FP10GameObject(void*) {}
extern "C" void objectIterateGrid__FPFP10GameObject_v(void (*visit)(void*)) {
    entered = static_cast<unsigned char>(noMonsters);
    if (visit != checkForMonsters__FP10GameObject) entered = -1;
    if (clear_on_visit) noMonsters = 0;
}
extern "C" int amxCheckIfAnyMonstersRemain__FP5__amxPi(void*, int*);
int main() {
    noMonsters = 7;
    clear_on_visit = 1;
    if (amxCheckIfAnyMonstersRemain__FP5__amxPi(0, 0) != 1) return 1;
    if (entered != 1 || noMonsters != 0) return 2;
    clear_on_visit = 0;
    noMonsters = 7;
    if (amxCheckIfAnyMonstersRemain__FP5__amxPi(0, 0) != 0) return 3;
    if (entered != 1 || noMonsters != 1) return 4;
    return 0;
}
