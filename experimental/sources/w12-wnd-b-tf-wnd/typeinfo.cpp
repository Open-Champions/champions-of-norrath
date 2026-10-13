// Observed __tf5VIWnd at 0x0120aba0, 64 bytes. It returns its eight-byte
// __ti5VIWnd node. A zero first word calls __rtti_user with that node and the
// class-name bytes "5VIWnd"; a nonzero word skips initialization. The node's
// remaining bytes are not read by this accessor. The runtime meaning of its
// initialized fields is outside this function's evidence.
extern "C" {
unsigned char ti_wnd[8] __asm__("__ti5VIWnd");
void rtti_user(void*, const char*) __asm__("__rtti_user");
void* wnd_typeinfo() __asm__("__tf5VIWnd");
void* wnd_typeinfo() {
    unsigned first = *reinterpret_cast<unsigned*>(ti_wnd);
    if (first == 0u) rtti_user(ti_wnd, "5VIWnd");
    return ti_wnd;
}
}
