// Observed: Next__16VIWndReadMessage at 0x010d1658 is 84 bytes. With this: if the word at +16800 is below
// (the word at +16796) - 7, it adds one to the word at +16800 and calls SoundPlay__4VIUI11VIUISoundId(mUI, 11);
// otherwise it leaves it unchanged and plays sound 9. mUI is the word _5VIWnd$mUI (0x01192c70).
// Inferred: +16800 is the first visible line or page position of the message text and +16796 a total line
// count, with seven lines shown at once (arithmetic only); sound 11 is the "moved" cue and 9 the "blocked" cue.
// Unknown: the names of the two fields, the unit of the count, and the sound identities.
extern "C" {
void SoundPlay__4VIUI11VIUISoundId(const void* ui, int sound);
extern unsigned _5VIWnd$mUI;

void Next__16VIWndReadMessage(void* self) {
    int* words = static_cast<int*>(self);
    const int position = words[4200];
    const int next = position + 1;
    if (position < words[4199] - 7) {
        words[4200] = next;
        SoundPlay__4VIUI11VIUISoundId(reinterpret_cast<const void*>(static_cast<unsigned long>(_5VIWnd$mUI)), 11);
    } else {
        SoundPlay__4VIUI11VIUISoundId(reinterpret_cast<const void*>(static_cast<unsigned long>(_5VIWnd$mUI)), 9);
    }
}
}
