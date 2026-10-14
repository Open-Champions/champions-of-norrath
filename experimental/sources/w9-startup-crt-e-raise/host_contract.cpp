#include <cstdio>
#include "raise.cpp"
static unsigned char reent_object[8];
extern "C" void* _impure_ptr = reent_object;
static void* seen_reent; static int seen_signal;
extern "C" int _raise_r(void* reent, int signal_number) {
    seen_reent = reent; seen_signal = signal_number; return signal_number * 3 + 1;
}
int main() {
    if (raise(6) != 19) return 1;
    if (seen_reent != reent_object || seen_signal != 6) return 2;
    _impure_ptr = nullptr;
    raise(2);
    if (seen_reent != nullptr || seen_signal != 2) return 3;
    std::puts("raise contract passed");
    return 0;
}
