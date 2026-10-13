// Observed: OnMessage__10VIWndLegalP5VIWndiii at 0x010bf9f0 is 60 bytes. With (this, from, message, a,
// b): when message is 295 and the fifth argument b equals this, it calls GetTime__7VITimer (no
// arguments, a 64-bit result) and stores the result as a doubleword at byte offset 40 of this.
// For any other message, or when b differs from this, it does nothing. It returns nothing.
// 295 is the message SetFocus sends with (previous focus, 295, previous focus, new focus), so b is the
// window receiving focus.
// Inferred: the legal-notice window records the time at which it gains focus.
// Unknown: how the stored time is read back; whether the original signature has a fifth word.
extern "C" {
long long GetTime__7VITimer();

void OnMessage__10VIWndLegalP5VIWndiii(void* self, void*, int message, int, int b) {
    if (message == 295 && b == static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(self)))
        *reinterpret_cast<long long*>(static_cast<unsigned char*>(self) + 40) = GetTime__7VITimer();
}
}
