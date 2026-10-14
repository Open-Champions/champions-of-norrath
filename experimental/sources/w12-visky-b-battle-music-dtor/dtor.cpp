// Observed: _$_13VIBattleMusic at 0x0106de08 is 84 bytes. With (this, flags) it calls
// Free__13VIBattleMusic(this) and then, only if bit 0 of flags is set, tail-calls
// __builtin_delete(this). Nothing is returned.
extern "C" {
void Free__13VIBattleMusic(void* self);
void __builtin_delete(void* pointer);

void _$_13VIBattleMusic(void* self, int flags) {
    Free__13VIBattleMusic(self);
    if (flags & 1) __builtin_delete(self);
}
}
