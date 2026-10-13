struct Slot { unsigned adjustment; void (*function)(); };
extern "C" {
void s0() __asm__("__tf9VIWndEdit"); void s1() __asm__("_$_9VIWndEdit");
void s2() __asm__("OnDraw__5VIWnd"); void s3() __asm__("OnDrawSelf__9VIWndEdit");
void s4() __asm__("OnKeyUp__9VIWndEditi"); void s5() __asm__("OnKeyDown__9VIWndEditi");
void s6() __asm__("OnMessage__9VIWndEditP5VIWndiii");
extern Slot actual[8] __asm__("_vt$9VIWndEdit");
void s0() {} void s1() {} void s2() {} void s3() {} void s4() {} void s5() {} void s6() {}
}
int main() {
    void (*expected[8])() = {s0, s1, s2, s3, s4, s5, s6, nullptr};
    for (unsigned i = 0; i != 8; ++i)
        if (actual[i].adjustment != 0 || actual[i].function != expected[i]) return 1;
    return 0;
}
