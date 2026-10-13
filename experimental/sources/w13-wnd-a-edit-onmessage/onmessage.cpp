// Observed: OnMessage__9VIWndEditP5VIWndiii at 0x010ad7d0 is 184 bytes. With (this, sender, message, p1, p2):
// any message other than 295 is passed unchanged to OnMessage__5VIWndP5VIWndiii. For message 295 nothing
// happens unless p2 equals this. When it does: GetTime__7VITimer() is stored as 64 bits at +376; the words at
// +328, +332, +344, +348, +352, +360, +364, +384, +388, +44, +48 and +52 are cleared; +368 is set to 1;
// the byte sKeyMap[64] (sKeyMap at 0x01192b80, 66 bytes) is set to 6 when the word at +440 is nonzero and to
// 0 otherwise; and sCompactKeyboard (0x01192bc8) is set to 1. Message 295 never reaches the base class.
// Inferred: 295 resets the edit window's input state (held key, repeat timing, special-key map) when this
// window is the target; this reading comes from the clears only.
// Unknown: the meaning of message 295, of p1, and of the sKeyMap byte at 64; the owner of both statics;
// what the base handler returns (the 295 path leaves an unrelated value in the result register, so the
// result is treated as void).
extern "C" {
extern unsigned char sKeyMap[];
extern int sCompactKeyboard;
unsigned long long GetTime__7VITimer();
void OnMessage__5VIWndP5VIWndiii(void* self, void* sender, int message, int p1, void* p2);

void OnMessage__9VIWndEditP5VIWndiii(void* self, void* sender, int message, int p1, void* p2) {
    if (message != 295) {
        OnMessage__5VIWndP5VIWndiii(self, sender, message, p1, p2);
        return;
    }
    if (p2 != self) return;
    int* words = static_cast<int*>(self);
    const unsigned long long now = GetTime__7VITimer();
    words[92] = 1;
    words[87] = 0;
    words[86] = 0;
    words[13] = 0;
    words[96] = 0;
    words[83] = 0;
    words[82] = 0;
    *reinterpret_cast<unsigned long long*>(words + 94) = now;
    const int flag = words[110];
    words[90] = 0;
    words[91] = 0;
    words[88] = 0;
    words[97] = 0;
    sKeyMap[64] = flag ? 6 : 0;
    words[12] = 0;
    words[11] = 0;
    sCompactKeyboard = 1;
}
}
