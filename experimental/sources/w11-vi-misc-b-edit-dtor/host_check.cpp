// Host check only; not a build input. Build as x86_64 with statics below 4 GiB. Runs the routine with
// recording stubs for flags 0 to 3 (and, where the routine tests a word, both a zero and a nonzero
// word) and checks the call order, this+offset arguments, stored words and untouched neighbours.
// The same cases were compared with the original in a private interpreter run.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char _vt$9VIWndEdit[8];
static unsigned char object[0x8000];
static std::string trace;
static std::string off(void* p) { return std::to_string(static_cast<unsigned char*>(p) - object); }
extern "C" {
void _$_5VIWnd(void* self, int flags) { trace += "_$_5VIWnd:" + off(self) + "/" + std::to_string(flags) + ";"; }
void RemoveChild__5VIWndP5VIWnd(void* self, void* child) { trace += "rm:" + off(self) + "," + std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(child)) + ";"; }
void __builtin_vec_delete(void* p) { trace += "vecdel:" + std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(p)) + ";"; }
void __builtin_delete(void* p) { trace += "del:" + off(p) + ";"; }
}

static bool run([[maybe_unused]] unsigned mask, int flags) {
    std::memset(object, 0x5a, sizeof object);
    unsigned char expect[0x8000];
    std::memset(expect, 0x5a, sizeof expect);
    std::string want;
    { unsigned v = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$9VIWndEdit)); std::memcpy(expect + 32, &v, 4); }
    { unsigned v = (mask >> 0) & 1 ? 0x1000u + 424 : 0u; std::memcpy(object + 424, &v, 4); std::memcpy(expect + 424, &v, 4); }
    { unsigned v = (mask >> 1) & 1 ? 0x1000u + 432 : 0u; std::memcpy(object + 432, &v, 4); std::memcpy(expect + 432, &v, 4); }
    if ((mask >> 0) & 1) want += "vecdel:" + std::to_string(0x1000u + 424) + ";";
    if ((mask >> 1) & 1) want += "vecdel:" + std::to_string(0x1000u + 432) + ";";
    want += "_$_5VIWnd:0/0;";
    if (flags & 1) want += "del:0;";
    trace.clear();
    dtor_9VIWndEdit(object, flags);
    bool ok = trace == want && std::memcmp(object, expect, sizeof object) == 0;
    if (!ok) std::printf("got  %s\nwant %s\n", trace.c_str(), want.c_str());
    return ok;
}

int main() {
    bool ok = true;
    for (int flags = 0; flags < 4; ++flags)
        for (unsigned mask = 0; mask < 4u; ++mask) ok = run(mask, flags) && ok;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
