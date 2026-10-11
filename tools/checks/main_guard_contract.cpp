#include <cstdio>
extern "C" void norrath_test_main();
static unsigned calls;
extern "C" void norrath_test_ctors() {
    ++calls;
    if (calls < 10) norrath_test_main();
}
int main() {
    for (unsigned i = 0; i < 10000; ++i) norrath_test_main();
    if (calls != 1) return 1;
    std::puts("10000 repeated calls and constructor reentry passed");
}
