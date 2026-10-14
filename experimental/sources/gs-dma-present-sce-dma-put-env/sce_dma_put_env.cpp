/* Observed sceDmaPutEnv at 0x002d2db0, 472 bytes. It reads 0x1000e000,
 * 0x1000e020, 0x1000e030, 0x1000e050, and 0x1000e040 before checking the
 * environment. Bytes 0, 1, and 2 must be below 10 or the result is -1, -2,
 * or -3. Byte 3 must be below 7 or the result is -4. The three byte tables
 * at 0x0047b3b0, 0x0047b3c0, and 0x0047b3d0 supply the bit fields merged
 * into the 0x1000e000 word. Halves at offsets 4 and 6 go to 0x1000e020,
 * halves at 10 and 8 go to 0x1000e030, the word at 12 goes to 0x1000e050,
 * and the word at 16 goes to 0x1000e040. Twenty bytes are copied to
 * sceDmaCurrentEnv (0x008dd830). A completed call returns 0. Fresh
 * compilation is not an original-byte match.
 */
extern "C" unsigned char sceDmaCurrentEnv[];

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_dma_env_load32(unsigned address);
extern "C" void norrath_host_dma_env_store32(unsigned address, unsigned value);
#define NORRATH_LOAD32(address) norrath_host_dma_env_load32(address)
#define NORRATH_STORE32(address, value) norrath_host_dma_env_store32((address), (value))
#else
static inline unsigned norrath_dma_env_load32(unsigned address) {
    return *reinterpret_cast<volatile unsigned*>(static_cast<unsigned long>(address));
}
static inline void norrath_dma_env_store32(unsigned address, unsigned value) {
    *reinterpret_cast<volatile unsigned*>(static_cast<unsigned long>(address)) = value;
}
#define NORRATH_LOAD32(address) norrath_dma_env_load32(address)
#define NORRATH_STORE32(address, value) norrath_dma_env_store32((address), (value))
#endif

static const unsigned char norrath_dma_env_table0[10] = {0, 0, 0, 3, 0, 1, 0, 2, 0, 0};
static const unsigned char norrath_dma_env_table1[10] = {0, 1, 2, 0, 0, 0, 3, 0, 0, 0};
static const unsigned char norrath_dma_env_table2[10] = {0, 2, 3, 0, 0, 0, 0, 0, 0, 0};

static unsigned short norrath_env_half(const unsigned char* env, int offset) {
    return static_cast<unsigned short>(env[offset] | (env[offset + 1] << 8));
}

static unsigned norrath_env_word(const unsigned char* env, int offset) {
    return static_cast<unsigned>(env[offset]) |
           (static_cast<unsigned>(env[offset + 1]) << 8) |
           (static_cast<unsigned>(env[offset + 2]) << 16) |
           (static_cast<unsigned>(env[offset + 3]) << 24);
}

extern "C" int sceDmaPutEnv(const unsigned char* env) {
    const unsigned ctrl = NORRATH_LOAD32(0x1000e000u);
    (void)NORRATH_LOAD32(0x1000e020u);
    (void)NORRATH_LOAD32(0x1000e030u);
    (void)NORRATH_LOAD32(0x1000e050u);
    (void)NORRATH_LOAD32(0x1000e040u);
    const unsigned char stall_source = env[0];
    if (stall_source >= 10u) return -1;
    const unsigned char stall_drain = env[1];
    if (stall_drain >= 10u) return -2;
    const unsigned char fifo_drain = env[2];
    if (fifo_drain >= 10u) return -3;
    const unsigned char release = env[3];
    if (release >= 7u) return -4;

    unsigned merged = (ctrl & 0xffffffcfu) | (static_cast<unsigned>(norrath_dma_env_table0[stall_source]) << 4);
    merged = (merged & 0xffffff3fu) | (static_cast<unsigned>(norrath_dma_env_table1[stall_drain]) << 6);
    merged = (merged & 0xfffffff3u) | (static_cast<unsigned>(norrath_dma_env_table2[fifo_drain]) << 2);
    if (release == 0u) {
        merged &= 0xfffffffdu;
    } else {
        merged = (merged | 0x2u) & 0xfffffcffu;
        merged |= (static_cast<unsigned>(release - 1u) << 8);
    }

    const unsigned squish = (static_cast<unsigned>(norrath_env_half(env, 4)) << 16) |
                            norrath_env_half(env, 6);
    const unsigned queue = (static_cast<unsigned>(norrath_env_half(env, 10)) << 16) |
                           norrath_env_half(env, 8);
    const unsigned ring = norrath_env_word(env, 12);
    const unsigned ring_size = norrath_env_word(env, 16);
    NORRATH_STORE32(0x1000e000u, merged);
    NORRATH_STORE32(0x1000e020u, squish);
    NORRATH_STORE32(0x1000e030u, queue);
    NORRATH_STORE32(0x1000e050u, ring);
    NORRATH_STORE32(0x1000e040u, ring_size);
    for (int index = 0; index < 16; ++index)
        sceDmaCurrentEnv[index] = env[index];
    sceDmaCurrentEnv[16] = env[16];
    sceDmaCurrentEnv[17] = env[17];
    sceDmaCurrentEnv[18] = env[18];
    sceDmaCurrentEnv[19] = env[19];
    return 0;
}
