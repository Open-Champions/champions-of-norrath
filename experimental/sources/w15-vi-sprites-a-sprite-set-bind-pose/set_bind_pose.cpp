// Observed: SetBindPose__9VIHSprite at 0x01064520, 120 bytes. The vector at this+64 has its count at +0 (this+64) and its element pointer
// at +8 (this+72). For each index i from 0 while i < the count (re-read each pass) it takes the 256-byte element e = data + i*256 and
// calls Transform__10VIMatrix44RC6VIQuatfRC7VIVect3 with the matrix at e+48, the quaternion-typed argument at e+124, the float at e+140
// and the vector at e+144. It returns zero. No other state is read or written.
// Inferred: each node record holds its bind-pose rotation (e+124), a scalar (e+140) and a vector (e+144), and the call rebuilds the node's
// matrix from them; the argument roles come from the mangled callee name.
// Unknown: the remaining node fields and the meaning of the scalar.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void Transform__10VIMatrix44RC6VIQuatfRC7VIVect3(void* matrix, const void* rotation, float scalar, const void* vector);

int SetBindPose__9VIHSprite(void* object) {
    const unsigned* vector = reinterpret_cast<const unsigned*>(static_cast<unsigned char*>(object) + 64);
    for (int i = 0; i < static_cast<int>(vector[0]); ++i) {
        unsigned char* element = NORRATH_WORD_TO_POINTER(vector[2]) + i * 256;
        Transform__10VIMatrix44RC6VIQuatfRC7VIVect3(element + 48, element + 124, *reinterpret_cast<float*>(element + 140), element + 144);
    }
    return 0;
}
}
