// Host comparison only. Not an EE build input. Scripted callees record call order and arguments.
#include <cstdio>
#include "exit.cpp"
static int order[4], n, status_seen = -99;
extern "C" void TerminateLibrary() { order[n++] = 1; }
extern "C" void _Exit(int status) { order[n++] = 2; status_seen = status; }
int main() {
    Exit(101);
    if (n != 2 || order[0] != 1 || order[1] != 2) { std::printf("order wrong\n"); return 1; }
    if (status_seen != 101) { std::printf("status not passed on\n"); return 2; }
    Exit(-1);
    if (n != 4 || status_seen != -1) { std::printf("second call\n"); return 3; }
    std::printf("exit cases passed\n");
    return 0;
}
