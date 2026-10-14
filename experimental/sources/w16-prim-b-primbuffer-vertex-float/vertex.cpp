// Observed: Vertex__12VIPrimBufferR7VIVect3 at 0x011014b0 is 616 bytes. If the type word at offset
// 8 is not 0 it returns -1 and changes nothing. Otherwise, with e the 28-byte entry at (pointer at
// offset 92) + (word at offset 72) * 28 and v the 16-byte slot at (pointer at offset 96) + (word at
// offset 80) * 16, it adds one to the word at e offset 0 and then writes four consecutive
// 16-byte slots, advancing the word at offset 80 by one after each:
//   slot 0: the argument's x, y, z, then 1.0f;
//   slot 1: the two floats of _12VIPrimBuffer$PrimUV0 (0x011b0110), then 1.0f, then 0.0f;
//   slot 2: the three floats of _12VIPrimBuffer$PrimNormal (0x011b0130), then 0.0f;
//   slot 3: the four bytes of _12VIPrimBuffer$PrimColor (0x011b0148), each converted to float and
//           multiplied by the constant with bits 0x3b808080 (one ulp below the float nearest 1/255).
// It returns 0. Only the loaded word values are used for indexing, so overlapping memory between
// the object and its arrays is not modelled.
// Inferred: each call appends one logical vertex made of position, texture, normal and colour
// slots in float layout; the entry word counts logical vertices while the slot index counts slots.
// Unknown: the consumer of the entry/slot arrays, and what the second-slot constant 1.0f and the
// trailing 0.0f/1.0f lanes represent.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern float PrimUV0[2] __asm__("_12VIPrimBuffer$PrimUV0");
extern float PrimNormal[3] __asm__("_12VIPrimBuffer$PrimNormal");
extern unsigned char PrimColor[4] __asm__("_12VIPrimBuffer$PrimColor");

extern "C" {
int Vertex__12VIPrimBufferR7VIVect3(unsigned char* self, const float* v) {
    if (*reinterpret_cast<int*>(self + 8) != 0)
        return -1;
    int* count = reinterpret_cast<int*>(at(*reinterpret_cast<unsigned*>(self + 92)) + *reinterpret_cast<int*>(self + 72) * 28);
    *count = *count + 1;
    int* index = reinterpret_cast<int*>(self + 80);
    unsigned char* slots = at(*reinterpret_cast<unsigned*>(self + 96));
    float* s = reinterpret_cast<float*>(slots + *index * 16);
    s[0] = v[0];
    s[1] = v[1];
    s[2] = v[2];
    s[3] = 1.0f;
    *index = *index + 1;

    s = reinterpret_cast<float*>(at(*reinterpret_cast<unsigned*>(self + 96)) + *index * 16);
    s[0] = PrimUV0[0];
    s[1] = PrimUV0[1];
    s[2] = 1.0f;
    s[3] = 0.0f;
    *index = *index + 1;

    s = reinterpret_cast<float*>(at(*reinterpret_cast<unsigned*>(self + 96)) + *index * 16);
    s[0] = PrimNormal[0];
    s[1] = PrimNormal[1];
    s[2] = PrimNormal[2];
    s[3] = 0.0f;
    *index = *index + 1;

    const float scale = 0x1.010100p-8f;
    float c[4];
    c[0] = static_cast<float>(PrimColor[0]) * scale;
    c[1] = static_cast<float>(PrimColor[1]) * scale;
    c[2] = static_cast<float>(PrimColor[2]) * scale;
    c[3] = static_cast<float>(PrimColor[3]) * scale;
    s = reinterpret_cast<float*>(at(*reinterpret_cast<unsigned*>(self + 96)) + *index * 16);
    s[0] = c[0];
    s[1] = c[1];
    s[2] = c[2];
    s[3] = c[3];
    *index = *index + 1;
    return 0;
}
}
