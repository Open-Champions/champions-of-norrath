/* Observed sceGsResetGraph at 0x002d16e8, 400 bytes. Each argument is
 * sign-extended to 16 bits. Mode 1 stores 0x100 as a 64-bit word at
 * 0x12001000 and returns. Mode 0 stores 0x200 there, records interlace and
 * the frame mode in gp.15, stores bits 16..23 of that word at offset 6,
 * calls GsPutIMR(0xff00), and stores 1 at offset 4 when the version is
 * nonzero. A nonzero word at offset 8 then calls DisableIntc(2) and
 * RemoveIntcHandler(2, offset 12) and clears both words. It tail-calls
 * SetGsCrt with interlace bit 0, the low eight frame-mode bits, and version
 * bit 0. Mode 5 updates the same halves from the current word at
 * 0x12001000 without storing 0x200, without GsPutIMR, and without removing
 * a handler, then calls SetGsCrt the same way. Every other mode returns
 * without those calls. This source is not an original-byte match.
 */
extern "C" unsigned char* sceGsGetGParam();
extern "C" void GsPutIMR(int mask);
extern "C" unsigned long long DisableIntc(int cause);
extern "C" int RemoveIntcHandler(int cause, int handler);
extern "C" void SetGsCrt(int interlace, int mode, int field);

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned long long norrath_host_gs_csr;
extern "C" unsigned long long norrath_host_gs_written;
static unsigned long long norrath_read_csr() { return norrath_host_gs_csr; }
static void norrath_write_csr(unsigned long long value) { norrath_host_gs_written = value; }
#else
static unsigned long long norrath_read_csr() {
    return *reinterpret_cast<volatile unsigned long long*>(0x12001000u);
}
static void norrath_write_csr(unsigned long long value) {
    *reinterpret_cast<volatile unsigned long long*>(0x12001000u) = value;
}
#endif

static short norrath_half(int value) { return static_cast<short>(value); }

static void norrath_store_half(unsigned char* param, int offset, int value) {
    const unsigned bits = static_cast<unsigned>(static_cast<unsigned short>(value));
    param[offset] = static_cast<unsigned char>(bits);
    param[offset + 1] = static_cast<unsigned char>(bits >> 8);
}

static int norrath_load_word(const unsigned char* param, int offset) {
    return static_cast<int>(param[offset]
        | (param[offset + 1] << 8)
        | (param[offset + 2] << 16)
        | (param[offset + 3] << 24));
}

static void norrath_store_word(unsigned char* param, int offset, int value) {
    const unsigned bits = static_cast<unsigned>(value);
    param[offset] = static_cast<unsigned char>(bits);
    param[offset + 1] = static_cast<unsigned char>(bits >> 8);
    param[offset + 2] = static_cast<unsigned char>(bits >> 16);
    param[offset + 3] = static_cast<unsigned char>(bits >> 24);
}

extern "C" void sceGsResetGraph(int mode_in, int interlace_in, int frame_in, int version_in) {
    const short mode = norrath_half(mode_in);
    const short interlace = norrath_half(interlace_in);
    const short frame = norrath_half(frame_in);
    const short version = norrath_half(version_in);
    if (mode == 1) {
        norrath_write_csr(0x100ull);
        return;
    }
    if (mode != 0 && mode != 5)
        return;
    unsigned char* param = sceGsGetGParam();
    if (mode == 0) {
        norrath_write_csr(0x200ull);
        norrath_store_half(param, 0, interlace);
        norrath_store_half(param, 2, frame);
        const unsigned long long csr = norrath_read_csr();
        norrath_store_half(param, 6, static_cast<int>((csr >> 16) & 0xffull));
        GsPutIMR(0xff00);
        norrath_store_half(param, 4, version != 0 ? 1 : 0);
        if (norrath_load_word(param, 8) != 0) {
            DisableIntc(2);
            RemoveIntcHandler(2, norrath_load_word(param, 12));
            norrath_store_word(param, 12, 0);
            norrath_store_word(param, 8, 0);
        }
    } else {
        norrath_store_half(param, 4, version != 0 ? 1 : 0);
        const unsigned long long csr = norrath_read_csr();
        norrath_store_half(param, 0, interlace);
        norrath_store_half(param, 2, frame);
        norrath_store_half(param, 6, static_cast<int>((csr >> 16) & 0xffull));
    }
    SetGsCrt(interlace & 1, frame & 0xff, version & 1);
}
