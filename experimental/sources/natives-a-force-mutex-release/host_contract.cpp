static int calls;
extern "C" void DLG_ForceMutexRelease__Fv() { ++calls; }
extern "C" int amxForceMutexRelease__FP5__amxPi(void*, int*);
int main() {
    if (amxForceMutexRelease__FP5__amxPi(0, 0) != 0 || calls != 1) return 1;
    return 0;
}
