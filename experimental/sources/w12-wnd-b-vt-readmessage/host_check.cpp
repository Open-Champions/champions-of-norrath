#include "vtable.cpp"
// Host check only; not a build input. Compiles vtable.cpp for the host with a stub per slot
// function and checks every slot's adjustment word and function identity and the table length.
#include <cstdio>
#include <cstring>

extern "C" {
const char* tag_0() __asm__("__tf16VIWndReadMessage");
const char* tag_0() { return "__tf16VIWndReadMessage"; }
const char* tag_1() __asm__("_$_16VIWndReadMessage");
const char* tag_1() { return "_$_16VIWndReadMessage"; }
const char* tag_2() __asm__("OnDraw__5VIWnd");
const char* tag_2() { return "OnDraw__5VIWnd"; }
const char* tag_3() __asm__("OnDrawSelf__16VIWndReadMessage");
const char* tag_3() { return "OnDrawSelf__16VIWndReadMessage"; }
const char* tag_4() __asm__("OnKeyUp__5VIWndi");
const char* tag_4() { return "OnKeyUp__5VIWndi"; }
const char* tag_5() __asm__("OnKeyDown__16VIWndReadMessagei");
const char* tag_5() { return "OnKeyDown__16VIWndReadMessagei"; }
const char* tag_6() __asm__("OnMessage__16VIWndReadMessageP5VIWndiii");
const char* tag_6() { return "OnMessage__16VIWndReadMessageP5VIWndiii"; }
extern Slot vtable[8] __asm__("_vt$16VIWndReadMessage");
}

int main() {
    const char* want[8] = {"__tf16VIWndReadMessage", "_$_16VIWndReadMessage", "OnDraw__5VIWnd", "OnDrawSelf__16VIWndReadMessage", "OnKeyUp__5VIWndi", "OnKeyDown__16VIWndReadMessagei", "OnMessage__16VIWndReadMessageP5VIWndiii", nullptr};
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
