// Observed: Prev__12VIWndOptions at 0x010c49b0 is 172 bytes. With this and the word at +1144 (call it the
// section): when the section is -1 and the word at +1180 is above zero, it subtracts one from +1180; when the
// section is not -1 and the word at +1148 is above zero, it subtracts one from +1148. In either successful case
// it calls ResetTickPulse__10VIUIWindow(this + 40) and then SoundPlay__4VIUI11VIUISoundId(mUI, 11); otherwise it
// only calls SoundPlay(mUI, 9). mUI is the word _5VIWnd$mUI (0x01192c70). Nothing is returned.
// Inferred: section -1 is the top-level list (cursor at +1180) and any other value a sub-list (cursor at +1148);
// the object holds a VIUIWindow at +40 whose tick pulse restarts when the cursor moves; 11 is the "moved" cue
// and 9 the "blocked" cue, as in the read-message window.
// Unknown: field names, what the section values 0..2 are, and the sound identities.
extern "C" {
void ResetTickPulse__10VIUIWindow(void* window);
void SoundPlay__4VIUI11VIUISoundId(const void* ui, int sound);
extern unsigned _5VIWnd$mUI;

void Prev__12VIWndOptions(void* self) {
    int* words = static_cast<int*>(self);
    const void* ui = reinterpret_cast<const void*>(static_cast<unsigned long>(_5VIWnd$mUI));
    if (words[286] == -1) {
        const int cursor = words[295];
        if (cursor > 0) {
            words[295] = cursor - 1;
            ResetTickPulse__10VIUIWindow(static_cast<char*>(self) + 40);
            SoundPlay__4VIUI11VIUISoundId(ui, 11);
        } else {
            SoundPlay__4VIUI11VIUISoundId(ui, 9);
        }
    } else {
        const int cursor = words[287];
        if (cursor > 0) {
            words[287] = cursor - 1;
            ResetTickPulse__10VIUIWindow(static_cast<char*>(self) + 40);
            SoundPlay__4VIUI11VIUISoundId(ui, 11);
        } else {
            SoundPlay__4VIUI11VIUISoundId(ui, 9);
        }
    }
}
}
