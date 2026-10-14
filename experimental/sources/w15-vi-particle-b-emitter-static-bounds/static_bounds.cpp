// Observed: StaticBounds__C17VIParticleEmitter at 0x010b7308, 396 bytes. It returns a VIBBox (two
// vectors of three floats, 24 bytes) through a hidden first argument, which is also the result
// register; the emitter is the second argument. Offsets are bytes from the emitter.
//  1. A local box and a local array of eight vectors are set to zero.
//  2. Store__C6VIBBoxP7VIVect3(emitter + 916, corners) fills the eight corners from the box member
//     at +916.
//  3. For i from 0 to 7: __ml__FRC7VIVect3RC10VIMatrix33(temp, corner[i], emitter + 40) rotates the
//     corner by the matrix at +40; corner[i] becomes temp plus the vector at emitter + 16 (x, y, z
//     added componentwise); then Bound__6VIBBoxiRC7VIVect3(local box, i == 0, corner[i]) runs.
//  4. The result slot is zeroed and then receives the six floats of the local box.
// Inferred: this is the world-space bounding box of the emitter's static box, with Location at +16
// and the matrix at +40 (the accessors Location and Direction return those addresses); the flag
// starts the box on the first corner. Unknown: the member names, whether the matrix is a plain
// orientation, and why the image zeroes the result before copying.
extern "C" {
int Store__C6VIBBoxP7VIVect3(const float* box, float* out);
void __ml__FRC7VIVect3RC10VIMatrix33(void* result, const void* vector, const void* matrix);
void Bound__6VIBBoxiRC7VIVect3(float* box, int first, const float* point);

void* StaticBounds__C17VIParticleEmitter(float* result, const void* emitter) {
    const unsigned char* e = static_cast<const unsigned char*>(emitter);
    float box[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    float corners[8][3];
    for (int i = 0; i < 8; ++i) {
        corners[i][0] = 0.0f;
        corners[i][1] = 0.0f;
        corners[i][2] = 0.0f;
    }
    Store__C6VIBBoxP7VIVect3(reinterpret_cast<const float*>(e + 916), &corners[0][0]);
    const float* location = reinterpret_cast<const float*>(e + 16);
    for (int i = 0; i < 8; ++i) {
        float rotated[3];
        __ml__FRC7VIVect3RC10VIMatrix33(rotated, corners[i], e + 40);
        corners[i][0] = rotated[0] + location[0];
        corners[i][1] = rotated[1] + location[1];
        corners[i][2] = rotated[2] + location[2];
        Bound__6VIBBoxiRC7VIVect3(box, i == 0, corners[i]);
    }
    for (int i = 0; i < 6; ++i) result[i] = 0.0f;
    for (int i = 0; i < 6; ++i) result[i] = box[i];
    return result;
}
}
