// Observed: UnlinkAllMotifBlenders__17VIParticleEmitter at 0x010b8cc8 is 48 bytes. It
// repeatedly calls RemHead__7VILList with the list at offset 176 of the emitter. For every
// non-null node returned it stores zero at offset 16 of that node, and it stops when
// RemHead returns null.
// Inferred: the list at 176 holds motif blenders (see _$_14VIMotifBlender, whose word at 16
// is non-zero while linked) and the loop marks each as unlinked.
// Unknown: whether a blender's word at 16 is anything other than a linked flag.
extern "C" {
void* RemHead__7VILList(void* list);

void UnlinkAllMotifBlenders__17VIParticleEmitter(unsigned char* emitter) {
    void* node;
    while ((node = RemHead__7VILList(emitter + 176)) != 0) {
        *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(node) + 16) = 0;
    }
}
}
