// Host contract for DLG_ForceMutexRelease. Not an EE plan input.
#include "force_mutex.cpp"

extern "C" int dialogMutex = 7;

int main() {
    dialogMutex = 7;
    DLG_ForceMutexRelease__Fv();
    if (dialogMutex != 0) return 1;
    dialogMutex = -1;
    DLG_ForceMutexRelease__Fv();
    if (dialogMutex != 0) return 2;
    return 0;
}
