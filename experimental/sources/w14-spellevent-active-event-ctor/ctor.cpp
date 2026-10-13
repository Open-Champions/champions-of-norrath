// Observed: __18VISpellActiveEventP12VISpellEventP7VIScene at 0x0117cdb8 is 368 bytes in the pinned
// SLUS-20565 image and returns nothing. Arguments: (this, event, scene). It calls __7VILNode(this)
// first (the base node constructor, an import here). Then it stores 0.0f into eight consecutive
// 12-byte float triples, +20 up to +116 (the image stores some of them from loops over two-element
// arrays and again straight, which is redundant); stores the event pointer at +116, the scene pointer
// at +16; zero at +120, +124 and +128; and -1 at +132, +140 and +144. Bytes +136 and +148 up to the
// 184-byte object size (the driver later stores a 64-bit time at +176) are not touched.
// Inferred: one live instance of a scripted spell event: +116 is its script record, +16 the scene
// it plays in, +120/+124 two particle-emitter pointers and +128 a motif blender (the ones the driver
// destructor tears down), +132/+140/+144 handles that start as "none"; the triples are vectors.
// Unknown: what the individual triples and +136 hold, and the base node layout. Nothing here ran on
// the EE.
extern "C" {
void __7VILNode(void* node);

void __18VISpellActiveEventP12VISpellEventP7VIScene(unsigned char* self, void* event, void* scene) {
    __7VILNode(self);
    float* triple = reinterpret_cast<float*>(self + 20);
    for (int i = 0; i < 8; ++i) {
        triple[0] = 0.0f;
        triple[1] = 0.0f;
        triple[2] = 0.0f;
        triple += 3;
    }
    unsigned* w = reinterpret_cast<unsigned*>(self);
    w[29] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(event));
    w[4] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(scene));
    w[36] = 0xffffffffu;
    w[30] = 0;
    w[31] = 0;
    w[32] = 0;
    w[33] = 0xffffffffu;
    w[35] = 0xffffffffu;
}
}
