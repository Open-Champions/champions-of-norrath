// Observed: OnKeyDown__11VIWndSplashi at 0x010d64c8 is 460 bytes. With (this, key), where mUI is the
// word _5VIWnd$mUI (0x01192c70) passed to SoundPlay__4VIUI11VIUISoundId:
//  - 135: store 1 at this+408 and at this+9664, SoundPlay(mUI, 13), SetFocus__5VIWndP5VIWndb(this, 0, true).
//  - 138: when the word at this+9656 is nonzero and the word at this+408 is above zero: decrement
//    this+408, ResetTickPulse__10VIUIWindow(this+40), SoundPlay(mUI, 11).
//  - 139: when this+9656 is nonzero and this+408 is zero or below: increment this+408, ResetTickPulse(this+40),
//    SoundPlay(mUI, 11).
//  - 134: nothing when this+9656 is zero. Otherwise by the value v of this+408:
//      v == 1: SoundPlay(mUI, 13), SetFocus(this, 0, true);
//      v == 0: SoundPlay(mUI, 15), SelectPlay__11VIWndSplash(this);
//      v == 2: store 1 at (this+2408)+28, SetFocus(this, this+2408, true), then SoundPlay(mUI, 15);
//      any other v: nothing.
//  - any other key: nothing.
// Inferred: 138 and 139 are up and down moving a menu selection held at this+408 (0 or 1 among the
// entries; 2 reaches an embedded sub-window at this+2408 whose word at +28 acts as its shown flag, as
// the child flag read by OnDraw__5VIWnd); this+9656 gates selection moves; 134 and 135 are accept keys.
// Unknown: the key names, the meaning of the words at this+9656 and this+9664, and what entry 2 is.
extern "C" {
extern unsigned _5VIWnd$mUI;
void SoundPlay__4VIUI11VIUISoundId(unsigned ui, int sound);
void SetFocus__5VIWndP5VIWndb(void* self, void* window, bool force);
void ResetTickPulse__10VIUIWindow(void* window);
void SelectPlay__11VIWndSplash(void* self);

void OnKeyDown__11VIWndSplashi(void* self, int key) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    int* selection = reinterpret_cast<int*>(bytes + 408);
    switch (key) {
    case 135:
        *selection = 1;
        *reinterpret_cast<int*>(bytes + 9664) = 1;
        SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 13);
        SetFocus__5VIWndP5VIWndb(self, 0, true);
        break;
    case 138:
        if (*reinterpret_cast<int*>(bytes + 9656) == 0) break;
        if (*selection <= 0) break;
        *selection = *selection - 1;
        ResetTickPulse__10VIUIWindow(bytes + 40);
        SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 11);
        break;
    case 139:
        if (*reinterpret_cast<int*>(bytes + 9656) == 0) break;
        if (*selection > 0) break;
        *selection = *selection + 1;
        ResetTickPulse__10VIUIWindow(bytes + 40);
        SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 11);
        break;
    case 134:
        if (*reinterpret_cast<int*>(bytes + 9656) == 0) break;
        if (*selection == 1) {
            SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 13);
            SetFocus__5VIWndP5VIWndb(self, 0, true);
        } else if (*selection == 0) {
            SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 15);
            SelectPlay__11VIWndSplash(self);
        } else if (*selection == 2) {
            *reinterpret_cast<int*>(bytes + 2408 + 28) = 1;
            SetFocus__5VIWndP5VIWndb(self, bytes + 2408, true);
            SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 15);
        }
        break;
    default:
        break;
    }
}
}
