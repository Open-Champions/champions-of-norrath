// Host check only; not a build input.
#include "allocate.cpp"
#include <cstdio>
static unsigned want, calls, result;
extern "C" unsigned __builtin_vec_new(unsigned size) { want = size; ++calls; return result; }
int main() {
    unsigned font[32] = {};
    font[20] = 0; font[21] = 0x77;
    bool ok = Allocate__7VIRFont(reinterpret_cast<unsigned char*>(font)) == 0 && calls == 0 && font[21] == 0x77;  // zero size: no allocation
    font[20] = 5000; result = 0x4000;
    ok = ok && Allocate__7VIRFont(reinterpret_cast<unsigned char*>(font)) == 0 && calls == 1 && want == 5000 && font[21] == 0x4000;
    result = 0;
    ok = ok && Allocate__7VIRFont(reinterpret_cast<unsigned char*>(font)) == -1 && calls == 2 && font[21] == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
