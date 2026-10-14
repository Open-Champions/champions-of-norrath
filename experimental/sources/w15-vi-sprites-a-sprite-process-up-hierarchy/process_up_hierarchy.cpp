// Observed: ProcessUpHierarchy__9VIHSpritei at 0x01062d58, 388 bytes. Arguments (this, index). Nodes are 256-byte records in the array
// whose pointer is the word at this+72; e is node[index]. It keeps a zeroed local 4x4 matrix and a zeroed local three-float vector.
//   * When the signed word at e+0 is -1 it copies the three floats at e+112 into the local vector, multiplies each by the float at e+32,
//     and calls Transform__10VIMatrix44RC6VIQuatRC7VIVect3T2(e+48, e+16, vector, e+36).
//   * Otherwise it first calls ProcessUpHierarchy__9VIHSpritei(this, e[0]) recursively (result ignored), then builds the same scaled
//     vector and calls Transform(local matrix, e+16, vector, e+36), and finally Mul__10VIMatrix44RC10VIMatrix44T1(e+48, local matrix,
//     parent+48) where parent is node[e[0]].
// The local matrix is destroyed with _$_10VIMatrix44(matrix, 2) on both paths and the result is always zero.
// Inferred: e+0 is the parent node index (-1 for a root), e+16 a rotation, e+36 a translation, e+32 a scale applied to the vector at
// e+112, and e+48 the node's world matrix; so this brings every ancestor up to date before combining. Names are guesses from the call
// signatures only.
// Unknown: the roles of the vectors at e+112 and e+36 relative to each other, and the meaning of the local-matrix path in the root case.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void Transform__10VIMatrix44RC6VIQuatRC7VIVect3T2(void* matrix, const void* rotation, const void* scaled, const void* translation);
void Mul__10VIMatrix44RC10VIMatrix44T1(void* out, const void* a, const void* b);
void _$_10VIMatrix44(void* matrix, int flags);

int ProcessUpHierarchy__9VIHSpritei(void* object, int index) {
    alignas(16) float matrix[16] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    float vector[3] = {0.0f, 0.0f, 0.0f};
    unsigned char* nodes = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(static_cast<unsigned char*>(object) + 72));
    unsigned char* node = nodes + index * 256;
    int parent = *reinterpret_cast<int*>(node);
    if (parent == -1) {
        vector[0] = *reinterpret_cast<float*>(node + 112);
        vector[1] = *reinterpret_cast<float*>(node + 116);
        vector[2] = *reinterpret_cast<float*>(node + 120);
        float scale = *reinterpret_cast<float*>(node + 32);
        vector[0] = vector[0] * scale;
        vector[1] = vector[1] * scale;
        vector[2] = vector[2] * scale;
        Transform__10VIMatrix44RC6VIQuatRC7VIVect3T2(node + 48, node + 16, vector, node + 36);
    } else {
        ProcessUpHierarchy__9VIHSpritei(object, parent);
        unsigned char* parentNode = nodes + parent * 256;
        vector[0] = *reinterpret_cast<float*>(node + 112);
        vector[1] = *reinterpret_cast<float*>(node + 116);
        vector[2] = *reinterpret_cast<float*>(node + 120);
        float scale = *reinterpret_cast<float*>(node + 32);
        vector[0] = vector[0] * scale;
        vector[1] = vector[1] * scale;
        vector[2] = vector[2] * scale;
        Transform__10VIMatrix44RC6VIQuatRC7VIVect3T2(matrix, node + 16, vector, node + 36);
        Mul__10VIMatrix44RC10VIMatrix44T1(node + 48, matrix, parentNode + 48);
    }
    _$_10VIMatrix44(matrix, 2);
    return 0;
}
}
