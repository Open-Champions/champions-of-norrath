// Observed: Free__13VIBattleMusic at 0x0106de98 is 64 bytes. When the word at byte offset 0 of
// the battle-music object (the scene pointer set by Init) is nonzero it calls
// Unload__13VIBattleMusic(this) and then stores -1 at +4, zero at +0 and zero at +12, in that
// order. When the word is zero it does nothing. Nothing is returned.
// Inferred: this undoes Init. Unknown: what Unload does and the member names.
extern "C" {
void Unload__13VIBattleMusic(unsigned char* self);

void Free__13VIBattleMusic(unsigned char* self) {
    if (*reinterpret_cast<const unsigned*>(self + 0) == 0) return;
    Unload__13VIBattleMusic(self);
    *reinterpret_cast<int*>(self + 4) = -1;
    *reinterpret_cast<unsigned*>(self + 0) = 0;
    *reinterpret_cast<unsigned*>(self + 12) = 0;
}
}
