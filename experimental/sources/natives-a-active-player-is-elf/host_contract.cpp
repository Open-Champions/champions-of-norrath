extern "C" int amxActivePlayerIsElf__FP5__amxPi(void* amx, int* params);
int main() {
    int params[2] = {4, 7};
    int token = 3;
    if (amxActivePlayerIsElf__FP5__amxPi(&token, params) != 0) return 1;
    if (token != 3 || params[1] != 7) return 2;
    if (amxActivePlayerIsElf__FP5__amxPi(0, 0) != 0) return 3;
    return 0;
}
