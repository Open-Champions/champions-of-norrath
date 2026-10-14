// Observed: Set__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoR14VIMaterialPass at 0x01132800 is 800 bytes in the pinned SLUS-20565
// image. Arguments: the cache (only passed on to Log2__13VIEESurfCachei), a surface s, a cache-info record d (the one filled by
// CalcCacheInfo__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfo), and a material pass p. It fills four 16-byte entries of the pass: a
// 64-bit value at p+80/96/112/128 and a register number at p+88/104/120/136 (6, 20, 52 and 54), and returns 0. The values are
// composed from these inputs (w = s[8], h = s[12], f = d[20], flags = p[4], all 32-bit words; d[k] means the word at d+k):
//   m = 2 if f is 19 or 20, else 1.
//   p+80  = (d[32] >> 8) | tbw << 14 | f << 20 | (log2(w) - 1) << 26 | (log2(h) - 1) << 30 | 1 << 34 | (flags & 1 ? 0 : 1 << 35)
//           and, when s[40] is nonzero, also (d[12] >> 8) << 37 | d[4] << 51 | 1 << 61. Here tbw = max(w >> 6, 2) if m is 2, else
//           max(w >> 6, 1), and log2 is Log2__13VIEESurfCachei(cache, value) called for w first and then for h.
//   p+96  = (d[28] - 1) << 2 | ((int8)(int)(float at p+52 * -127.0f)) << 32 | (flags & 8 ? 320 : 352)
//   p+112 = (d[36] >> 8) | max(w >> 7, m) << 14 | (d[40] >> 8) << 20 | max(w >> 8, m) << 34 | (d[44] >> 8) << 40 | max(w >> 9, m) << 54
//   p+128 = (a >> 8) | max(w >> 10, m) << 14 | (b >> 8) << 20 | max(w >> 11, m) << 34 | (c >> 8) << 40 | max(w >> 12, m) << 54, with
//           (a, b, c) = (d[48], d[52], d[56]) when m is 2 and (d[36], d[40], d[44]) when m is 1 (the m == 1 case reuses the same three
//           words as p+112, as observed).
// All shifts of d words are 64-bit arithmetic shifts of the sign-extended word. The int8 conversion keeps the low byte of the converted
// integer, sign-extended, placed at bit 32.
// Inferred: this builds the GIF A+D register writes for a texture bound to the EE surface cache: TEX0 (register 6), TEX1 (20), MIPTBP1
// (52, 0x34) and MIPTBP2 (54, 0x36) of GS context 1, from the layout CalcCacheInfo computed. The bit positions match those register
// formats; the mipmap base-pointer words d[32..56] are per-level offsets. The float at p+52 times -127 would be the mipmap K (LOD bias).
// Unknown: the meaning of the d[4]/d[12] palette words beyond their use here, the m == 1 reuse of the level 1..3 words for levels 4..6,
// and who owns the entries' 8 trailing bytes. Nothing here ran on the EE.
extern "C" {
int Log2__13VIEESurfCachei(unsigned char* self, int value);

static long long mip_word(int word) { return static_cast<long long>(word) >> 8; }

int Set__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoR14VIMaterialPass(unsigned char* self, unsigned char* surface, unsigned char* info,
                                                                          unsigned char* pass) {
    int* s = reinterpret_cast<int*>(surface);
    int* d = reinterpret_cast<int*>(info);
    int width = s[2];
    int tbw = width >> 6;
    if (tbw < 2) tbw = 2;
    const bool palette = s[10] != 0;
    int height = s[3];
    if (!(static_cast<unsigned>(d[5] - 19) < 2u)) {
        tbw = width >> 6;
        if (!(0 < tbw)) tbw = 1;
    }
    int widthLog = Log2__13VIEESurfCachei(self, width);
    int heightLog = Log2__13VIEESurfCachei(self, height);

    long long tex0 = mip_word(d[8]) | (static_cast<long long>(tbw) << 14);
    tex0 |= (static_cast<long long>(widthLog - 1) << 26) | (static_cast<long long>(heightLog - 1) << 30);
    tex0 |= static_cast<long long>(d[5]) << 20;
    tex0 |= 1ll << 34;
    int flags = *reinterpret_cast<int*>(pass + 4);
    if ((flags & 1) == 0) tex0 |= 1ll << 35;
    if (palette) tex0 |= (mip_word(d[3]) << 37) | (static_cast<long long>(d[1]) << 51);
    if (palette) tex0 |= 1ll << 61;
    *reinterpret_cast<long long*>(pass + 80) = tex0;
    *reinterpret_cast<long long*>(pass + 88) = 6;

    float bias = *reinterpret_cast<float*>(pass + 52) * -127.0f;
    int biasInt = static_cast<int>(bias);
    long long tex1 = (static_cast<long long>(d[7] - 1) << 2) | (static_cast<long long>(static_cast<signed char>(biasInt)) << 32) | ((flags & 8) ? 320 : 352);
    *reinterpret_cast<long long*>(pass + 96) = tex1;
    *reinterpret_cast<long long*>(pass + 104) = 20;

    int m = static_cast<unsigned>(d[5] - 19) < 2u ? 2 : 1;
    int w7 = width >> 7;
    int w8 = width >> 8;
    int w9 = width >> 9;
    if (w7 < m) w7 = m;
    if (w9 < m) w9 = m;
    if (w8 < m) w8 = m;
    long long miptbp1 = mip_word(d[9]) | (static_cast<long long>(w7) << 14) | (mip_word(d[10]) << 20) | (static_cast<long long>(w8) << 34) |
                        (mip_word(d[11]) << 40) | (static_cast<long long>(w9) << 54);
    *reinterpret_cast<long long*>(pass + 112) = miptbp1;
    *reinterpret_cast<long long*>(pass + 120) = 52;

    int a;
    int b;
    int c;
    if (static_cast<unsigned>(d[5] - 19) < 2u) {
        m = 2;
        a = d[12];
        b = d[13];
        c = d[14];
    } else {
        m = 1;
        a = d[9];
        b = d[10];
        c = d[11];
    }
    int w10 = width >> 10;
    int w11 = width >> 11;
    int w12 = width >> 12;
    if (w10 < m) w10 = m;
    if (w12 < m) w12 = m;
    if (w11 < m) w11 = m;
    long long miptbp2 = mip_word(a) | (static_cast<long long>(w12) << 54) | (static_cast<long long>(w11) << 34) | (mip_word(b) << 20) |
                        (static_cast<long long>(w10) << 14) | (mip_word(c) << 40);
    *reinterpret_cast<long long*>(pass + 128) = miptbp2;
    *reinterpret_cast<long long*>(pass + 136) = 54;
    return 0;
}
}
