// Observed: OnKeyDown__16VIWndReadMessagei at 0x010d1508 is 148 bytes. With (this, key): key 138 tail-calls
// Prev__16VIWndReadMessage(this); key 139 tail-calls Next__16VIWndReadMessage(this); key 132 calls
// SoundPlay__4VIUI11VIUISoundId(mUI, 8) with mUI the word _5VIWnd$mUI (0x01192c70), clears the word at +28,
// and calls SetFocus__5VIWndP5VIWndb(this, the word at +20, 1). Every other key does nothing.
// Inferred: 138 and 139 step to the previous and next page; 132 closes by returning focus to the word at
// +20 (a parent pointer is a guess from SetFocus's argument type).
// Unknown: the key names; what the word at +28 is; the return type (void).
extern "C" {
void Prev__16VIWndReadMessage(void* self);
void Next__16VIWndReadMessage(void* self);
void SoundPlay__4VIUI11VIUISoundId(const void* ui, int sound);
void SetFocus__5VIWndP5VIWndb(void* self, void* target, bool flag);
extern unsigned _5VIWnd$mUI;

void OnKeyDown__16VIWndReadMessagei(void* self, int key) {
    int* words = static_cast<int*>(self);
    if (key == 138) {
        Prev__16VIWndReadMessage(self);
    } else if (key == 139) {
        Next__16VIWndReadMessage(self);
    } else if (key == 132) {
        SoundPlay__4VIUI11VIUISoundId(reinterpret_cast<const void*>(static_cast<unsigned long>(_5VIWnd$mUI)), 8);
        words[7] = 0;
        SetFocus__5VIWndP5VIWndb(self, reinterpret_cast<void*>(static_cast<unsigned long>(words[5])), true);
    }
}
}
