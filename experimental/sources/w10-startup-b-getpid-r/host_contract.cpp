#include <cstdio>
static int calls;
extern "C" int getpid() { ++calls; return 31337; }
#include "getpid_r.cpp"
int main() {
    int dummy;
    if (_getpid_r(&dummy) != 31337 || calls != 1 || _getpid_r(nullptr) != 31337 || calls != 2) { std::puts("_getpid_r wrong"); return 1; }
    std::puts("_getpid_r contract passed");
    return 0;
}
