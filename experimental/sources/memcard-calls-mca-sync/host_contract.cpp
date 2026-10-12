#include "mca_sync.cpp"

#include <cstdio>

static int calls;
static int script;

extern "C" int MCA_Process__FPi(int* error) {
    ++calls;
    if (script == 1) {
        *error = 0;
        return calls == 1 ? 1 : 0;
    }
    if (script == 2) {
        *error = 9;
        return 4;
    }
    *error = 0;
    return 0;
}

int main() {
    int error = 3;
    script = 1;
    calls = 0;
    if (MCA_Sync__FPi(&error) != 0 || calls != 2 || error != 0) return 1;
    script = 2;
    calls = 0;
    error = 0;
    if (MCA_Sync__FPi(&error) != 1 || calls != 1 || error != 9) return 2;
    script = 3;
    calls = 0;
    error = 0;
    if (MCA_Sync__FPi(&error) != 0 || calls != 1) return 3;
    std::puts("mca sync passed");
    return 0;
}
