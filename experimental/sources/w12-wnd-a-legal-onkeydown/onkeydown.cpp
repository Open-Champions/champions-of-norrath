// Observed: OnKeyDown__10VIWndLegali at 0x010bf988 is 92 bytes. With (this, key): nothing happens
// unless key is 144. For key 144 it reads GetTime__7VITimer() and subtracts the 64-bit word at
// this+40; if the low 32 bits of the difference, as a signed value, are below 5001 it returns.
// Otherwise it stores GetTime__7VITimer() - 7000 as the 64-bit word at this+40.
// Inferred: 144 is a key code; this+40 is a timestamp; the stored value backdates "now" by 7000
// time units, so a later check against 5001 soon succeeds (a countdown reset), units unknown.
// Unknown: the unit of the timer and the key's name.
extern "C" {
long long GetTime__7VITimer();

void OnKeyDown__10VIWndLegali(void* self, int key) {
    if (key != 144) return;
    long long* stamp = reinterpret_cast<long long*>(static_cast<unsigned char*>(self) + 40);
    const int elapsed = static_cast<int>(GetTime__7VITimer() - *stamp);
    if (elapsed < 5001) return;
    *stamp = GetTime__7VITimer() - 7000;
}
}
