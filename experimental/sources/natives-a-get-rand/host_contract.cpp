extern "C" unsigned syncRandState = 0;
extern "C" int amxGetRand__FP5__amxPi(void*, int*);
int main() {
    syncRandState = 1u;
    if (amxGetRand__FP5__amxPi(0, 0) != 0x3c88) return 1;
    if (syncRandState != 0x3c88596cu) return 2;
    unsigned again = syncRandState * 0x19660du + 0x3c6ef35fu;
    if (amxGetRand__FP5__amxPi(0, 0) != static_cast<int>(again >> 16)) return 3;
    if (syncRandState != again) return 4;
    syncRandState = 0xffffffffu;
    unsigned wrapped = 0xffffffffu * 0x19660du + 0x3c6ef35fu;
    if (amxGetRand__FP5__amxPi(0, 0) != static_cast<int>(wrapped >> 16)) return 5;
    if (syncRandState != wrapped) return 6;
    return 0;
}
