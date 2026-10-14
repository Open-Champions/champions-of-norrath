// Host check only; not a build input. Build with the host C++ compiler. Checks the argument order (box first),
// the self+44 pointer and that the callee's result is returned unchanged.
#include "volume.cpp"
#include <cstdio>

static const void* seen_box;
static void* seen_planes;
extern "C" int StoreOutward__C6VIBBoxP7VIPlane(const void* box, void* planes) {
    seen_box = box;
    seen_planes = planes;
    return 4242;
}

int main() {
    alignas(16) unsigned char object[256];
    alignas(16) unsigned char box[64];
    int got = Volume__14VIQueryTriListRC6VIBBox(object, box);
    bool ok = got == 4242 && seen_box == box && seen_planes == object + 44;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
