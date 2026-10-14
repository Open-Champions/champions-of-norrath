// Host contract for memCheckHeapReal__FPCci. Not an EE plan input.
#include <cstdio>
extern "C" void memCheckHeapReal__FPCci(const char*, int);
int main() {
    memCheckHeapReal__FPCci("file.cpp", 502);
    memCheckHeapReal__FPCci(nullptr, -1);
    std::printf("memCheckHeapReal contract passed\n");
    return 0;
}
