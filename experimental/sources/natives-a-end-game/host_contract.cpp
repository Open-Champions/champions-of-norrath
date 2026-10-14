static int calls;
static int slot = -1;
static int mode = -1;
extern "C" int MC_SaveConfigSilent__Fii(int got_slot, int got_mode) {
    ++calls;
    slot = got_slot;
    mode = got_mode;
    return 9;
}
extern "C" int amxEndGame__FP5__amxPi(void*, int*);
int main() {
    if (amxEndGame__FP5__amxPi(0, 0) != 0) return 1;
    if (calls != 1 || slot != 0 || mode != 1) return 2;
    return 0;
}
