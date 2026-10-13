struct Slot { unsigned adjustment; void (*function)(); };
extern "C" {
void s0() __asm__("__tf5VIWnd"); void s1() __asm__("_$_5VIWnd");
void s2() __asm__("OnDraw__5VIWnd"); void s3() __asm__("OnDrawSelf__5VIWnd");
void s4() __asm__("OnKeyUp__5VIWndi"); void s5() __asm__("OnKeyDown__5VIWndi");
void s6() __asm__("OnMessage__5VIWndP5VIWndiii");
extern Slot actual[8] __asm__("_vt$5VIWnd");
}
static void f0() {} static void f1() {} static void f2() {} static void f3() {}
static void f4() {} static void f5() {} static void f6() {}
extern "C" {
void s0() { f0(); } void s1() { f1(); } void s2() { f2(); }
void s3() { f3(); } void s4() { f4(); } void s5() { f5(); } void s6() { f6(); }
}
int main() {
    void (*expected[8])() = {s0, s1, s2, s3, s4, s5, s6, nullptr};
    for (unsigned i = 0; i != 8; ++i)
        if (actual[i].adjustment != 0 || actual[i].function != expected[i]) return 1;
    return 0;
}
