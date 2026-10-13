// Observed: OnKeyDown__9VIWndEulai at 0x010b5220 is 348 bytes. With (this, key) it calls
// GetAppPtr__5VIWnd(this) and zeroes the word at this+9756, then dispatches on key - 134 (unsigned,
// six entries from a jump table; any other key does nothing more):
//  - 134 and 135: SoundPlay__4VIUI11VIUISoundId(mUI, 15), then EulaComplete__9VIStationUc(app, 1);
//  - 136: when the word at this+9764 is zero and the word at this+1544 is 1, SoundPlay(mUI, 11), set
//    this+1544 to 0, and call ResetTickPulse__10VIUIWindow(this+40);
//  - 137: the same with the condition this+1544 equal to 0, setting it to 1;
//  - 138: when this+9764 is zero, store 10 at this+9756 and -1 at this+9760, then call Up__9VIWndEula(this);
//  - 139: when this+9764 is zero, store 10 at this+9756 and 1 at this+9760, then call Down__9VIWndEula(this).
// mUI is the word _5VIWnd$mUI (0x01192c70), passed by value as the first argument of SoundPlay.
// Inferred: 138 and 139 are the up and down keys, 136 and 137 move a two-state selection (the word at
// this+1544, probably accept/decline), 134 and 135 confirm; this+9764 blocks input while set; this+9756
// and this+9760 are a repeat/scroll counter and direction. The key code names are not established.
// Unknown: the exact meaning of those words, and the original's 4 KiB stack frame (no buffer is read here).
extern "C" {
extern unsigned _5VIWnd$mUI;
void* GetAppPtr__5VIWnd(void* self);
void SoundPlay__4VIUI11VIUISoundId(unsigned ui, int sound);
void EulaComplete__9VIStationUc(void* station, unsigned char accepted);
void ResetTickPulse__10VIUIWindow(void* window);
void Up__9VIWndEula(void* self);
void Down__9VIWndEula(void* self);

void OnKeyDown__9VIWndEulai(void* self, int key) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    void* app = GetAppPtr__5VIWnd(self);
    *reinterpret_cast<int*>(bytes + 9756) = 0;
    switch (key) {
    case 134:
    case 135:
        SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 15);
        EulaComplete__9VIStationUc(app, 1);
        break;
    case 136:
        if (*reinterpret_cast<int*>(bytes + 9764) != 0) break;
        if (*reinterpret_cast<int*>(bytes + 1544) != 1) break;
        SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 11);
        *reinterpret_cast<int*>(bytes + 1544) = 0;
        ResetTickPulse__10VIUIWindow(bytes + 40);
        break;
    case 137:
        if (*reinterpret_cast<int*>(bytes + 9764) != 0) break;
        if (*reinterpret_cast<int*>(bytes + 1544) != 0) break;
        SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 11);
        *reinterpret_cast<int*>(bytes + 1544) = 1;
        ResetTickPulse__10VIUIWindow(bytes + 40);
        break;
    case 138:
        if (*reinterpret_cast<int*>(bytes + 9764) != 0) break;
        *reinterpret_cast<int*>(bytes + 9756) = 10;
        *reinterpret_cast<int*>(bytes + 9760) = -1;
        Up__9VIWndEula(self);
        break;
    case 139:
        if (*reinterpret_cast<int*>(bytes + 9764) != 0) break;
        *reinterpret_cast<int*>(bytes + 9756) = 10;
        *reinterpret_cast<int*>(bytes + 9760) = 1;
        Down__9VIWndEula(self);
        break;
    default:
        break;
    }
}
}
