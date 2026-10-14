// Observed: Next__12VIWndOptions at 0x010c4a60 is 252 bytes. With this and the word at +1144 (the section):
//  - section -1: when the word at +1180 is below 3, it adds one to it (otherwise only sound 9 is played);
//  - any other section: a local table of three limits {2, 3, 3} is copied from the read-only words at
//    0x011d53b8; if (VIGetFeatureFlags__Fv() & 0x100) is nonzero, entry 1 becomes 4. The word at +1148 is
//    increased by one when it is below the limit indexed by the section (the index is not range checked).
// A successful step calls ResetTickPulse__10VIUIWindow(this + 40) and then SoundPlay__4VIUI11VIUISoundId(mUI, 11);
// a refused step calls SoundPlay(mUI, 9). mUI is the word _5VIWnd$mUI (0x01192c70). Nothing is returned.
// Inferred: the mirror of Prev__12VIWndOptions; the limits are the largest cursor values of the three sub-lists,
// and the feature flag lengthens the middle one by one entry. In the section -1 case the flags are not read.
// Unknown: what the sub-lists contain, what feature bit 0x100 is, and the field names.
extern "C" {
unsigned VIGetFeatureFlags__Fv();
void ResetTickPulse__10VIUIWindow(void* window);
void SoundPlay__4VIUI11VIUISoundId(const void* ui, int sound);
extern unsigned _5VIWnd$mUI;

void Next__12VIWndOptions(void* self) {
    int* words = static_cast<int*>(self);
    const void* ui = reinterpret_cast<const void*>(static_cast<unsigned long>(_5VIWnd$mUI));
    int next;
    bool moved;
    if (words[286] == -1) {
        const int cursor = words[295];
        next = cursor + 1;
        moved = cursor < 3;
        if (moved) words[295] = next;
    } else {
        int limits[3] = {2, 3, 3};
        if (VIGetFeatureFlags__Fv() & 0x100) {
            limits[1] = 4;
        }
        const int cursor = words[287];
        next = cursor + 1;
        moved = cursor < limits[words[286]];
        if (moved) words[287] = next;
    }
    if (moved) {
        ResetTickPulse__10VIUIWindow(static_cast<char*>(self) + 40);
        SoundPlay__4VIUI11VIUISoundId(ui, 11);
    } else {
        SoundPlay__4VIUI11VIUISoundId(ui, 9);
    }
}
}
