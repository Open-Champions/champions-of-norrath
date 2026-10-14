// Host check only; not an EE build input. Build the source with -D__unwinding_cleanup=w15_unwinding_cleanup.
#include <cstdio>
extern "C" void w15_unwinding_cleanup();
int main() {
    w15_unwinding_cleanup();   // must return without touching anything
    std::puts("unwinding_cleanup ok");
    return 0;
}
