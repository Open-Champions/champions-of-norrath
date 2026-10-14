// Host contract for DLG_SetMutex__Fv. Not an EE plan input.
#include "set_mutex.cpp"
extern "C" int dialogMutex = 0;
int main() {
    dialogMutex = 0;
    if (DLG_SetMutex__Fv() != 0 || dialogMutex != 1) return 1;
    if (DLG_SetMutex__Fv() != -1 || dialogMutex != 1) return 2;
    dialogMutex = -3;
    if (DLG_SetMutex__Fv() != -1 || dialogMutex != -3) return 3;
    dialogMutex = 0;
    if (DLG_SetMutex__Fv() != 0 || dialogMutex != 1) return 4;
    return 0;
}
