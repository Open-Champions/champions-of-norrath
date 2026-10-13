// Observed: NewPlayTargetLightning__18VISpellEventDriverP12VISpellEvent at 0x0117eaf8 is 380 bytes in
// the pinned SLUS-20565 image. Arguments: (this, record). Always returns 0. e = the word at this+16
// (the effect driver, re-read for every loop test); flags = the int at record+4.
//   Loop i from 0 while i < (the int at e+16) + 1:
//   1. d = 0. When flags bit 0 is set and i == 0: d = AddSpellEventDriver__19VISpellEffectDriverP16
//      VISpellEventListi(e, the word at e + 24 added to 24, the int at e+12); flags is read again.
//   2. When flags bit 1 is set and i != 0: d = AddSpellEventDriver(e, (the word at e) + 36, the int at
//      e + 16 + 4 * i).
//   3. When the int at this+24 is not -1, d is non-null and the word at d+48 is zero: b =
//      __builtin_new(184); when non-null it calls __18VISpellActiveEventP12VISpellEventP7VIScene
//      (b, record, the word at this+20); stores b at d+48 and the int at this+24 (read again after
//      the construction) at b+140 (both stores happen even when b is null); then stores the 64-bit
//      GetTime__7VIScene(scene) at the word at d+48, plus 176.
//   The image also zero-initialises some stack floats that nothing reads; they are not reproduced.
// Inferred: record type 13 of the event table: iteration 0 targets the primary target of the effect
// driver (e+12, flag bit 0) and iterations 1..n the secondary targets (e+20.., flag bit 1); for each,
// a child event driver is created on the matching list (+24 or +36 of the table at *e), and an active
// event carrying the lightning record is hung on it at +48 (the slot that Process tests for
// ProcessLightningEvent) with this driver's actor handle, unless one is already there.
// Unknown: what the lightning is made of (the called routines only create the driver), the lists at
// *e+24 and *e+36, and why the loop runs one more time than the stored count. Nothing here ran on the
// EE.
extern "C" {
void* AddSpellEventDriver__19VISpellEffectDriverP16VISpellEventListi(void* effect, void* list, int id);
void* __builtin_new(unsigned size);
void __18VISpellActiveEventP12VISpellEventP7VIScene(void* self, void* event, void* scene);
long long GetTime__7VIScene(void* scene);

int NewPlayTargetLightning__18VISpellEventDriverP12VISpellEvent(unsigned char* self, unsigned char* record) {
    for (int i = 0; i < *reinterpret_cast<int*>(*reinterpret_cast<unsigned char**>(self + 16) + 16) + 1; ++i) {
        unsigned char* effect = *reinterpret_cast<unsigned char**>(self + 16);
        int flags = *reinterpret_cast<int*>(record + 4);
        unsigned char* driver = 0;
        if ((flags & 1) != 0 && i == 0) {
            driver = static_cast<unsigned char*>(AddSpellEventDriver__19VISpellEffectDriverP16VISpellEventListi(
                effect, *reinterpret_cast<unsigned char**>(effect) + 24, *reinterpret_cast<int*>(effect + 12)));
            flags = *reinterpret_cast<int*>(record + 4);
        }
        if ((flags & 2) != 0 && i != 0) {
            effect = *reinterpret_cast<unsigned char**>(self + 16);
            driver = static_cast<unsigned char*>(AddSpellEventDriver__19VISpellEffectDriverP16VISpellEventListi(
                effect, *reinterpret_cast<unsigned char**>(effect) + 36, *reinterpret_cast<int*>(effect + 16 + 4 * i)));
        }
        if (*reinterpret_cast<int*>(self + 24) != -1 && driver != 0 && *reinterpret_cast<unsigned*>(driver + 48) == 0) {
            unsigned char* b = static_cast<unsigned char*>(__builtin_new(184));
            if (b != 0) {
                __18VISpellActiveEventP12VISpellEventP7VIScene(b, record, *reinterpret_cast<void**>(self + 20));
            }
            *reinterpret_cast<unsigned char**>(driver + 48) = b;
            *reinterpret_cast<int*>(b + 140) = *reinterpret_cast<int*>(self + 24);
            long long now = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
            *reinterpret_cast<long long*>(*reinterpret_cast<unsigned char**>(driver + 48) + 176) = now;
        }
    }
    return 0;
}
}
