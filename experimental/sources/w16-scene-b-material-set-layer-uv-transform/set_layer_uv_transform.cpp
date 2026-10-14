// Observed: SetLayerUVTransform__10VIMaterialiRC10VIMatrix33 at 0x010cfa50 is 76
// bytes. It calls the genuine VIMatrix33 assignment operator with destination
// material + 64 + 144 * layer and the source matrix pointer, stores 1 in the word at
// offset 32 of the material, and returns 0.
// Inferred: each 144-byte layer record holds a matrix at +16 inside the record
// (record base is material + 48). Unknown: the matrix size beyond the assignment
// operator, and the reader.
extern "C" {
void* __as__10VIMatrix33RC10VIMatrix33(void* self, const void* other);

int SetLayerUVTransform__10VIMaterialiRC10VIMatrix33(unsigned char* material, int layer, const void* matrix) {
    __as__10VIMatrix33RC10VIMatrix33(material + layer * 144 + 64, matrix);
    *reinterpret_cast<int*>(material + 32) = 1;
    return 0;
}
}
