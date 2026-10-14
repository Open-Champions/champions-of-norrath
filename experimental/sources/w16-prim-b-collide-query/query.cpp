// Observed: Query__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer at 0x010971e0 is 104
// bytes. It has five pointer arguments (a0..a3 and t0); the fifth is the collision buffer. It reads
// the type word at offset 8 of that buffer and tail-jumps with all five arguments unchanged to
//   type 0: QueryBufferV__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer
//   type 1: QueryBufferPackV__...  (same mangled parameter list)
//   type 3: QueryBufferPackVGF__... (same mangled parameter list)
// and returns -1 without calling anything for any other type value. The callee's result is the
// result of this routine.
// Inferred: a dispatcher over the buffer layouts whose type numbers match the ones accepted by
// the VICollBuffer Vertex/End routines (0 float vertices, 1 and 3 packed 16-bit vertices).
// Unknown: what the "V", "PackV" and "PackVGF" suffixes abbreviate, and the callee return type.
extern "C" {
int QueryBufferV__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer(void*, const void*, const void*, void*, void*);
int QueryBufferPackV__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer(void*, const void*, const void*, void*, void*);
int QueryBufferPackVGF__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer(void*, const void*, const void*, void*, void*);

int Query__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer(void* self, const void* box, const void* box2, void* list, void* buffer) {
    switch (*reinterpret_cast<int*>(static_cast<unsigned char*>(buffer) + 8)) {
    case 0:
        return QueryBufferV__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer(self, box, box2, list, buffer);
    case 1:
        return QueryBufferPackV__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer(self, box, box2, list, buffer);
    case 3:
        return QueryBufferPackVGF__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer(self, box, box2, list, buffer);
    default:
        return -1;
    }
}
}
