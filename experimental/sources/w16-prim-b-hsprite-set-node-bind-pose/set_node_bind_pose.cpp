// Observed: SetNodeBindPose__9VIHSpriteiRC6VIQuatfRC7VIVect3 at 0x0105fa90 is 528 bytes.
// Arguments: the sprite (a0), a node index n (a1), a quaternion q (a2), a float s (float register
// 12) and a vector t (a3). The sprite's vector at offset 64 has its element pointer at offset 72;
// the 256-byte element e is that pointer + n * 256. Two 64-byte local matrices m0 and m1 are
// built first: each is zero filled by four-float stores and then zero filled again by a VU0
// quadword sequence (vsub of a register from itself, four sqc2 stores). Then, in order:
//   1. e[124..139] = the 16 bytes of q (copied with unaligned doubleword pairs); e[140] = s;
//      e[144], e[148], e[152] = the three floats of t.
//   2. Transform__10VIMatrix44RC6VIQuatfRC7VIVect3(m0, q, s, t)   (q and t are the arguments)
//   3. Invert__10VIMatrix44RC10VIMatrix44(e + 160, m0)
//   4. With p the word at e[0]: if p != -1, Mul__10VIMatrix44RC10VIMatrix44T1(m1, m0, parent + 160)
//      where parent is the 256-byte element p (so the product uses the parent's inverse at +160),
//      then ConvertTo__C10VIMatrix44R6VIQuatRfR7VIVect3(m1, e + 224, e + 240, e + 244), with e
//      re-read from the sprite after the earlier calls. If p == -1, e[224..239] = the 16 bytes of
//      q, e[240] = s and e[244..252] = the three floats of t, each copied as in step 1.
//   5. The matrix destructor _$_10VIMatrix44(m1, 2) then _$_10VIMatrix44(m0, 2).
// It returns 0.
// Inferred: element offset 0 is the parent index (-1 for a root); e + 160 holds the node's
// inverse bind matrix and e + 224/240/244 the pose relative to the parent (rotation, scale,
// translation), per the callee names. Offsets 124..152 are the node's own pose, matching
// SetBindPose__9VIHSprite. Unknown: the remaining element fields, the precise operand order of
// the matrix product, and the argument roles of ConvertTo (taken from its mangled name).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

struct Raw64 {
    unsigned long long value;
} __attribute__((packed));

extern "C" {
void Transform__10VIMatrix44RC6VIQuatfRC7VIVect3(float* matrix, const float* quat, float scale, const float* vec);
void Invert__10VIMatrix44RC10VIMatrix44(float* result, const float* source);
void Mul__10VIMatrix44RC10VIMatrix44T1(float* result, const float* a, const float* b);
void ConvertTo__C10VIMatrix44R6VIQuatRfR7VIVect3(const float* matrix, float* quat, float* scale, float* vec);
void _$_10VIMatrix44(float* matrix, int flags);
}

namespace {
inline void zeroMatrix(float* m) {
    for (int i = 0; i < 4; ++i) {
        m[4 * i] = 0.0f;
        m[4 * i + 1] = 0.0f;
        m[4 * i + 2] = 0.0f;
        m[4 * i + 3] = 0.0f;
    }
#if defined(__mips__)
    __asm__ volatile(
        "vsub.xyzw $vf4,$vf0,$vf0\n\t"
        "sqc2 $vf4,48(%0)\n\t"
        "sqc2 $vf4,32(%0)\n\t"
        "sqc2 $vf4,16(%0)\n\t"
        "sqc2 $vf4,0(%0)"
        :
        : "r"(m)
        : "memory");
#else
    for (int i = 0; i < 16; ++i)
        m[i] = 0.0f;
#endif
}

inline void copyPose(unsigned char* element, int offset, const unsigned char* quat, float scale, const float* vec) {
    reinterpret_cast<Raw64*>(element + offset)[0] = reinterpret_cast<const Raw64*>(quat)[0];
    reinterpret_cast<Raw64*>(element + offset)[1] = reinterpret_cast<const Raw64*>(quat)[1];
    *reinterpret_cast<float*>(element + offset + 16) = scale;
    float* out = reinterpret_cast<float*>(element + offset + 20);
    out[0] = vec[0];
    out[1] = vec[1];
    out[2] = vec[2];
}
}

extern "C" {
int SetNodeBindPose__9VIHSpriteiRC6VIQuatfRC7VIVect3(unsigned char* self, int node, const unsigned char* quat,
                                                    float scale, const float* vec) {
    alignas(16) float m0[16];
    alignas(16) float m1[16];
    zeroMatrix(m0);
    zeroMatrix(m1);
    unsigned* elements = reinterpret_cast<unsigned*>(self + 64);
    unsigned char* e = at(elements[2]) + node * 256;

    // step 1: the node's own pose (quaternion 124..139, scale 140, vector 144..152)
    reinterpret_cast<Raw64*>(e + 124)[0] = reinterpret_cast<const Raw64*>(quat)[0];
    reinterpret_cast<Raw64*>(e + 124)[1] = reinterpret_cast<const Raw64*>(quat)[1];
    e = at(elements[2]) + node * 256;
    *reinterpret_cast<float*>(e + 140) = scale;
    float* pos = reinterpret_cast<float*>(e + 144);
    pos[0] = vec[0];
    pos[1] = vec[1];
    pos[2] = vec[2];

    Transform__10VIMatrix44RC6VIQuatfRC7VIVect3(m0, reinterpret_cast<const float*>(quat), scale, vec);
    Invert__10VIMatrix44RC10VIMatrix44(reinterpret_cast<float*>(at(elements[2]) + node * 256 + 160), m0);

    unsigned char* base = at(elements[2]);
    unsigned char* element = base + node * 256;
    int parent = *reinterpret_cast<int*>(element);
    if (parent != -1) {
        Mul__10VIMatrix44RC10VIMatrix44T1(m1, m0, reinterpret_cast<float*>(base + parent * 256 + 160));
        unsigned char* again = at(elements[2]) + node * 256;
        ConvertTo__C10VIMatrix44R6VIQuatRfR7VIVect3(m1, reinterpret_cast<float*>(again + 224),
                                                    reinterpret_cast<float*>(again + 240),
                                                    reinterpret_cast<float*>(again + 244));
    } else {
        copyPose(element, 224, quat, scale, vec);
    }
    _$_10VIMatrix44(m1, 2);
    _$_10VIMatrix44(m0, 2);
    return 0;
}
}
