// Host check only; not a build input. Build as x86_64 with statics below 4 GiB. Elements are
// 20-byte records with the child word at offset 16; table entries are host-sized. Covers empty,
// one, several children, a null child, and flags 0 to 3 (delete only when bit 0 is set).
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char _vt$5VIWnd[8];
static unsigned char wnd[128];
static unsigned char elements[5 * 20];
static unsigned char child[4][128];
static VtEntry table[4][2];
static int count;
static std::string trace;

static std::string who(void* p) {
    unsigned char* b = static_cast<unsigned char*>(p);
    for (int i = 0; i < 4; ++i)
        if (b >= child[i] && b < child[i] + 128) return "c" + std::to_string(i) + "+" + std::to_string(b - child[i]);
    return b == wnd ? "self" : "?";
}
static void vfn(void* p, int f) { trace += "v:" + who(p) + "/" + std::to_string(f) + ";"; }

extern "C" {
Iterator Begin__Ct5VISet1ZP5VIWnd(void*) { trace += "begin;"; return 0; }
Iterator End__Ct5VISet1ZP5VIWnd(void*) { return count; }
Iterator Next__Ct5VISet1ZP5VIWndi(void*, Iterator it) { trace += "next;"; return it + 1; }
void RemoveChild__5VIWndP5VIWnd(void* s, void* c) { trace += "rm:" + who(s) + "," + who(c) + ";"; }
void _$_t5VISet1ZP5VIWnd(void* s, int f) { trace += "set:" + who(s) + "/" + std::to_string(f) + ";"; }
void __builtin_delete(void* p) { trace += "del:" + who(p) + ";"; }
}

static bool run(int n, int null_child, int flags) {
    std::memset(wnd, 0x5a, sizeof wnd);
    std::memset(elements, 0, sizeof elements);
    count = n; trace.clear();
    unsigned base = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(elements));
    std::memcpy(wnd + 8, &base, 4);
    for (int i = 0; i < n; ++i) {
        unsigned c = i == null_child ? 0 : static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(child[i]));
        std::memcpy(elements + 20 * i + 16, &c, 4);
        table[i][1] = VtEntry{static_cast<short>(8 * i + 4), 3, vfn};
        unsigned t = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(table[i]));
        std::memcpy(child[i] + 32, &t, 4);
    }
    dtor_5VIWnd(wnd, flags);
    std::string want = "begin;";
    for (int i = 0; i < n; ++i) {
        want += "next;";
        if (i == null_child) { want += "rm:self,?;"; continue; }
        want += "rm:self,c" + std::to_string(i) + "+0;v:c" + std::to_string(i) + "+" + std::to_string(8 * i + 4) + "/3;";
    }
    want += "set:self/2;";
    if (flags & 1) want += "del:self;";
    unsigned vt;
    std::memcpy(&vt, wnd + 32, 4);
    bool ok = trace == want && vt == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$5VIWnd));
    if (!ok) std::printf("got  %s\nwant %s\n", trace.c_str(), want.c_str());
    return ok;
}

int main() {
    bool ok = true;
    for (int flags = 0; flags < 4; ++flags) {
        ok = run(0, -1, flags) && ok;
        ok = run(1, -1, flags) && ok;
        ok = run(3, -1, flags) && ok;
        ok = run(3, 1, flags) && ok;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
