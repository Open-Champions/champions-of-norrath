static int calls;
static int result = 5;
extern "C" int gameCheckPlayerAlive__Fv() {
    ++calls;
    return result;
}
extern "C" int amxCheckPlayerAlive__FP5__amxPi(void*, int*);
int main() {
    int params[1] = {8};
    if (amxCheckPlayerAlive__FP5__amxPi(0, params) != 5 || calls != 1) return 1;
    result = -2;
    if (amxCheckPlayerAlive__FP5__amxPi(0, params) != -2 || calls != 2) return 2;
    if (params[0] != 8) return 3;
    return 0;
}
