static int calls;
static int result;
extern "C" int DLG_Active__Fv() {
    ++calls;
    return result;
}
extern "C" int amxDialogRunning__FP5__amxPi(void*, int*);
int main() {
    result = 0;
    if (amxDialogRunning__FP5__amxPi(0, 0) != 0 || calls != 1) return 1;
    result = 1;
    if (amxDialogRunning__FP5__amxPi(0, 0) != 1 || calls != 2) return 2;
    return 0;
}
