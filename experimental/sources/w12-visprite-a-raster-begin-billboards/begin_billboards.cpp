// Observed: BeginBillboards__8VIRasterPP11VIBillboardi at 0x0110cb90, 1616
// bytes. Arguments: the raster r, the address of a pointer slot and a count.
//   The count is limited to 80. The material m = Material__8VIRasteri(r, the
//   int at r + 17824) is fetched. When the limited count is not positive, the
//   slot gets 0, the word at r + 17860 gets 0 and the result is 0.
//   Otherwise: if the word at r + 19532 is not 2,
//   UploadBillboardMicro__8VIRasterPP10_sceDmaTag(r, r + 19492). When the word
//   at r + 17892 is non-zero, the matrix block is queued: CalcMatrices__8VIRaster(r);
//   a zeroed 64-byte local matrix; P = the word at r + 19492 and the packet
//   written from P + 16 (16-byte quadwords, written as 64-bit halves where
//   the image stores doublewords):
//     {0, 0, 0x13000000, 0x6c0c0000}; the 64 bytes at scratchpad 0x70000000;
//     Mul__10VIMatrix44RC10VIMatrix44T1(local, 0x70000040, 0x700000c0) and
//     the 64 local bytes; the 64 bytes at scratchpad 0x70000080;
//     {0, 0, 0, 0x6c040010}; {0xb000000000008000, 0x0000044242424421};
//     {0x202a400000008001, 66}; {0, 0, 0x3f800000, 0}; {0x2000, 0x2000, 0x2000, 0};
//     {0, 0, 0, 0x6c1c0020}; then 28 quadwords whose low doubleword is taken
//     from a fixed table (below) and whose high doubleword is 0x3f800000;
//     {0, 0, 0, 0x6c02000c}; {1.0, 1.0, 1.0, 1.0}; a float quadword
//     {the float at r + 17780, the float at r + 17784, 128.0, 128.0} when the
//     word at m + 60 is zero, or {..., 128.0, 64.0} when it is non-zero;
//     and {0, 0, 0, 0x14000000}, which is the last quadword.
//   The tag at P is then finished: its byte 3 = 16, its word at +4 = 0 and its
//   halfword at +0 = the count of quadwords from P + 16 up to and including the
//   last one. The word at r + 19492 becomes the address after the last
//   quadword, and only then FlushDMABufferIfFull__8VIRaster(r) is called. The
//   word at r + 17892 is cleared and _$_10VIMatrix44(local, 2) is called.
//   Common ending (also reached directly when r + 17892 is zero): with P the
//   word at r + 19492 as it now is, the quadwords at P + 16 are
//   {0, 0, 0x13000000, ((3 * n + 1) << 16) | 0x6c008000} and {n, 0, 0, 0}; the
//   slot receives P + 48, where the caller is expected to write 3 quadwords per
//   billboard. The word at r + 17860 is set to n and n is returned.
// The low doublewords of the 28 table quadwords are listed in kBillboardTable; each
// high doubleword is 0x3f800000.
// Inferred: a VIF/GIF packet header for batched billboard drawing; the three
// matrices are copies of scratchpad state and their product; EndBillboards
// closes the packet. The meaning of the table words (they look like pairs of
// floats and 2-bit fixed point values) and of the packet qualifiers is not
// established.
// Unknown: member names and the unit of the material flag at m + 60.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char* norrath_host_scratchpad(unsigned address);
#define NORRATH_SCRATCH(address) norrath_host_scratchpad(address)
#else
#define NORRATH_SCRATCH(address) reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
void* Material__8VIRasteri(void* raster, int handle);
void UploadBillboardMicro__8VIRasterPP10_sceDmaTag(void* raster, void* slot);
void CalcMatrices__8VIRaster(void* raster);
void Mul__10VIMatrix44RC10VIMatrix44T1(void* result, const void* left, const void* right);
void FlushDMABufferIfFull__8VIRaster(void* raster);
void _$_10VIMatrix44(void* matrix, int flags);

static const unsigned long long kBillboardTable[28] = {
    0xfe00ull << 46, 0ull, 0x3f8000003f800000ull, 0x3f800000ull, 0xfc00ull << 46, 0ull,
    0x3f0000003f000000ull, 0x3f000000ull, 0x3f0000003f000000ull, 0x3f000000ull,
    0x3f0000003f800000ull, 0x3f800000ull, 0xfe00ull << 46, 0xfc00ull << 46,
    0x3f8000003f000000ull, 0x3f0000003f000000ull, 0x3f8000003f000000ull,
    0x3f0000003f000000ull, 0x3f8000003f800000ull, 0x3f0000003f800000ull,
    0xfe00ull << 46, 0ull, 0x3f8000003f000000ull, 0x3f000000ull,
    0x3f8000003f000000ull, 0x3f000000ull, 0x3f8000003f800000ull, 0x3f800000ull};

static void quad(unsigned*& q, unsigned a, unsigned b, unsigned c, unsigned d) {
    q[0] = a;
    q[1] = b;
    q[2] = c;
    q[3] = d;
    q += 4;
}

static void copy64(unsigned*& q, const unsigned char* from) {
    const unsigned long long* s = reinterpret_cast<const unsigned long long*>(from);
    unsigned long long* d = reinterpret_cast<unsigned long long*>(q);
    for (int i = 0; i < 8; ++i) d[i] = s[i];
    q += 16;
}

int BeginBillboards__8VIRasterPP11VIBillboardi(void* raster, void** out, int count) {
    unsigned char* r = static_cast<unsigned char*>(raster);
    if (count > 80) count = 80;
    unsigned char* material = static_cast<unsigned char*>(Material__8VIRasteri(raster, *reinterpret_cast<int*>(r + 17824)));
    if (count <= 0) {
        *out = 0;
        *reinterpret_cast<int*>(r + 17860) = 0;
        return 0;
    }
    if (*reinterpret_cast<int*>(r + 19532) != 2) UploadBillboardMicro__8VIRasterPP10_sceDmaTag(raster, r + 19492);
    if (*reinterpret_cast<int*>(r + 17892) != 0) {
        CalcMatrices__8VIRaster(raster);
        alignas(16) float local[16] = {0};
        unsigned char* tag = *reinterpret_cast<unsigned char**>(r + 19492);
        unsigned* q = reinterpret_cast<unsigned*>(tag + 16);
        quad(q, 0, 0, 0x13000000u, 0x6c0c0000u);
        copy64(q, NORRATH_SCRATCH(0x70000000));
        Mul__10VIMatrix44RC10VIMatrix44T1(local, NORRATH_SCRATCH(0x70000040), NORRATH_SCRATCH(0x700000c0));
        copy64(q, reinterpret_cast<unsigned char*>(local));
        copy64(q, NORRATH_SCRATCH(0x70000080));
        quad(q, 0, 0, 0, 0x6c040010u);
        unsigned long long* dq = reinterpret_cast<unsigned long long*>(q);
        dq[0] = 0xb000000000008000ull;
        dq[1] = 0x0000044242424421ull;
        dq[2] = 0x202a400000008001ull;
        dq[3] = 66ull;
        q += 8;
        quad(q, 0, 0, 0x3f800000u, 0);
        quad(q, 0x2000, 0x2000, 0x2000, 0);
        quad(q, 0, 0, 0, 0x6c1c0020u);
        dq = reinterpret_cast<unsigned long long*>(q);
        for (int i = 0; i < 28; ++i) {
            dq[0] = kBillboardTable[i];
            dq[1] = 0x3f800000ull;
            dq += 2;
        }
        q += 28 * 4;
        quad(q, 0, 0, 0, 0x6c02000cu);
        quad(q, 0x3f800000u, 0x3f800000u, 0x3f800000u, 0x3f800000u);
        unsigned lastWord = *reinterpret_cast<int*>(material + 60) == 0 ? 0x43000000u : 0x42800000u;
        quad(q, *reinterpret_cast<unsigned*>(r + 17780), *reinterpret_cast<unsigned*>(r + 17784), 0x43000000u, lastWord);
        q[0] = 0;
        q[3] = 0x14000000u;
        q[1] = 0;
        q[2] = 0;
        unsigned quadwords = static_cast<unsigned>(reinterpret_cast<unsigned char*>(q) - tag) >> 4;
        tag[3] = 16;
        *reinterpret_cast<unsigned*>(tag + 4) = 0;
        *reinterpret_cast<unsigned short*>(tag) = static_cast<unsigned short>(quadwords);
        *reinterpret_cast<unsigned char**>(r + 19492) = tag + 16 * quadwords + 16;
        FlushDMABufferIfFull__8VIRaster(raster);
        *reinterpret_cast<int*>(r + 17892) = 0;
        _$_10VIMatrix44(local, 2);
    }
    unsigned* q = reinterpret_cast<unsigned*>(*reinterpret_cast<unsigned char**>(r + 19492) + 16);
    quad(q, 0, 0, 0x13000000u, (static_cast<unsigned>(3 * count + 1) << 16) | 0x6c008000u);
    quad(q, static_cast<unsigned>(count), 0, 0, 0);
    *out = q;
    *reinterpret_cast<int*>(r + 17860) = count;
    return count;
}
}
