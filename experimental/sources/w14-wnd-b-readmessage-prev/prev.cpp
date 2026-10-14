// Observed: Prev__16VIWndReadMessage at 0x010d1608 is 72 bytes. With this: if the word at +16800 is above
// zero, it subtracts one and calls SoundPlay__4VIUI11VIUISoundId(mUI, 11); otherwise it leaves it unchanged
// and plays sound 9. mUI is the word _5VIWnd$mUI (0x01192c70).
// Inferred: the mirror of Next__16VIWndReadMessage: +16800 is a scroll position that cannot go below zero.
// Unknown: the field name and the sound identities.
extern "C" {
void SoundPlay__4VIUI11VIUISoundId(const void* ui, int sound);
extern unsigned _5VIWnd$mUI;

void Prev__16VIWndReadMessage(void* self) {
    int* words = static_cast<int*>(self);
    const int position = words[4200];
    if (position > 0) {
        words[4200] = position - 1;
        SoundPlay__4VIUI11VIUISoundId(reinterpret_cast<const void*>(static_cast<unsigned long>(_5VIWnd$mUI)), 11);
    } else {
        SoundPlay__4VIUI11VIUISoundId(reinterpret_cast<const void*>(static_cast<unsigned long>(_5VIWnd$mUI)), 9);
    }
}
}
