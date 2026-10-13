// Observed: Back__12VIWndOptions at 0x010c63d0 is 80 bytes. With this it plays sound id 8 through
// SoundPlay__4VIUI11VIUISoundId(the word _5VIWnd$mUI, 8) in both paths. When the signed word at +1144 is
// non-negative it first stores -1 there and returns 0; when it is negative the word is left alone and the result is 1.
// Inferred: +1144 is a sub-menu index (-1 meaning the top level), so a result of 1 means "leave the options window"
// as the key handler uses it, and 0 means a sub-menu was closed.
// Unknown: the sound id name, the field name and the result type.
extern "C" {
extern unsigned _5VIWnd$mUI;
void SoundPlay__4VIUI11VIUISoundId(unsigned ui, int sound);

int Back__12VIWndOptions(void* self) {
    int* words = static_cast<int*>(self);
    if (words[286] >= 0) {
        words[286] = -1;
        SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 8);
        return 0;
    }
    SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 8);
    return 1;
}
}
