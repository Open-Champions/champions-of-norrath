// Observed: AllocParticles__14VIEffectVolume at 0x01147960, 476 bytes. Offsets are bytes from the object. It calls Size__C6VIBBox with a
// three-float result buffer and the box at +8, then FreeParticles__14VIEffectVolume(this), then switches on the word at +36 (type T):
//  - T = 0: calls FreeParticles again and returns 0 (the word at +32 is not stored).
//  - T of 5 or more: stores 1 at +32 and returns 0 without allocating.
//  - T = 1, 2, 3, 4: value = ((x * y) * 0.39999998), (x * 0.5), (x * 20) and (x * 20), then times the float at +44, where x and y are the
//    first two result floats; the record size is 36, 32, 36 and 60 bytes. The count is that value converted to int (cvt.w.s), and 1 when
//    it is below 1. The count is stored at +84, __wrap_malloc(count * record size) at +88 (stored even when null). A null result returns -1
//    with +32 untouched. Otherwise memset(block, 0, the word at +84 * record size) runs, 1 is stored at +32, and it returns 0.
// Inferred: the types match the dispatch in Process__14VIEffectVolumef (1 falling leaves, 2 light shafts, 3 ground fog, 4 dust storm); the
// float at +44 is a density that scales the particle count with the volume's footprint (area for leaves, width for the others); the word at
// +32 flags that particles are allocated.
// Unknown: the record layouts, the +44 field's real name, and why type 0 frees twice.
extern "C" {
void Size__C6VIBBox(float* result, const void* box);
void FreeParticles__14VIEffectVolume(void* self);
void* __wrap_malloc(unsigned size);
void* memset(void* destination, int value, unsigned count);

int AllocParticles__14VIEffectVolume(unsigned char* self) {
    float size[3];
    Size__C6VIBBox(size, self + 8);
    FreeParticles__14VIEffectVolume(self);
    unsigned recordSize;
    float value;
    switch (*reinterpret_cast<unsigned*>(self + 36)) {
    case 0:
        FreeParticles__14VIEffectVolume(self);
        return 0;
    case 1:
        value = size[0] * size[1];
        value = value * 0x1.999998p-2f;
        recordSize = 36;
        break;
    case 2:
        value = size[0] * 0.5f;
        recordSize = 32;
        break;
    case 3:
        value = size[0] * 20.0f;
        recordSize = 36;
        break;
    case 4:
        value = size[0] * 20.0f;
        recordSize = 60;
        break;
    default:
        *reinterpret_cast<unsigned*>(self + 32) = 1;
        return 0;
    }
    value = value * *reinterpret_cast<float*>(self + 44);
    int count = static_cast<int>(value);
    if (count < 1) count = 1;
    *reinterpret_cast<int*>(self + 84) = count;
    void* block = __wrap_malloc(count * recordSize);
    *reinterpret_cast<unsigned*>(self + 88) = static_cast<unsigned>(reinterpret_cast<unsigned long>(block));
    if (block == nullptr) return -1;
    memset(block, 0, *reinterpret_cast<int*>(self + 84) * recordSize);
    *reinterpret_cast<unsigned*>(self + 32) = 1;
    return 0;
}
}
