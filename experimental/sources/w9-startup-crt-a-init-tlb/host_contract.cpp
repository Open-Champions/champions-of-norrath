#include "init_tlb.cpp"

#include <cstdio>

static int size_result;
static int calls32;
static int callsdefault;

extern "C" int GetMemorySize() { return size_result; }
extern "C" void InitTLB32MB() { ++calls32; }
extern "C" void _InitTLB() { ++callsdefault; }

int main() {
    size_result = 0x02000000;
    InitTLB();
    if (calls32 != 1 || callsdefault != 0) return 1;
    size_result = 0x04000000;
    InitTLB();
    if (calls32 != 1 || callsdefault != 1) return 2;
    size_result = 0x01ffffff;
    InitTLB();
    size_result = 0x02000001;
    InitTLB();
    if (calls32 != 1 || callsdefault != 3) return 3;
    std::puts("init tlb contract passed");
    return 0;
}
