// Observed: _$_14VIMotifBlender at 0x010b8690 is 92 bytes. If the word at offset 16 of the
// blender is non-zero it calls Remove__7VILNode with the blender. Bit 0 of the second
// argument then selects __builtin_delete of the blender. The blender bytes are otherwise
// left unchanged.
// Inferred: the blender starts with a VILNode and the word at 16 is non-zero while it is
// linked into a list (UnlinkAllMotifBlenders__17VIParticleEmitter clears that word).
// Unknown: what else a blender owns.
extern "C" {
void Remove__7VILNode(void* node);
void __builtin_delete(void* pointer);

void _$_14VIMotifBlender(unsigned char* blender, int flags) {
    if (*reinterpret_cast<unsigned*>(blender + 16) != 0) Remove__7VILNode(blender);
    if ((flags & 1) != 0) __builtin_delete(blender);
}
}
