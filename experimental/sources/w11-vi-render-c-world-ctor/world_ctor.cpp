// Observed __7VIWorld at 0x010fa9b8, 280 bytes. With the world as this:
//   1. Floats 0.0 at this + 0, 4, 8 (twice in the original), this + 12, 16, 20 and this + 36, 40, 44.
//   2. __11VIWorldTree(this + 52); __t7VIArray1Z6VIZone(this + 72); __t7VIArray1Z13VIWorldRegion(this + 80).
//   3. For i = 0 .. 21: floats 0.0 at this + 96 + 40 * i, +100, +104 and +108 (the forty-byte
//      stride elements start at this + 88; the first eight bytes of each are left alone).
//   4. Then: -1000.0f at this + 0, 4 and 8; the word 4 at this + 48; 1000.0f at this + 12, 16, 20;
//      the word 1 at this + 28; 2000.0f at this + 32; the word 1 at this + 24; 0.0f at this + 36, 40, 44.
// Nothing is returned. The double zero stores of step 1 are duplicated member initialisations in
// the original; only their effect is reproduced. Unknown: what the members mean (the two
// triples look like a bounding box, the rest are unnamed).
extern "C" {
void __11VIWorldTree(void* tree);
void __t7VIArray1Z6VIZone(void* array);
void __t7VIArray1Z13VIWorldRegion(void* array);

void __7VIWorld(void* self) {
    char* base = static_cast<char*>(self);
    float* a = reinterpret_cast<float*>(base + 0);
    float* b = reinterpret_cast<float*>(base + 12);
    float* c = reinterpret_cast<float*>(base + 36);
    a[0] = a[1] = a[2] = 0.0f;
    b[0] = b[1] = b[2] = 0.0f;
    c[0] = c[1] = c[2] = 0.0f;
    __11VIWorldTree(base + 52);
    __t7VIArray1Z6VIZone(base + 72);
    __t7VIArray1Z13VIWorldRegion(base + 80);
    for (int i = 0; i < 22; ++i) {
        float* element = reinterpret_cast<float*>(base + 96 + 40 * i);
        element[0] = element[1] = element[2] = element[3] = 0.0f;
    }
    a[0] = a[1] = a[2] = -1000.0f;
    *reinterpret_cast<int*>(base + 48) = 4;
    b[0] = b[1] = b[2] = 1000.0f;
    *reinterpret_cast<int*>(base + 28) = 1;
    *reinterpret_cast<float*>(base + 32) = 2000.0f;
    *reinterpret_cast<int*>(base + 24) = 1;
    c[0] = c[1] = c[2] = 0.0f;
}
}
