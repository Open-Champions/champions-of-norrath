#include <cstdio>
using Callback = void (*)();
extern "C" void norrath_test_dtors();
static unsigned trace[16], length;
static bool reenter;
static void first() {
    trace[length++] = 1;
    if (reenter) { norrath_test_dtors(); trace[length++] = 4; }
}
static void second() { trace[length++] = 2; }
static void third() { trace[length++] = 3; }
extern "C" {
Callback norrath_test_dtor_list[] = {nullptr, first, second, third, nullptr};
unsigned char norrath_test_frames[4];
void norrath_test_deregister(const void* frames) {
    if (frames != norrath_test_frames) __builtin_trap();
    trace[length++] = 9;
    norrath_test_dtors(); // The completed flag must already be set.
}
}
int main(int argc, char**) {
    reenter = argc == 2;
    if (argc == 3) norrath_test_dtor_list[1] = nullptr;
    norrath_test_dtors();
    for (unsigned i = 0; i < 100; ++i) norrath_test_dtors();
    unsigned expected[] = {1, 2, 3, 9, 4};
    if (argc == 3) {
        if (length != 1 || trace[0] != 9) return 1;
    } else {
        if (length != (reenter ? 5u : 4u)) return 2;
        for (unsigned i = 0; i < length; ++i) if (trace[i] != expected[i]) return 3;
    }
    std::puts("destructor order, persistent cursor and deregistration guard passed");
}
