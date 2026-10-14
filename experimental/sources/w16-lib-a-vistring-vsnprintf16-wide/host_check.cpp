// Host check, not a plan input.
#include <cstdio>
#include "vsnprintf16.cpp"
static unsigned short* sb; static int sn; static const unsigned short* sf; static void* sa; static int calls;
extern "C" void VI_vsnprintf__H1ZUs_PX01iPCX01Pv_v(unsigned short* b, int n, const unsigned short* f, void* a) { ++calls; sb = b; sn = n; sf = f; sa = a; }
int main() {
    unsigned short out[2], fmt[2]; int args;
    vsnprintf16__8VIStringPUsiPCUsPv(out, -5, fmt, &args);
    if (calls != 1 || sb != out || sn != -5 || sf != fmt || sa != &args) return 1;
    std::puts("vsnprintf16 wide check passed");
    return 0;
}
