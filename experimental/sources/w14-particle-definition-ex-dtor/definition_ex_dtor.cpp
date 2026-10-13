// Observed: _$_22VIParticleDefinitionEx at 0x010b4c08 is 92 bytes. It calls the destructor
// of the VIPool<VIParticleMotif*> at offset 724 of the object (_$_t6VIPool1ZP15VIParticleMotif)
// with a flag argument of 2, then, when bit 0 of its own second argument is set, calls
// __builtin_delete with the object. The object bytes are otherwise left unchanged.
// Inferred: a VIParticleDefinitionEx embeds the motif pool at offset 724 and, since it
// extends VIParticleDefinition (whose destructor is not called here), the base destructor
// is not part of this function.
// Unknown: why the base destructor is not invoked here, and the pool destructor's meaning
// of the flag 2.
extern "C" {
void _$_t6VIPool1ZP15VIParticleMotif(void* pool, int flags);
void __builtin_delete(void* pointer);

void _$_22VIParticleDefinitionEx(unsigned char* definition, int flags) {
    _$_t6VIPool1ZP15VIParticleMotif(definition + 724, 2);
    if ((flags & 1) != 0) __builtin_delete(definition);
}
}
