// Host check only; not a build input. Compiles vtable.cpp for the host with a stub per slot
// function and checks every slot's adjustment word and function identity and the table length.
#include "vtable.cpp"
#include <cstdio>
#include <cstring>

extern "C" {
const char* tag_0() __asm__("__tf9VIWndDnas");
const char* tag_0() { return "__tf9VIWndDnas"; }
const char* tag_1() __asm__("_$_9VIWndDnas");
const char* tag_1() { return "_$_9VIWndDnas"; }
const char* tag_2() __asm__("OnDraw__5VIWnd");
const char* tag_2() { return "OnDraw__5VIWnd"; }
const char* tag_3() __asm__("OnDrawSelf__9VIWndDnas");
const char* tag_3() { return "OnDrawSelf__9VIWndDnas"; }
const char* tag_4() __asm__("OnKeyUp__9VIWndDnasi");
const char* tag_4() { return "OnKeyUp__9VIWndDnasi"; }
const char* tag_5() __asm__("OnKeyDown__9VIWndDnasi");
const char* tag_5() { return "OnKeyDown__9VIWndDnasi"; }
const char* tag_6() __asm__("OnMessage__9VIWndDnasP5VIWndiii");
const char* tag_6() { return "OnMessage__9VIWndDnasP5VIWndiii"; }
extern Slot vtable[8] __asm__("_vt$9VIWndDnas");
}

int main() {
    const char* want[8] = {"__tf9VIWndDnas", "_$_9VIWndDnas", "OnDraw__5VIWnd", "OnDrawSelf__9VIWndDnas", "OnKeyUp__9VIWndDnasi", "OnKeyDown__9VIWndDnasi", "OnMessage__9VIWndDnasP5VIWndiii", nullptr};
    typedef const char* (*Tag)();
    int bad = 0;
    for (int i = 0; i < 8; ++i) {
        if (vtable[i].adjustment != 0) bad = 1;
        if (want[i] == nullptr) { if (vtable[i].function != nullptr) bad = 1; continue; }
        if (vtable[i].function == nullptr || std::strcmp(reinterpret_cast<Tag>(vtable[i].function)(), want[i]) != 0) bad = 1;
    }
    std::puts(bad ? "FAIL" : "ok");
    return bad;
}
