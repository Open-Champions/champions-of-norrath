// Observed: UpdateParticleEmitters__9VICSpriteR8VICameraR7VISceneR8VIRasterR10VIMatrix44 at 0x010b0280 is 968 bytes in the pinned
// SLUS-20565 image. Arguments: the sprite (this), a camera, a scene, a raster (never used) and a matrix. It builds two zeroed 64-byte
// VIMatrix44 locals and a zero 3-float vector (inlined constructors) and returns 0 after destroying both matrices with
// _$_10VIMatrix44(matrix, 2). Nothing else happens unless the 64-bit word at sprite +4608 differs from GetTime__7VIScene(scene)
// (called twice: once for the test, once more for the value stored back). In that case:
//  - Mul__10VIMatrix44RC10VIMatrix44T1(B, argument matrix, camera + 176) fills the second local; then
//    CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(sprite, B, scene, A) fills the first local.
//  - The emitter array sits at sprite +4616 (count at +0, data pointer at +4, entries of 1056 bytes). For each entry, in order, with
//    the count re-read every iteration: the entry is skipped when its flag word at +1012 has bit 0 set and the sprite word at +400 has
//    bit 2 clear. Otherwise Mul(B, node matrix + 48, A) is called, where the node matrix is in the 256-byte entry whose index is the
//    entry word at +1016, in the array whose data pointer is the word at sprite +72. The translation floats of B (at
//    +48, +52, +56) are copied to the 3-float vector, and when the squared distance to the camera floats at +28, +32, +36 exceeds
//    10000.0f the emitter (entry + 16) is Purge__17VIParticleEmitter'd and the entry is done.
//  - Otherwise SetLocation__17VIParticleEmitterRC7VIVect3(emitter, vector) is called if the entry word at +1020 is non-zero, or with
//    a zero vector if it is zero. Then, if the entry word at +1020 is non-zero, a VIMatrix33 is constructed (__10VIMatrix33), its nine
//    floats are set from the 3x3 upper-left part of B (rows of B at floats 0-2, 4-6, 8-10), SetDirection(emitter, matrix33) is called
//    and the matrix is destroyed (_$_10VIMatrix33(matrix, 2)); if it is zero, SetDirection(emitter, (0, 0, 1.0f)) and
//    SetSpace__17VIParticleEmitterRC10VIMatrix44(emitter, B) are called instead.
//  - Finally Update__17VIParticleEmitter(emitter) is called, except when the entry word at +1008 equals 1 and the signed 64-bit sum
//    of the entry's 64-bit word at +1040 and its word at +1032 (sign-extended) is less than GetTime(scene), which skips it.
// Inferred: the emitters follow skeleton nodes of the sprite, are culled by distance to the camera (limit 100 units, squared 10000), and +1008 == 1 marks emitters with an expiry time (start at +1040, duration at +1032).
// Unknown: the layouts of the sprite, camera and entries, the meaning of the flag words, and the callees' behaviour (imports of this
// unit). Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
long long GetTime__7VIScene(unsigned char* scene);
void Mul__10VIMatrix44RC10VIMatrix44T1(unsigned char* self, const unsigned char* first, const unsigned char* second);
void CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(unsigned char* self, unsigned char* matrix, unsigned char* scene, unsigned char* out);
void Purge__17VIParticleEmitter(unsigned char* emitter);
void SetLocation__17VIParticleEmitterRC7VIVect3(unsigned char* emitter, const unsigned char* point);
void SetDirection__17VIParticleEmitterRC7VIVect3(unsigned char* emitter, const unsigned char* vector);
void SetDirection__17VIParticleEmitterRC10VIMatrix33(unsigned char* emitter, const unsigned char* matrix);
void SetSpace__17VIParticleEmitterRC10VIMatrix44(unsigned char* emitter, const unsigned char* matrix);
void Update__17VIParticleEmitter(unsigned char* emitter);
void __10VIMatrix33(unsigned char* matrix);
void _$_10VIMatrix33(unsigned char* matrix, int flags);
void _$_10VIMatrix44(unsigned char* matrix, int flags);
int UpdateParticleEmitters__9VICSpriteR8VICameraR7VISceneR8VIRasterR10VIMatrix44(unsigned char* self, unsigned char* camera, unsigned char* scene, unsigned char* raster, unsigned char* matrix) {
    (void)raster;
    alignas(16) float a[16];
    alignas(16) float b[16];
    float position[3];
    for (int i = 0; i < 16; ++i) a[i] = 0.0f;
    for (int i = 0; i < 16; ++i) b[i] = 0.0f;
    position[0] = 0.0f; position[1] = 0.0f; position[2] = 0.0f;
    unsigned char* A = reinterpret_cast<unsigned char*>(a);
    unsigned char* B = reinterpret_cast<unsigned char*>(b);
    if (GetTime__7VIScene(scene) != *reinterpret_cast<long long*>(self + 4608)) {
        *reinterpret_cast<long long*>(self + 4608) = GetTime__7VIScene(scene);
        unsigned char* array = self + 4616;
        Mul__10VIMatrix44RC10VIMatrix44T1(B, matrix, camera + 176);
        CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(self, B, scene, A);
        if (*reinterpret_cast<int*>(array) > 0) {
            int offset = 0;
            int i = 0;
            do {
                unsigned char* entry = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(array + 4)) + offset;
                if (!(*reinterpret_cast<unsigned*>(entry + 1012) & 1) || (*reinterpret_cast<unsigned*>(self + 400) & 4)) {
                    unsigned char* nodes = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 72));
                    Mul__10VIMatrix44RC10VIMatrix44T1(B, nodes + (*reinterpret_cast<int*>(entry + 1016) << 8) + 48, A);
                    const float* cam = reinterpret_cast<const float*>(camera + 28);
                    position[0] = b[12];
                    float dx = b[12] - cam[0];
                    float dy = b[13] - cam[1];
                    position[1] = b[13];
                    float dz = b[14] - cam[2];
                    position[2] = b[14];
                    if (10000.0f < dx * dx + dy * dy + dz * dz) {
                        Purge__17VIParticleEmitter(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(array + 4)) + offset + 16);
                    } else {
                        if (*reinterpret_cast<int*>(entry + 1020) != 0) {
                            SetLocation__17VIParticleEmitterRC7VIVect3(entry + 16, reinterpret_cast<unsigned char*>(position));
                        } else {
                            float zero[3];
                            zero[0] = 0.0f; zero[1] = 0.0f; zero[2] = 0.0f;
                            SetLocation__17VIParticleEmitterRC7VIVect3(entry + 16, reinterpret_cast<unsigned char*>(zero));
                        }
                        entry = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(array + 4)) + offset;
                        if (*reinterpret_cast<int*>(entry + 1020) != 0) {
                            alignas(4) float m33[9];
                            __10VIMatrix33(reinterpret_cast<unsigned char*>(m33));
                            m33[0] = b[0]; m33[1] = b[1]; m33[2] = b[2];
                            m33[3] = b[4]; m33[4] = b[5]; m33[5] = b[6];
                            m33[6] = b[8]; m33[7] = b[9]; m33[8] = b[10];
                            SetDirection__17VIParticleEmitterRC10VIMatrix33(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(array + 4)) + offset + 16, reinterpret_cast<unsigned char*>(m33));
                            _$_10VIMatrix33(reinterpret_cast<unsigned char*>(m33), 2);
                        } else {
                            float direction[3];
                            direction[0] = 0.0f; direction[1] = 0.0f; direction[2] = 1.0f;
                            SetDirection__17VIParticleEmitterRC7VIVect3(entry + 16, reinterpret_cast<unsigned char*>(direction));
                            SetSpace__17VIParticleEmitterRC10VIMatrix44(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(array + 4)) + offset + 16, B);
                        }
                        entry = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(array + 4)) + offset;
                        bool expired = false;
                        if (*reinterpret_cast<int*>(entry + 1008) == 1) {
                            long long end = *reinterpret_cast<long long*>(entry + 1040) + *reinterpret_cast<int*>(entry + 1032);
                            expired = end < GetTime__7VIScene(scene);
                        }
                        if (!expired) Update__17VIParticleEmitter(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(array + 4)) + offset + 16);
                    }
                }
                i++;
                offset += 1056;
            } while (i < *reinterpret_cast<int*>(array));
        }
    }
    _$_10VIMatrix44(B, 2);
    _$_10VIMatrix44(A, 2);
    return 0;
}
}
