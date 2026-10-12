#include <cstdio>
#include "load_thread_id.cpp"
int main() {
    static_assert(sizeof(loadThreadID) == 4, "four-byte word");
    if (loadThreadID != 0) return 1;
    loadThreadID = 33;
    if (loadThreadID != 33) return 2;
    std::puts("loadThreadID contract passed");
    return 0;
}
