#include "window_ui.cpp"

int main() {
    VIUI* ui = reinterpret_cast<VIUI*>(0x1234);
    norrath_wnd_focus = reinterpret_cast<void*>(0x77);
    VISetWindowUI__FP4VIUI(ui);
    if (norrath_wnd_ui != ui || norrath_wnd_focus != 0) return 1;
    VISetWindowUI__FP4VIUI(0);
    if (norrath_wnd_ui != 0) return 2;
    return 0;
}
