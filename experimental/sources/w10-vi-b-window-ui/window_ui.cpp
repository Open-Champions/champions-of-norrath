// Observed VISetWindowUI__FP4VIUI at 0x010c3338, 20 bytes. It stores its
// argument in _5VIWnd$mUI (0x01192c70) and clears _5VIWnd$mFocus (0x01192c74).
// The two words are published under their symbol-table names; what VIWnd
// does with them is not established here.
extern "C" {
struct VIUI;
VIUI* norrath_wnd_ui __asm__("_5VIWnd$mUI");
void* norrath_wnd_focus __asm__("_5VIWnd$mFocus");

void VISetWindowUI__FP4VIUI(VIUI* ui) {
    norrath_wnd_ui = ui;
    norrath_wnd_focus = 0;
}
}
