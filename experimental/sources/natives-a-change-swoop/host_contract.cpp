extern "C" int DLG_SwoopMode = 0;
extern "C" int amxChangeSwoop__FP5__amxPi(void*, int*);
int main() {
    int params[2] = {4, 0};
    DLG_SwoopMode = 9;
    if (amxChangeSwoop__FP5__amxPi(0, params) != 0 || DLG_SwoopMode != 0) return 1;
    params[1] = -3;
    if (amxChangeSwoop__FP5__amxPi(0, params) != 0 || DLG_SwoopMode != -3) return 2;
    return 0;
}
