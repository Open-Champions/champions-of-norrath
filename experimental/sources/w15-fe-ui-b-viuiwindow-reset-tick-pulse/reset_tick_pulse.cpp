// Observed: ResetTickPulse__10VIUIWindow at 0x0103f778, 40 bytes. It calls GetTime__7VITimer (no arguments;
// the 64-bit result comes back in v0) and stores the full 64-bit value at offset 352 of the window.
// Inferred: offset 352 is the time at which the blink pulse read by TickPulse__C10VIUIWindow restarts.
extern "C" {
long long GetTime__7VITimer();

void ResetTickPulse__10VIUIWindow(void* self) {
    *reinterpret_cast<long long*>(static_cast<char*>(self) + 352) = GetTime__7VITimer();
}
}
