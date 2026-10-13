// Host check only; not a build input. Build as x86_64 with statics below 4 GiB. Checks the
// stored words and the set constructor call. The same fill was compared with the original in a
// private interpreter run.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>

unsigned char _vt$5VIWnd[8];
static unsigned char wnd[128];
static void* constructed;
extern "C" void __t5VISet1ZP5VIWnd(void* self) { constructed = self; }

int main() {
    std::memset(wnd, 0x5a, sizeof wnd);
    __5VIWnd(wnd);
    unsigned w[4];
    std::memcpy(w, wnd + 20, 4); std::memcpy(w + 1, wnd + 24, 4);
    std::memcpy(w + 2, wnd + 28, 4); std::memcpy(w + 3, wnd + 32, 4);
    bool ok = constructed == wnd && w[0] == 0 && w[1] == 0 && w[2] == 1 &&
              w[3] == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$5VIWnd)) &&
              wnd[19] == 0x5a && wnd[36] == 0x5a;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
