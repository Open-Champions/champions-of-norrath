// Host check only; not a build input.
#include "allocate.cpp"
#include <cstdio>
static unsigned want, calls, result;
extern "C" unsigned __builtin_vec_new(unsigned size) { want = size; ++calls; return result; }
int main() {
    unsigned s[96] = {};
    s[77] = 0; s[78] = 0x77;
    bool ok = Allocate__9VISurface(reinterpret_cast<unsigned char*>(s)) == 0 && calls == 0 && s[78] == 0x77;  // zero size: no allocation
    s[77] = 6000; result = 0x4000;
    ok = ok && Allocate__9VISurface(reinterpret_cast<unsigned char*>(s)) == 0 && calls == 1 && want == 6000 && s[78] == 0x4000;
    result = 0;
    ok = ok && Allocate__9VISurface(reinterpret_cast<unsigned char*>(s)) == -1 && calls == 2 && s[78] == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
