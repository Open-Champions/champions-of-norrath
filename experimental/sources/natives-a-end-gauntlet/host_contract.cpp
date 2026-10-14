extern "C" int amxEndGauntlet__FP5__amxPi(void* amx, int* params);
int main() {
    int params[1] = {99};
    int token = 4;
    if (amxEndGauntlet__FP5__amxPi(&token, params) != 0) return 1;
    if (token != 4 || params[0] != 99) return 2;
    return 0;
}
