// Host contract for emathInit__Fv. Not an EE plan input.
#include <cstdio>

extern "C" void emathInit__Fv();

namespace {
int calls = 0;
}

extern "C" void initNoiseTable__Fv() { ++calls; }

int main() {
    emathInit__Fv();
    if (calls != 1) {
        std::fprintf(stderr, "fail: initNoiseTable calls %d\n", calls);
        return 1;
    }
    return 0;
}
