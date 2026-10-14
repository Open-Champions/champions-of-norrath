#include "reset_with_close.cpp"

#include <cstdio>
#include <cstring>

int MCA_FileOpen, errorCondition;
static char log_[128];
static int* sync_arg;
static void ev(const char* s) { std::strcat(log_, s); }

extern "C" void MCA_Reset__Fv() { ev("R;"); }
extern "C" void MCA_Cmd__Fiiii(int c, int t, int a, int b) {
    char tmp[32];
    std::snprintf(tmp, sizeof tmp, "C%d,%d,%d,%d;", c, t, a, b);
    ev(tmp);
}
extern "C" void MCA_Sync__FPi(int* e) { sync_arg = e; ev("S;"); }

int main() {
    MCA_FileOpen = 0;
    errorCondition = 5;
    resetWithClose__Fv();
    if (std::strcmp(log_, "R;") != 0 || errorCondition != 5) { std::printf("FAIL closed: %s\n", log_); return 1; }
    log_[0] = 0;
    MCA_FileOpen = 9;
    resetWithClose__Fv();
    if (std::strcmp(log_, "R;C1,0,0,0;S;") != 0 || sync_arg != &errorCondition) { std::printf("FAIL open: %s\n", log_); return 1; }
    std::puts("ok");
    return 0;
}
