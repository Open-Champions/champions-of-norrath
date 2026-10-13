// Host contract for DLG_CheckMutex__Fv. Not an EE plan input.
#include "check_mutex.cpp"
extern "C" int dialogMutex = 0;
int main() {
    const int values[] = {0, 1, -1, 0x7fffffff, -0x7fffffff - 1, 42};
    for (int i = 0; i < 6; ++i) {
        dialogMutex = values[i];
        if (DLG_CheckMutex__Fv() != values[i]) return 1 + i;
        if (dialogMutex != values[i]) return 10 + i;
    }
    return 0;
}
