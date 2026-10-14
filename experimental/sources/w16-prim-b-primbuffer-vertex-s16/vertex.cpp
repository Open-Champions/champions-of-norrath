// Observed: Vertex__12VIPrimBufferR10VIVect3S16 at 0x01101718 is 1412 bytes. The type word at
// offset 8 selects the layout; types 2, 4, 5 and 8 are accepted and any other value returns -1
// without writing. In every accepted case, with E the word at offset 0 of the 28-byte entry at
// (pointer at offset 92) + (word at offset 72) * 28, v the 16-byte slot at (pointer at offset 96) +
// (word at offset 80) * 16, p the argument's three halfwords, uv the halfwords of
// _12VIPrimBuffer$PrimUV0S16 (0x011b0118), n the three bytes of _12VIPrimBuffer$PrimNormalS8
// (0x011b0140), c the four bytes of _12VIPrimBuffer$PrimColor (0x011b0148), and r(x) the int
// (truncated) of 1.0f / (float at object offset x):
//   type 8 (one slot):   v+0 word = (E*2+1)|0x69028000; v+4,6,8 = p; v+10 = uv[0]; v+12 = uv[1];
//                        v+14 = r(52); E += 1; word at 80 += 1.
//   type 2 (two slots):  v+0 word = (E*4+1)|0x6d028000; v+4,6,8 = p; v+10 = r(48); v+12 = uv[0];
//                        v+14 = uv[1]; v+16 = r(52); v+18 = 0; v+20 word = (E*4+3)|0x6e028000;
//                        v+24,25,26 = n[0],n[1],n[2]; v+27 = 0; v+28..31 = c[i] >> 1;
//                        then E += 1; word at 80 += 2.
//   type 4 (two slots):  as type 2 except v+10 = the halfword _12VIPrimBuffer$PrimVertexGroup
//                        (0x01228a30) and v+16 = r(52) only; r(48) is not computed.
//   type 5 (two slots):  v+0 word = (E*5+1)|0x69028000; v+4,6,8 = p; v+10 = uv[0]; v+12 = uv[1];
//                        v+14 = r(52); v+16 word = (E*5+3)|0x6e038000; v+20,21,22 = n[0],n[1],n[2];
//                        v+23 = 0; v+24..27 = the four bytes of _12VIPrimBuffer$PrimWeightsS8
//                        (0x011b0170); v+28..31 = the four bytes of _12VIPrimBuffer$PrimBones
//                        (0x011b0150) with every byte equal to 255 replaced by 0; E += 1; word
//                        at 80 += 2.
// It returns 0.
// Inferred: the high word constants look like packed command words whose low 15 bits and the
// multiplier track the number of 16-byte slots per vertex; this is a guess and nothing checks it.
// The r() values are fixed-point reciprocals of the pack scales set by SetPacking__12VIPrimBufferiii.
// Unknown: the meaning of the type numbers, the consumer of the slot array, what the high words
// encode, and whether the halfword at v+18 (type 2/4) is padding.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern unsigned short PrimUV0S16[2] __asm__("_12VIPrimBuffer$PrimUV0S16");
extern unsigned char PrimNormalS8[3] __asm__("_12VIPrimBuffer$PrimNormalS8");
extern unsigned char PrimColor[4] __asm__("_12VIPrimBuffer$PrimColor");
extern unsigned char PrimBones[4] __asm__("_12VIPrimBuffer$PrimBones");
extern unsigned char PrimWeightsS8[4] __asm__("_12VIPrimBuffer$PrimWeightsS8");
extern unsigned short PrimVertexGroup __asm__("_12VIPrimBuffer$PrimVertexGroup");

static inline int* entryCount(unsigned char* self) {
    return reinterpret_cast<int*>(at(*reinterpret_cast<unsigned*>(self + 92)) + *reinterpret_cast<int*>(self + 72) * 28);
}

static inline unsigned short reciprocal(unsigned char* self, int offset) {
    return static_cast<unsigned short>(static_cast<int>(1.0f / *reinterpret_cast<float*>(self + offset)));
}

extern "C" {
int Vertex__12VIPrimBufferR10VIVect3S16(unsigned char* self, const unsigned short* p) {
    int type = *reinterpret_cast<int*>(self + 8);
    if (type != 2 && type != 4 && type != 5 && type != 8)
        return -1;
    unsigned char* v = at(*reinterpret_cast<unsigned*>(self + 96)) + *reinterpret_cast<int*>(self + 80) * 16;
    unsigned E = static_cast<unsigned>(*entryCount(self));
    if (type == 8) {
        *reinterpret_cast<unsigned*>(v) = (E * 2 + 1) | 0x69028000u;
        *reinterpret_cast<unsigned short*>(v + 4) = p[0];
        *reinterpret_cast<unsigned short*>(v + 6) = p[1];
        *reinterpret_cast<unsigned short*>(v + 8) = p[2];
        *reinterpret_cast<unsigned short*>(v + 10) = PrimUV0S16[0];
        *entryCount(self) = *entryCount(self) + 1;
        *reinterpret_cast<unsigned short*>(v + 14) = reciprocal(self, 52);
        *reinterpret_cast<unsigned short*>(v + 12) = PrimUV0S16[1];
        *reinterpret_cast<int*>(self + 80) = *reinterpret_cast<int*>(self + 80) + 1;
        return 0;
    }
    if (type == 5) {
        *reinterpret_cast<unsigned*>(v) = (E * 5 + 1) | 0x69028000u;
        *reinterpret_cast<unsigned short*>(v + 4) = p[0];
        *reinterpret_cast<unsigned short*>(v + 6) = p[1];
        *reinterpret_cast<unsigned short*>(v + 8) = p[2];
        *reinterpret_cast<unsigned short*>(v + 10) = PrimUV0S16[0];
        *reinterpret_cast<unsigned short*>(v + 14) = reciprocal(self, 52);
        *reinterpret_cast<unsigned short*>(v + 12) = PrimUV0S16[1];
        *reinterpret_cast<unsigned*>(v + 16) = (E * 5 + 3) | 0x6e038000u;
        v[20] = PrimNormalS8[0];
        v[21] = PrimNormalS8[1];
        v[23] = 0;
        v[22] = PrimNormalS8[2];
        for (int i = 0; i < 4; ++i)
            v[28 + i] = PrimBones[i] == 255 ? 0 : PrimBones[i];
        for (int i = 0; i < 4; ++i)
            v[24 + i] = PrimWeightsS8[i];
    } else {
        *reinterpret_cast<unsigned*>(v) = (E * 4 + 1) | 0x6d028000u;
        *reinterpret_cast<unsigned short*>(v + 4) = p[0];
        *reinterpret_cast<unsigned short*>(v + 6) = p[1];
        *reinterpret_cast<unsigned short*>(v + 8) = p[2];
        if (type == 2)
            *reinterpret_cast<unsigned short*>(v + 10) = reciprocal(self, 48);
        else
            *reinterpret_cast<unsigned short*>(v + 10) = PrimVertexGroup;
        *reinterpret_cast<unsigned short*>(v + 12) = PrimUV0S16[0];
        *reinterpret_cast<unsigned short*>(v + 16) = reciprocal(self, 52);
        *reinterpret_cast<unsigned short*>(v + 14) = PrimUV0S16[1];
        *reinterpret_cast<unsigned*>(v + 20) = (E * 4 + 3) | 0x6e028000u;
        *reinterpret_cast<unsigned short*>(v + 18) = 0;
        v[24] = PrimNormalS8[0];
        v[25] = PrimNormalS8[1];
        v[27] = 0;
        v[26] = PrimNormalS8[2];
        for (int i = 0; i < 4; ++i)
            v[28 + i] = static_cast<unsigned char>(PrimColor[i] >> 1);
    }
    *entryCount(self) = *entryCount(self) + 1;
    *reinterpret_cast<int*>(self + 80) = *reinterpret_cast<int*>(self + 80) + 2;
    return 0;
}
}
