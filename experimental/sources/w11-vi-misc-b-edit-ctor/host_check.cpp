// Host check only; not a build input. Build as x86_64 with statics below 4 GiB. Each stub records
// its call and a checksum of the object bytes at that moment, so the order of the stores relative
// to the calls is checked; the final bytes are compared with a table of the documented values.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char _vt$9VIWndEdit[8];
static unsigned char object[512];
static std::string trace;
static unsigned long long sum() {
    unsigned long long s = 1469598103934665603ull;
    for (unsigned char b : object) s = (s ^ b) * 1099511628211ull;
    return s;
}
extern "C" {
void __5VIWnd(void*) { trace += "base:" + std::to_string(sum()) + ";"; }
void SetMaxTextLen__9VIWndEditi(void*, int n) { trace += "max" + std::to_string(n) + ":" + std::to_string(sum()) + ";"; }
void ClearText__9VIWndEdit(void*) { trace += "clear:" + std::to_string(sum()) + ";"; }
void SetDisplayRows__9VIWndEditi(void*, int n) { trace += "rows" + std::to_string(n) + ":" + std::to_string(sum()) + ";"; }
void EnableCr__9VIWndEditi(void*, int n) { trace += "cr" + std::to_string(n) + ":" + std::to_string(sum()) + ";"; }
}

static void put32(unsigned char* b, int off, unsigned v) { std::memcpy(b + off, &v, 4); }

int main() {
    std::memset(object, 0x5a, sizeof object);
    unsigned char expect[512], before[512];
    std::memset(expect, 0x5a, sizeof expect);
    std::memcpy(before, object, sizeof before);
    const unsigned vt = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$9VIWndEdit));
    // image after step 2
    std::memcpy(expect, before, sizeof expect);
    unsigned char s2[512];
    std::memcpy(s2, before, sizeof s2);
    for (int off : {56, 60, 64, 68, 328, 332, 424, 336, 340, 432, 344, 348}) put32(s2, off, 0);
    put32(s2, 32, vt);
    auto hash = [](const unsigned char* p) {
        unsigned long long s = 1469598103934665603ull;
        for (int i = 0; i < 512; ++i) s = (s ^ p[i]) * 1099511628211ull;
        return s;
    };
    unsigned char s4[512];
    std::memcpy(s4, s2, sizeof s4);
    for (int off : {36, 40, 356, 360, 364, 352, 44, 48, 52, 416, 420, 328, 332, 336, 340, 344, 348, 396, 400, 404, 440}) put32(s4, off, 0);
    put32(s4, 56, 16); put32(s4, 60, 40); put32(s4, 368, 1); put32(s4, 444, 1);
    unsigned char fin[512];
    std::memcpy(fin, s4, sizeof fin);
    fin[72] = 0; fin[73] = 0;
    put32(fin, 376, 0); put32(fin, 380, 0);
    const std::string want = "base:" + std::to_string(hash(before)) + ";max64:" + std::to_string(hash(s2)) + ";clear:" +
                             std::to_string(hash(s2)) + ";rows2:" + std::to_string(hash(s4)) + ";cr0:" +
                             std::to_string(hash(s4)) + ";";
    __9VIWndEdit(object);
    bool ok = trace == want && std::memcmp(object, fin, sizeof object) == 0;
    if (!ok) std::printf("got  %s\nwant %s\n", trace.c_str(), want.c_str());
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
