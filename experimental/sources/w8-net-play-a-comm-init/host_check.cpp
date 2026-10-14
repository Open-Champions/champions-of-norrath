#include "comm_init.cpp"
int comm_handle;
unsigned char enableMallocWrapper;
static int calls, seen, wrapperDuringCall;
void lgAudClose(int handle) {
    ++calls;
    seen = handle;
    wrapperDuringCall = enableMallocWrapper;
}
int main() {
    comm_handle = -1;
    commInit__Fv();
    if (calls != 0 || comm_handle != -1 || enableMallocWrapper != 0) return 1;
    comm_handle = 7;
    commInit__Fv();
    if (calls != 1 || seen != 7) return 2;
    if (wrapperDuringCall != 1) return 3;
    if (comm_handle != -1 || enableMallocWrapper != 0) return 4;
    return 0;
}
