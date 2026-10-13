// Host contract for texTerminateThread__Fv. Not an EE plan input.
#include <cstdio>
int decompressThreadid = 42;
static int seen = -1, calls = 0;
extern "C" int TerminateThread(int t) { seen = t; ++calls; return 0; }
extern "C" void texTerminateThread__Fv();
int main() {
    texTerminateThread__Fv();
    if (seen != 42 || calls != 1 || decompressThreadid != 42) return 1;
    std::printf("tex terminate thread contract passed\n");
    return 0;
}
