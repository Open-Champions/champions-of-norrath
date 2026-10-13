#include "exit_stub.cpp"

static int calls;
static int seen;
extern "C" void Exit(int status) {
    ++calls;
    seen = status;
}

int main() {
    _exit(0);
    if (calls != 1 || seen != 0) return 1;
    _exit(1);
    if (calls != 2 || seen != 1) return 2;
    _exit(-7);
    if (calls != 3 || seen != -7) return 3;
    return 0;
}
