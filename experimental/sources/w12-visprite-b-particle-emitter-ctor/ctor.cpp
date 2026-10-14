// Observed __17VIParticleEmitter at 0x010b6010, 440 bytes in the pinned SLUS-20565 image. With the
// object as this it does, in order:
//   1. __7VILNode(this); floats 0.0 at this+16, +20, +24 and at +28, +32, +36.
//   2. __10VIMatrix33(this+40); 64 bytes of float zero at this+96 (inline, no call).
//   3. __7VILList(this+160); __7VILList(this+176); __20VIParticleAttributes(this+188).
//   4. Float zero at this+916 through +975 (five three-float groups at +916, +928, +940, +952, +964);
//      word 0 at this+80, this+84 and this+172; float 0.0 at this+76.
//   5. Identity__10VIMatrix33(this+40); floats 0.0 again at this+16..+24 and this+28..+36; word 0 at
//      this+980 and this+984; Identity__10VIMatrix44(this+96).
// Nothing is returned.
// Inferred: the three-float groups are vectors, the 64 bytes at +96 a 4x4 matrix whose inline zeroing
// the later Identity call overwrites, the two lists at +160 and +176 12-byte list objects (the
// destructor offsets) and the repeated zero stores of +16..+36 a vector assignment.
// Unknown: the meaning of every field, and the size of the object. Nothing here ran on the EE.
extern "C" {
void __7VILNode(void* self);
void __10VIMatrix33(void* self);
void __7VILList(void* self);
void __20VIParticleAttributes(void* self);
void Identity__10VIMatrix33(void* self);
void Identity__10VIMatrix44(void* self);

void fn_17VIParticleEmitter(void* self) __asm__("__17VIParticleEmitter");
void fn_17VIParticleEmitter(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    float* v1 = reinterpret_cast<float*>(bytes + 16);
    float* v2 = reinterpret_cast<float*>(bytes + 28);
    float* m = reinterpret_cast<float*>(bytes + 96);
    __7VILNode(self);
    v1[0] = 0.0f; v1[1] = 0.0f; v1[2] = 0.0f;
    v2[0] = 0.0f; v2[1] = 0.0f; v2[2] = 0.0f;
    __10VIMatrix33(bytes + 40);
    for (int i = 0; i < 16; ++i) m[i] = 0.0f;
    __7VILList(bytes + 160);
    __7VILList(bytes + 176);
    __20VIParticleAttributes(bytes + 188);
    float* g = reinterpret_cast<float*>(bytes + 916);
    for (int i = 0; i < 15; ++i) g[i] = 0.0f;
    *reinterpret_cast<unsigned*>(bytes + 80) = 0;
    *reinterpret_cast<unsigned*>(bytes + 84) = 0;
    *reinterpret_cast<unsigned*>(bytes + 172) = 0;
    *reinterpret_cast<float*>(bytes + 76) = 0.0f;
    Identity__10VIMatrix33(bytes + 40);
    v1[0] = 0.0f; v1[1] = 0.0f; v1[2] = 0.0f;
    v2[0] = 0.0f; v2[1] = 0.0f; v2[2] = 0.0f;
    *reinterpret_cast<unsigned*>(bytes + 984) = 0;
    *reinterpret_cast<unsigned*>(bytes + 980) = 0;
    Identity__10VIMatrix44(bytes + 96);
}
}
