// Observed: _$_17VIParticleEmitter at 0x010b8a30 is 140 bytes in the pinned SLUS-20565 image.
// With (this, flags) it calls Destroy__17VIParticleEmitter(this), then calls
// _$_20VIParticleAttributes(this+188, 2), then calls _$_7VILList(this+176, 2), then calls
// _$_7VILList(this+160, 2), then calls _$_10VIMatrix44(this+96, 2), then calls
// _$_10VIMatrix33(this+40, 2), and when bit 0 of flags is set tail-calls __builtin_delete(this).
// Nothing is returned.
// Unknown: what Destroy does. Nothing here ran on the EE.
extern "C" {
void Destroy__17VIParticleEmitter(void* self);
void _$_20VIParticleAttributes(void* self, int flags);
void _$_7VILList(void* self, int flags);
void _$_10VIMatrix44(void* self, int flags);
void _$_10VIMatrix33(void* self, int flags);
void __builtin_delete(void* pointer);

void fn_17VIParticleEmitter(void* self, int flags) __asm__("_$_17VIParticleEmitter");
void fn_17VIParticleEmitter(void* self, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    Destroy__17VIParticleEmitter(self);
    _$_20VIParticleAttributes(bytes + 188, 2);
    _$_7VILList(bytes + 176, 2);
    _$_7VILList(bytes + 160, 2);
    _$_10VIMatrix44(bytes + 96, 2);
    _$_10VIMatrix33(bytes + 40, 2);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
